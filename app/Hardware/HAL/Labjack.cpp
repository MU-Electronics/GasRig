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
     *
     *
     * @brief LabJack::configureIO
     */
    void LabJack::configureIO()
    {
        // Which function is being ran?
        m_method = "configureIO";

        //QByteArray setFIO5High = QByteArray::fromHex("0bf802001000000b0500");
        QByteArray setFIO5High; //= QByteArray::fromHex("0b f8 02 00 10 00 00 0b 05 00");
        setFIO5High.insert(0, 0x0b); // check sum 8


        write(setFIO5High);

        read(10);
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
