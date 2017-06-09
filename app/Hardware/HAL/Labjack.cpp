#include "Labjack.h"

// Include external deps
#include <QDebug>

namespace App { namespace Hardware { namespace HAL
{


    LabJack::LabJack(QObject *parent)
        :   LabJackController(parent)
    {
        // Sets what this class is responable for; @NOTE: Could be done in base class
        m_responsability = "LabJack";
    }


    /**
     * Check if the connection is still valid
     *
     * @brief LabJack::testConnection
     */
    void LabJack::testConnection()
    {
        // Which function is being ran?
        m_method = "testConnection";
    }


    /**
     * Close the current connection and open a new one
     *
     * @brief LabJack::resetConnection
     */
    void LabJack::resetConnection()
    {
        // Which function is being ran?
        m_method = "resetConnection";

//        // Close the current connection
//        close();

//        // Open a new connection
//        open();

        // The above is the actual method code the below is just a handy place for me to test my code

        m_command.insert("port", "FIO5");
        m_command.insert("value", "0");
        setDigitalPort();

    }


    /**
     * Create a type feedback function package to send to the lab jack
     *
     * @brief LabJack::createPackageFeedback
     * @return
     */
    QByteArray LabJack::createPackageFeedback(QStringList data)
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

        // Calculate checksum16 LSB
        QString highCheckSum = QString("%1").arg(checkSumSixteenHex.mid(0,2), 0, 16);
        int highCheckSumDecimal = highCheckSum.toInt(__null, 16);

        // Calculate checksum16 MSB
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


    QStringList LabJack::sendReceivePackage(QString type, QStringList data, int receivedBytes)
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
     * @brief LabJack::validate
     */
    bool LabJack::validate(QString type, QStringList package)
    {
        // Multiple different types of package structures
        if(type == "feedback")
        {


            // Validation successfull
            return true;
        }

        return false;
    }



    /**
     *
     *
     * @brief LabJack::configureIO
     */
    void LabJack::configureIO()
    {
        // Which function is being ran?
        m_method = "configureIO";

     }



    /**
     *
     *
     * @brief LabJack::setPortDirection
     */
    void LabJack::setPortDirection()
    {
        // Which function is being ran?
        m_method = "setPortDirection";

    }


    /**
     *
     *
     * @brief LabJack::setDigitalPort
     */
    void LabJack::setDigitalPort()
    {
        // Which function is being ran?
        m_method = "setDigitalPort";

        // Port name
        int port = portValueFromName(m_command.value("port").toString());

        // Port value
        int value = ((m_command.value("value").toInt() > 0) ? 1 : 0);

        // Port direction   @NOTE for output this will always be one as 0 means input
        int direction = 1;

        // Data container
        QStringList stringPackage;

        // Set the pin as digital in
        stringPackage.append("13");                                                     // IO Type                    (13 = BitDirWrite)
        stringPackage.append(QString::number( (long) port + (128 * direction) ) );      // Port Name * Value

        // Set the pin as digital high
        stringPackage.append("11");                                                     // IO Type                    (11 = BitStateWrite)
        stringPackage.append(QString::number( (long) port + (128 * value) ) );          // Port Name * Value

        qDebug() << "Reply was:" << sendReceivePackage("feedback", stringPackage, 0);
    }


    /**
     *
     *
     * @brief LabJack::setAnaloguePort
     */
    void LabJack::setAnaloguePort()
    {
        // Which function is being ran?
        m_method = "setAnaloguePort";

    }


    /**
     *
     *
     * @brief LabJack::readPortDirection
     */
    void LabJack::readPortDirection()
    {
        // Which function is being ran?
        m_method = "readPortDirection";

    }


    /**
     *
     *
     * @brief LabJack::readDigitalPort
     */
    void LabJack::readDigitalPort()
    {
        // Which function is being ran?
        m_method = "readDigitalPort";

    }


    /**
     *
     *
     * @brief LabJack::readAnaloguePort
     */
    void LabJack::readAnaloguePort()
    {
        // Which function is being ran?
        m_method = "readAnaloguePort";

    }

}}}
