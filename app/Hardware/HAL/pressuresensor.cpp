#include "PressureSensor.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>

namespace App { namespace Hardware { namespace HAL
{
    PressureSensor::PressureSensor(QObject *parent, int id)
        :   SerialController(parent),
            m_id(id)
    {
        // Sets what this class is responable for; @NOTE: Could be done in base class
        m_responsability = "PressureSensor";
        m_id = 1;
        m_hexToAcsii = true;
    }


    QByteArray PressureSensor::createPackage(QString action, QString parameterValue, QString data)
    {
        // Below inits the device
        QByteArray package;
        package.resize(4);
        package[0] = 0x01; // ID with a decimal value of 1
        package[1] = 0x30; // Function with a decimal value of 48
        package[2] = 0x34; // Upper CRC with a decimal value of 52
        package[3] = 0x00; // Lower CRC with a decimal value of 0

        // Below reads the serial number
        QByteArray package2;
        package2.resize(4);
        package2[0] = 0x01; // ID with a decimal value of 1
        package2[1] = 0x45; // Function with a decimal value of 69
        package2[2] = 0xD3; // Upper CRC with a decimal value of 211
        package2[3] = 0xC1; // Lower CRC with a decimal value of 193

        write(package);

        qDebug() << package2;

        // Return string and Conver to btye array
        return package2;
    }

    /**
     * Sets the ID for the vac pump
     *
     * @brief VacStation::setId
     * @param id
     */
    void PressureSensor::setId(int id)
    {
        m_id = id;
    }

    /**
     * This method validate the data before procceeding to proccessReadData
     * WARNING: This method may recieve half complete data packages so it
     *          must take that into account when verifing the package
     *
     * @brief PressureSensor::validate
     * @param package
     * @return
     */
    bool PressureSensor::validate(QStringList package)
    {
        return true;
    }

    /**
     * When data has been recived it will be handled here
     * by this time validation has been performed on the data via a check sum
     *
     * @brief PressureSensor::proccessReadData
     * @param readData
     */
    void PressureSensor::proccessReadData(QStringList readData)
    {
        for (int i = 0; i < readData.size(); ++i)
        {
            qDebug() << readData.at(i);
        }
        qDebug() << "Read on port: " << readData;
    }


    /**
     * Test whether the connection is working
     *
     * @brief PressureSensor::testConnection
     */
    void PressureSensor::testConnection()
    {

    }


    /**
     * Reset the current connection
     *
     * @brief PressureSensor::resetConnection
     */
    void PressureSensor::resetConnection()
    {
        readPressure();
    }


    /**
     * Confirm the device is init
     * Relatvie to function 48 in the datasheet
     *
     * @brief PressureSensor::confirmIniti
     */
    void PressureSensor::confirmInit()
    {
        QByteArray package;
        package.resize(4);
        package[0] = 0x01; // ID with a decimal value of 1
        package[1] = 0x30; // Function with a decimal value of 48
        package[2] = 0x34; // Upper CRC with a decimal value of 52
        package[3] = 0x00; // Lower CRC with a decimal value of 0
        write(package);
    }


    /**
     * Reads the current pressure of the sensor
     * Relative to function 73 in the datasheet
     *
     * @brief PressureSensor::readPressure
     */
    void PressureSensor::readPressure()
    {
        int channel = m_command.value("channel").toInt();

        //createPackage("73","1","");
         createPackage("48","","");
    }
}}}






