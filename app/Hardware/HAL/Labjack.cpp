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

        configureIO();

    }


    /**
     * Calcuates the check sum 8 for a given package
     *
     * @brief LabJack::checkSumEight
     * @param package
     * @return
     */
    QString LabJack::checkSumEight(QStringList package)
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
     * Calcuates the check sum 16 for a given package
     *
     * @brief LabJack::checkSumSixteen
     * @param package
     * @return
     */
    QString LabJack::checkSumSixteen(QStringList package)
    {
        int i, a = 0;

        //Sums bytes 6 to n-1 to a unsigned 2 byte value
        // was i=3
        for( i = 6; i < package.size(); i++ )
        {
            a += (unsigned short) package.at(i).toInt();
        }

        return QString::number(a);
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

        // Input and output data size
        int data = 4;
        int out = 0;


        int sendDWSize, recDWSize;
        if( ((sendDWSize = data + 1)%2) != 0 )
                sendDWSize++;

        if( ((recDWSize = out + 3)%2) != 0 )
            recDWSize++;

        qDebug() << sendDWSize << " " << recDWSize;

        QStringList stringPackage; //= QByteArray::fromHex("0b f8 02 00 10 00 00 0b 05 00");
        stringPackage.insert(0, "0"); // check sum 8                 (place holder)
        stringPackage.insert(1, "248"); // command byte              (Always 248)
        stringPackage.insert(2, QString::number(sendDWSize/2)); // Number of data words        (.5 word for echo and 10.5 words for IO Type + data)
        stringPackage.insert(3, "0"); // Extended command number     (Always 0)
        stringPackage.insert(4, "0"); // Checksum 16 LSB             (place holder)
        stringPackage.insert(5, "0"); // Checksum 16 MSB             (place holder)
        stringPackage.insert(6, "0"); // Echo                        (Always 0)

        // Set the pin as digital in
        stringPackage.insert(7, "13"); // IO Type                    (current = 13 = BitDirWrite)
        stringPackage.insert(8, QString::number( (long) 5 + 128) ); // Port Name                   (0-7=FIO, 8-15=EIO, or 16-19=CIO)
        //stringPackage.insert(9, "1"); // Input or output             (OUTPUT = 1 and INPUT =0)

        // Set the pin as digital high
        stringPackage.insert(9, "11"); // IO Type                    (current = 11 = BitStateWrite)
        stringPackage.insert(10, QString::number( (long) 5 + (128*1) ) ); // Port Name                   (0-7=FIO, 8-15=EIO, or 16-19=CIO)
        //stringPackage.insert(12, "0"); // Digital port value          (HIGH = 1 and LOW =0)

        stringPackage.insert(11, "0");

        // Caclates the check sum 16
        QString checkSumSixteenHex = QString("%1").arg(checkSumSixteen(stringPackage).toInt(), 4, 16, QChar('0'));

        // Sets the checksum 16 LSB
        QString highCheckSum = QString("%1").arg(checkSumSixteenHex.mid(0,2), 0, 16);
        int highCheckSumDecimal = highCheckSum.toInt(__null, 16);
        stringPackage.replace(5, QString::number(highCheckSumDecimal));

        // Sets the checksum 16 MSB
        QString lowCheckSum = QString("%1").arg(checkSumSixteenHex.mid(2,2), 0, 16);
        int lowCheckSumDecimal = lowCheckSum.toInt(__null, 16);
        stringPackage.replace(4, QString::number(lowCheckSumDecimal));

        // Calcuate check sum eight
        QString checkSumEightValue = checkSumEight(stringPackage);

        // Calcuate the checksum 8
        stringPackage.replace(0, checkSumEightValue);

        // Convert string list to hex string
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

        // Write package
        write(package);

        // Read the package
        read(recDWSize + 6);
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
