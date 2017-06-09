#include "LabJackController.h"

// Include external deps
#include <QByteArray>
#include <qDebug>

// Import correct labjack driver
#ifdef _WIN32
    // Using labjackUD library for windows as it uses WinUsb
    #include "../../vendor/labjack/windowsUD/LabJackUD.h"
#else
    // Using the exodriver for everything else as this uses usblib-1.0
    #include "../../vendor/labjack/exodriver/labjackusb.h"
#endif

namespace App { namespace Services
{

    LabJackController::LabJackController(QObject *parent)
        :   QObject(parent)
    {

    }


    LabJackController::~LabJackController()
    {

    }


    /**
     * Open the first lab jack U3 on the PC
     *
     * @brief LabJackController::open
     * @return
     */
    bool LabJackController::open()
    {

        // Run the correct method using the correct library
        #ifdef _WIN32
            // Using labjackUD library
            OpenLabJack (LJ_dtU3, LJ_ctUSB, "1", 1, &device);
        #else
            // Using the exodriver
            device = LJUSB_OpenDevice(1, 0, U3_PRODUCT_ID);
        #endif

        if(device)
        {
            // Send the connect sucessfull signal
            emit_comConnectionStatus(comConnectionPackageGenerator("Device1", true));

            // Bus not open
            isOpen = false;

            return true;
        }

        // Send the connect sucessfull signal
        emit_comConnectionStatus(comConnectionPackageGenerator("Device1", false));

        // Bus open
        isOpen = true;

        return false;
    }


    /**
     * Close the current labjack connection
     *
     * @brief LabJackController::close
     */
    void LabJackController::close()
    {
        // Run the correct close connection method using the correct library
        #ifdef _WIN32
            // Using labjackUD library
            Close();
        #else
            // Using the exodriver
            LJUSB_CloseDevice(device);
        #endif

        // Send the connect sucessfull signal
        emit_comConnectionStatus(comConnectionPackageGenerator("Device1", false));

        // Bus not open
        isOpen = false;
    }


    /**
     * Read a packet of data from the labjack
     *
     * @brief LabJackController::read
     * @param length
     * @return
     */
    QStringList LabJackController::read(int length)
    {
        // Hold the reads data
        unsigned char data[length];

        // Run the correct method using the correct library
        #ifdef _WIN32
            // Number of bytes to read
            double byteLength = (double) length;

            // Using labjackUD library
            eGetPtr(device, LJ_ioRAW_IN, 0, &byteLength, &data);
        #else
            // Number of bytes to be read
            unsigned long byteLength = (unsigned int) length;

            // Using the exodriver
            byteLength = LJUSB_Read(device, data, byteLength);
        #endif


        // Check for valid data
        if(byteLength < length)
        {
            // Tell the application we failed
            emit_critialLabJackError(errorPackageGenerator("Device1", "Device1", "Could not read any data from the labjack"));

            // Return empty array
            QStringList returnEmpty;
            return returnEmpty;
        }

        // Data
        QByteArray dataArray = QByteArray::fromRawData((char*) data, byteLength);

        // For each btye we will convert to ascii format then sort in read container
        QStringList asciiContainer;
        for (int i = 0; i < dataArray.size(); ++i)
        {
             asciiContainer.append(QString::number((unsigned char) dataArray.at(i)));
        }

        // Temp debug message
        //qDebug() << "The data received was: " << QByteArray::fromRawData((char*) data, byteLength) << " Length of: " << byteLength;

        // Format char into QByteArray
        return asciiContainer;
    }


