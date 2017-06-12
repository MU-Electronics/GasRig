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


        qDebug() << "Config IO";
        m_command.insert("FIO7", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("FIO6", "1"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("FIO5", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("FIO4", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO7", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO6", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO5", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO4", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO3", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO2", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO1", "0"); // 1 = Analgue Input;    0 = Digital IO
        m_command.insert("EIO0", "0"); // 1 = Analgue Input;    0 = Digital IO
        configureIO();

        m_command.clear();
        qDebug() << "";qDebug() << "";

        qDebug() << "Setting FIO7 as input";
        m_command.insert("port", "FIO7");
        m_command.insert("value", "-1");
        setDigitalPort();

        m_command.clear();
        qDebug() << "";qDebug() << "";

        qDebug() << "Setting FIO5 LOW";
        m_command.insert("port", "FIO5");
        m_command.insert("value", "0");
        setDigitalPort();

        m_command.clear();
        qDebug() << "";qDebug() << "";

        qDebug() << "Reading FIO7";
        m_command.insert("port", "FIO7");
        readDigitalPort();

        m_command.clear();
        qDebug() << "";qDebug() << "";


        qDebug() << "Setting FIO5 HIGH";
        m_command.insert("port", "FIO5");
        m_command.insert("value", "1");
        setDigitalPort();

        m_command.clear();
        qDebug() << "";qDebug() << "";

        qDebug() << "Reading FIO7";
        m_command.insert("port", "FIO7");
        readDigitalPort();

        m_command.clear();
        qDebug() << "";qDebug() << "";

        qDebug() << "Writing a analogue voltage";
        m_command.insert("port", "DAC1");
        m_command.insert("value", "20020"); //65535
        setAnaloguePort();

        m_command.clear();
        qDebug() << "";qDebug() << "";

        qDebug() << "Reading a analogue value";
       m_command.insert("port", "FIO6");
       readAnaloguePort();


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
        customise.insert("bit_6", "12"); //=   0   0    1   1    0    0  =   Write UART Related settings      Reserved, Pass 0        EIOAnalog      FIOAnalog      DAC1Enable      TimerCounterConfig

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
        binaryFIOConfig.append(m_command.value("FIO7").toString());
        binaryFIOConfig.append(m_command.value("FIO6").toString());
        binaryFIOConfig.append(m_command.value("FIO5").toString());
        binaryFIOConfig.append(m_command.value("FIO4").toString());
        binaryFIOConfig.append("1111"); // FIO0, FIO1, FIO2, FIO3 can only be analogue inputs
        // Convert and append
        int FIOConfigValue = binaryFIOConfig.toInt(__null, 2);
        stringPackage.append(QString::number(FIOConfigValue));


        // EIO Analogue
        QString binaryEIOConfig;
        binaryEIOConfig.append(m_command.value("EIO7").toString());
        binaryEIOConfig.append(m_command.value("EIO6").toString());
        binaryEIOConfig.append(m_command.value("EIO5").toString());
        binaryEIOConfig.append(m_command.value("EIO4").toString());
        binaryEIOConfig.append(m_command.value("EIO3").toString());
        binaryEIOConfig.append(m_command.value("EIO2").toString());
        binaryEIOConfig.append(m_command.value("EIO1").toString());
        binaryEIOConfig.append(m_command.value("EIO0").toString());
        // Convert and append
        int EIOConfigValue = binaryEIOConfig.toInt(__null, 2);
        stringPackage.append(QString::number(EIOConfigValue));

        qDebug() << "Config IO" << sendReceivePackage("config", stringPackage, 12);
     }


    /**
     * Sets a digital port HIGH or LOW or as an input
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
        if(m_command.value("direction").toInt() == -1 )
            direction = 0;

        // Data container
        QStringList stringPackage;

        // Set the pin as digital in / out
        stringPackage.append("13");                                                     // IO Type                    (13 = BitDirWrite)
        stringPackage.append(QString::number( (long) port + (128 * direction) ) );      // Port Name * Value

        // If digital port is ment to be output set the level, low or high
        if(m_command.value("value").toInt() != -1)
        {
            stringPackage.append("11");                                                     // IO Type                    (11 = BitStateWrite)
            stringPackage.append(QString::number( (long) port + (128 * value) ) );          // Port Name * Value
        }


        // Send the data
        qDebug() << "Setting digital IO " << sendReceivePackage("feedback", stringPackage, 9);
    }


    /**
     * Sets a analogue value for DAC0 or DAC1
     *
     * @brief LabJack::setAnaloguePort
     */
    void LabJack::setAnaloguePort()
    {
        // Which function is being ran?
        m_method = "setAnaloguePort";

        // Port name
        int port = portValueFromName(m_command.value("port").toString());

        // Get the MSB and LSB of the value wanted
        int value = m_command.value("value").toInt();
        int LSB = value & 255;
        int MSB = (value & 65280) / 256;

        // Data container
        QStringList stringPackage;

        // Set the pin as digital in
        stringPackage.append(QString::number(38 + port));                       // IO Type                    (38 = dac16)

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
     * Reads whether a digital port is logic high or low
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
        stringPackage.append("13");
        stringPackage.append(QString::number(port));

        // Set the pin as digital in
        stringPackage.append("10");                       // IO Type                    (12 = BitDirRead)
        stringPackage.append(QString::number(port));      // Port Name * Value

        // Send the data
        qDebug() << "Reading digital port" << sendReceivePackage("feedback", stringPackage, 10);
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

        // Port name
        int port = portValueFromName(m_command.value("port").toString());

        // Allow long settings
        int settling = 0;
        if(m_command.value("settling").toInt() == 1 )
            settling = 1;

        // Allow quick sampling
        int quick = 0;
        if(m_command.value("quick").toInt() == 1 )
            quick = 1;

        // Set negative or signle ended
        int negative = 31; // Single ended
        if(!m_command.value("negative").isNull())
            negative = m_command.value("negative").toInt();

        // Data container
        QStringList stringPackage;

        // Set IOType
        stringPackage.append("1");

        // Set the ADC settlings
        int ADCSettings = port + (settling*64) + (quick*128);
        stringPackage.append(QString::number(ADCSettings));

        // Allow select the negative channe;
        stringPackage.append(QString::number(negative));

        // Send the data
        QStringList data = sendReceivePackage("feedback", stringPackage, 11);
        qDebug() << "Reading analogue port: " << data << " 9:" << data.at(9).toInt() << " 10:" << data.at(10).toInt();

        // get voltage bits
        double voltage = data.at(9).toInt() + (data.at(10).toInt()*256);

        // Un calibrated voltage
        double voltageUncal = ( voltage / 65536) * 2.44;

        // Find cal voltage
        double slope = 	3.7231E-05;
        double offset = 0.0000E+00;
        double voltageCal = ( slope * voltage ) + offset;

        // Compare the voltages
        qDebug() << voltage << "; rought voltage: " << voltageUncal << " Accurate voltage: " << voltageCal;

    }

}}}
