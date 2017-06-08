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
    QByteArray LabJackController::read(int length)
    {
        // Hold the reads data
        unsigned char data[10];

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
            byteLength = LJUSB_Read(device, data, 10);
        #endif

        // Check for valid data
        if(byteLength < length)
        {
            // Tell the application we failed
            emit_critialLabJackError(errorPackageGenerator("Device1", "Device1", "Could not read any data from the labjack"));

            // Return empty array
            QByteArray returnEmpty;
            return returnEmpty;
        }

        // Temp debug message
        qDebug() << "The data received was: " << QByteArray::fromRawData((char*) data, byteLength) << " Length of: " << byteLength;

        // Format char into QByteArray
        return QByteArray::fromRawData((char*) data, byteLength);
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
        qDebug() << "The data send was: " << QByteArray::fromRawData((char*) data, byteLength) << " Length of: " << byteLength;

        // Success
        return true;
    }


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
