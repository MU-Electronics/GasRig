#include "PressureSensor.h"

#include <QDebug>

namespace App { namespace Hardware { namespace HAL
{
    PressureSensor::PressureSensor(QObject *parent)
    {
        // Sets what this class is responable for; @NOTE: Could be done in base class
        m_responsability = "PressureSensor";
        m_id = 1;
    }


    QByteArray PressureSensor::createPackage(QString action, QString parameterValue, QString data)
    {
        // Compile string
        //QString package = QString::number(m_id) + action + parameterValue + data;
        bool ok;
        QByteArray package;
        package.append("1");
        package.append("48");

        // Get crc
        QString crc = calculateCheckSumSixteen(package);

        unsigned int low = crc.toUInt(&ok, 16) & 0xff;
        unsigned int high= (crc.toUInt(&ok, 16)  >> 8) & 0xff;

        // Append the check sum  and carriage return
        //package = package + QString::number(high) + QString::number(low) + "\r";
        package.append("52");
        package.append("0");

        //write(QByteArray::fromStdString(package.toStdString()));
        write(package);

        qDebug() << package;

        // Return string and Conver to btye array
        return QByteArray::fromStdString(package.toStdString());
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
    bool PressureSensor::validate(QString package)
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
    void PressureSensor::proccessReadData(QString readData)
    {


        qDebug() << readData;
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

    }


    /**
     * Confirm the device is init
     * Relatvie to function 48 in the datasheet
     *
     * @brief PressureSensor::confirmIniti
     */
    void PressureSensor::confirmInit()
    {

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