    /**
     * Write a packet of data to the labjack
     *
     * @brief LabJackController::write
     * @param package
     * @return
     */
    bool LabJackController::write(QByteArray package)
    {
        // Convert QBtyeArray to char
        unsigned char data[package.size()];
        for (int i = 0; i < package.size(); i++) {
            data[i] = package.at(i);
        }

        // Get the size of the data
        int length = package.size();

        // Run the correct method using the correct library
        #ifdef _WIN32
            // Number of bytes to read
            double byteLength = (double) length;

            // Using labjackUD library
            eGetPtr(device, LJ_ioRAW_OUT, 0, &byteLength, &data);
        #else
            // Number of bytes to be read
            unsigned long byteLength = (unsigned int) length;

            // Using the exodriver
            byteLength = LJUSB_Write(device, data, byteLength);
        #endif

        // Check for valid data
        if(byteLength < package.size())
        {
            // Tell the application we failed
            emit_critialLabJackError(errorPackageGenerator("Device1", "Device1", "Could not write any data to the labjack"));

            // Failure
            return false;
        }

        // Temp debug message
        //qDebug() << "The data send was: " << QByteArray::fromRawData((char*) data, byteLength) << " Length of: " << byteLength;

        // Success
        return true;
    }


    /**
     * Helper that calculates the check sum 8 for a given package
     *
     * @brief LabJackController::checkSumEight
     * @param package
     * @return
     */
    QString LabJackController::checkSumEight(QStringList package)
    {
        int i, a, bb;

       //Sums bytes 1 to 5. Sums quotient and remainder of 256 division. Again,
       //sums quotient and remainder of 256 division.
       for( i = 1, a = 0; i < 6; i++ )
           a += (unsigned short) package.at(i).toInt();

       bb = a / 256;
       a = (a - 256 * bb) + bb;
       bb = a / 256;

       return QString::number( (a - 256 * bb) + bb );
    }


    /**
     * Helper that calcuates the check sum 16 for a given package
     *
     * @brief LabJackController::checkSumSixteen
     * @param package
     * @return
     */
    QString LabJackController::checkSumSixteen(QStringList package)
    {
        int i, a = 0;

        //Sums bytes 6 to n-1 to a unsigned 2 byte value
        for( i = 6; i < package.size(); i++ )
        {
            a += (unsigned short) package.at(i).toInt();
        }

        return QString::number(a);
    }


    /**
     * Helper method to convert acsii port name to int representation
     *      0-7=FIO
     *      8-15=EIO
     *      16-19=CIO
     *
     * @brief LabJackController::portValueFromName
     * @param name
     * @return
     */
    int LabJackController::portValueFromName(QString name)
    {
        // Hold the port value
        int port;

        // Return the correct value
        if(name.mid(0,3) == "FIO")
        {
            return name.mid(3,name.size()).toInt();
        }
        else if(name.mid(0,3) == "EIO")
        {
            return name.mid(3,name.size()).toInt() + 8;
        }
        else if(name.mid(0,3) == "CIO")
        {
            return name.mid(3,name.size()).toInt() + 16;
        }

        // No name found
        return 999;
    }


    /**
     * Helper method for generating error packages to send to the applcation in the event of a failure
     *
     * @brief LabJackController::errorPackageGenerator
     * @param com
     * @param port
     * @param error
     * @return
     */
    QVariantMap LabJackController::errorPackageGenerator(QString com, QString port, QString error)
    {
        // Create package to be emitted
        QVariantMap errorPackage;
        errorPackage["responsability"] = m_responsability;
        errorPackage["method"] = m_method;
        errorPackage["status"] = false;
        errorPackage["com"] = com;
        errorPackage["comAttempt"] = port;
        errorPackage["error"] = error;

        return errorPackage;
    }


    /**
     * Helper method for generating com status packages to send to the applcation in the event of com changes
     *
     * @brief LabJackController::comConnectionPackageGenerator
     * @param com
     * @param status
     * @return
     */
    QVariantMap LabJackController::comConnectionPackageGenerator(QString com, bool status)
    {
        // Create package to be emitted
        QVariantMap package;
        package["responsability"] = m_responsability;
        package["com"] = com;
        package["status"] = status;

        return package;
    }


}}
