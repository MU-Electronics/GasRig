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



        m_command.insert("FIO7", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("FIO6", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("FIO5", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("FIO4", "1"); // 1 = Analgue Input;    0 = Digital IO

        m_command.insert("EIO7", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO6", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO5", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO4", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO3", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO2", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO1", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO0", "1"); // 1 = Analgue Input;    0 = Digital IO

        configureIO();

        m_command.insert("port", "FIO5");
        m_command.insert("value", "0");
        setDigitalPort();

        m_command.insert("port", "FIO5");
        readPortDirection();

       m_command.insert("port", "DAC0");
       m_command.insert("value", "35535");
       setAnaloguePort();


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

        // Customise the package extended command
        customise.insert("extended_command", "11");

        // Customise bit 6 of the package to use as write mask
        customise.insert("bit_6", "14");

        // Data container
        QStringList stringPackage;

        // Reserver byte
        stringPackage.append(QString::number(0));

        // Timer Config (we will not configure timers in this function, could be expanded later to do this)
        stringPackage.append(QString::number(0));

        // Enable DAC 1?
        QString binaryDacConfig = "001";
        // Convert and append
        int DACConfigValue = binaryDacConfig.toInt(__null, 2);
        stringPackage.append(QString::number(DACConfigValue));

        // FIO Analogue
        QString binaryFIOConfig;
        binaryFIOConfig += m_command.value("FIO7").toString();
        binaryFIOConfig += m_command.value("FIO6").toString();
        binaryFIOConfig += m_command.value("FIO5").toString();
        binaryFIOConfig += m_command.value("FIO4").toString();
        binaryFIOConfig = "1111"; // FIO0, FIO1, FIO2, FIO3 can only be analogue inputs
        // Convert and append
        int FIOConfigValue = binaryFIOConfig.toInt(__null, 2);
        stringPackage.append(QString::number(FIOConfigValue));

        // EIO Analogue
        QString binaryEIOConfig;
        binaryEIOConfig += m_command.value("EIO7").toString();
        binaryEIOConfig += m_command.value("EIO6").toString();
        binaryEIOConfig += m_command.value("EIO5").toString();
        binaryEIOConfig += m_command.value("EIO4").toString();
        binaryEIOConfig += m_command.value("EIO3").toString();
        binaryEIOConfig += m_command.value("EIO2").toString();
        binaryEIOConfig += m_command.value("EIO1").toString();
        binaryEIOConfig += m_command.value("EIO0").toString();
        // Convert and append
        int EIOConfigValue = binaryEIOConfig.toInt(__null, 2);
        stringPackage.append(QString::number(EIOConfigValue));

        qDebug() << sendReceivePackage("config", stringPackage, 12);
     }


    /**
     * Sets a digital port HIGH or LOW
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

        // Send the data
        sendReceivePackage("feedback", stringPackage, 9);
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

        // Port name
        int port = portValueFromName(m_command.value("port").toString());

        // Value
        QString valueHex = QString("%1").arg(m_command.value("value").toInt(), 4, 16, QChar('0'));
        int MSB = valueHex.mid(0,2).toInt();
        int LSB = valueHex.mid(2,4).toInt();

        // Data container
        QStringList stringPackage;

        // Set the pin as digital in
        stringPackage.append("38");                       // IO Type                    (38 = dac16)
        stringPackage.append(QString::number(port));      // Port Name * Value

        // Set MSB
        stringPackage.append(QString::number(LSB));

        //Set LSB
        stringPackage.append(QString::number(MSB));

        // Send the data
        qDebug() << sendReceivePackage("feedback", stringPackage, 9);
    }


    /**
     * Reads the currently set port direction
     * WARNING: This can show it as an input when infact the labjack is outputing!
     *
     * @brief LabJack::readPortDirection
     */
    void LabJack::readPortDirection()
    {
        // Which function is being ran?
        m_method = "readPortDirection";

        // Port name
        int port = portValueFromName(m_command.value("port").toString());

        // Data container
        QStringList stringPackage;

        // Set the pin as digital in
        stringPackage.append("12");                       // IO Type                    (12 = BitDirRead)
        stringPackage.append(QString::number(port));      // Port Name * Value

        // Send the data
        qDebug() << sendReceivePackage("feedback", stringPackage, 10);

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

        // Port name
        int port = portValueFromName(m_command.value("port").toString());

        // Data container
        QStringList stringPackage;

        // Set the pin as digital in
        stringPackage.append("10");                       // IO Type                    (12 = BitDirRead)
        stringPackage.append(QString::number(port));      // Port Name * Value

        // Send the data
        qDebug() << sendReceivePackage("feedback", stringPackage, 10);
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
