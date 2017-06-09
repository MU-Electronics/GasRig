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








    /*##################################################
     #  Below this point are helper functions          #
     #  and not low level driver controller functions  #
     ###################################################*/






    /**
     * Create a type feedback function package to send to the lab jack
     *
     * @brief LabJackController::createPackageFeedback
     * @return
     */
    QByteArray LabJackController::createPackageFeedback(QStringList data)
    {
        // Calcuate the data word size ensure no decimals when grouped into 2's
        int wordLength = data.size() + 1;
        if( ( (wordLength) % 2 ) != 0 )
            wordLength++;

        // Create container for data
        QStringList stringPackage;
        stringPackage.insert(0, "0");                               // Checksum 8                  (place holder)
        stringPackage.insert(1, "248");                             // Command byte                (Always 248 for feedback function)
        stringPackage.insert(2, QString::number(wordLength/2));     // Number of data words        (.5 word for echo and 10.5 words for IO Type + data)
        stringPackage.insert(3, "0");                               // Extended command number     (Always 0)
        stringPackage.insert(4, "0");                               // Checksum 16 LSB             (place holder)
        stringPackage.insert(5, "0");                               // Checksum 16 MSB             (place holder)
        stringPackage.insert(6, "0");                               // Echo                        (Always 0)

        // Add the data to the package container
        for(int i = 0; i < data.size(); i++ )
        {
            stringPackage.append(data.at(i));
        }

        // If odd we add an addtion byte
        if( ( (data.size() + 1) % 2 ) != 0 )
            stringPackage.append("0");


        // Calcuate the check sum 16
        QString checkSumSixteenHex = QString("%1").arg(checkSumSixteen(stringPackage).toInt(), 4, 16, QChar('0'));

        // Calculate checksum16 MSB
        QString highCheckSum = QString("%1").arg(checkSumSixteenHex.mid(0,2), 0, 16);
        int highCheckSumDecimal = highCheckSum.toInt(__null, 16);

        // Calculate checksum16 LSB
        QString lowCheckSum = QString("%1").arg(checkSumSixteenHex.mid(2,2), 0, 16);
        int lowCheckSumDecimal = lowCheckSum.toInt(__null, 16);

        // Set MSB and LSB values into the package at the correct position
        stringPackage.replace(5, QString::number(highCheckSumDecimal));
        stringPackage.replace(4, QString::number(lowCheckSumDecimal));


        // Calcuate check sum eight
        QString checkSumEightValue = checkSumEight(stringPackage);

        // Set checksum8 at the correct position
        stringPackage.replace(0, checkSumEightValue);


        // Convert string package list to a hex string
        QString hex;
        for ( int i = 0; i < stringPackage.size(); i++ )
        {
            hex += QString("%1").arg(stringPackage.at(i).toInt(), 2, 16, QChar('0'));
        }

        // Create QByteArray container
        QByteArray package;

        // Resize to string package size
        package.resize(stringPackage.size());

        // Add the data into array
        package = QByteArray::fromHex(hex.toUtf8());

        // Return the generated package
        return package;
    }


    /**
     * Send a package of data and receive the reply. This method is generate the relivent command + checksum bytes and validate the responce
     *
     * @brief LabJackController::sendReceivePackage
     * @param type
     * @param data
     * @param receivedBytes
     * @return
     */
    QStringList LabJackController::sendReceivePackage(QString type, QStringList data, int receivedBytes)
    {
        // Container for data
        QStringList returnedData;

        // Multiple different types of package structures
        if(type == "feedback")
        {
            // Calcuate the number of returning btyes
            receivedBytes = receivedBytes + 3 + 6;
            if( ( (receivedBytes) %2 ) != 0 )
                receivedBytes++;

            // Create the package to send
            QByteArray package = createPackageFeedback(data);

            // Write the package
            write(package);

            // Read the package
            returnedData = read(receivedBytes);

            // Check the check sum if it is return with empty responce
            if(!validate(type, returnedData))
            {
                // Print debug message
                qDebug() << "There was a problem with the check sum validation for the LabJack. Sent Package:" << package << "; Returned Package: " << returnedData;

                // Empty the contain for incorrect data
                returnedData.clear();

                // Return empty responce
                return returnedData;
            }
        }
        else // Failed to find correct package type
        {
            return returnedData;
        }

        // Tell the applcation about the data
        emit_labJackData(m_responsability, m_method, data);

        // Return the data
        return returnedData;
    }


    /**
     * Validates a package is correct
     *
     * @brief LabJackController::validate
     */
    bool LabJackController::validate(QString type, QStringList package)
    {
        // Containers
        int highCheckSumDecimal;
        int lowCheckSumDecimal;
        QString checksum8;


        // Multiple different types of package structures
        if(type == "feedback")
        {
            // Calculate the check sum 16
            QString checksum16 = checkSumSixteen(package);

            // Find the checksum16 MSB
            QString highCheckSum = QString("%1").arg(checksum16.mid(0,2), 0, 16);
            highCheckSumDecimal = highCheckSum.toInt(__null, 16);

            // Find the checksum16 LSB
            QString lowCheckSum = QString("%1").arg(checksum16.mid(2,2), 0, 16);
            lowCheckSumDecimal = lowCheckSum.toInt(__null, 16);

            // Calculate the check sum 8
            checksum8 = checkSumEight(package);

            // Get the error codes
            if(package.at(7).toInt() != 0)
            {
                // Log the error
                qDebug() << "There was an error in the frame data for the LabJack; Error code received was:" << package.at(6).toInt() << "; For frame:" << package.at(7).toInt() << "; In Package:" << package << "; With type:" << type;

                // Tell the application we failed
                emit_critialLabJackError(errorPackageGenerator("Device1", "Device1", "There was an error in the frame data for the LabJack"));

                // Validation failed
                return false;
            }

            // Compare the check sums
            if( ( package.at(0).toInt() == checksum8.toInt() ) && ( package.at(4).toInt() == lowCheckSumDecimal ) && ( package.at(5).toInt() == highCheckSumDecimal ) )
                return true;
        }

        // There should never by invalid data here as this is a sync lib
        qDebug() << "There was an checksum error from the LabJack; "    << "In Package:" << package
                                                                        << "; With type:" << type
                                                                        << "; CheckSum 8:" << package.at(0).toInt() << " Should be:" <<  checksum8.toInt()
                                                                        << "; CheckSum 16 LSB:" << package.at(4).toInt() << " Should be:" << lowCheckSumDecimal
                                                                        << "; CheckSum 16 MSB:" << package.at(5).toInt() << " Should be:" << highCheckSumDecimal;

        // Tell the application we failed
        emit_critialLabJackError(errorPackageGenerator("Device1", "Device1", "There was an checksum error from the LabJack"));

        // Validation failed
        return false;
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
