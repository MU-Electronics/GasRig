#include "PressureSensor.h"

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>
#include <QTextStream>

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


    /**
     * PUBLIC: Creates a package and send it to the bus, then wait for a responce and validates the responce.
     *
     * @param QString action The action eg request, command etc (See Pfeffier Vacuum Protocol for "RS485" p.25 chap. 7.2 in TC110 OPerating Instructions)
     * @param QString parameterValue The id for the information / action required
     * @param QString  data The control data to be sent
     * @param unsigned int bytesRead The number of bytes for the returned response
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @return string
     */
    bool PressureSensor::send(QString action, QString parameterValue)
    {
        // Create package to send
        QByteArray package = createPackage(action, parameterValue);

        // Write the package to the bus
        write(package);

        // package write
        return true;
    }


    QByteArray PressureSensor::createPackage(QString action, QString parameterValue)
    {
        // Create package container
        QByteArray package;

        // convert id to hex
        QString id = QString("%1").arg(m_id, 2, 16, QChar('0'));

        // Convert function to hex
        QString actionHex = QString("%1").arg(action.toInt(), 2, 16, QChar('0'));

        // Convert for para Hex
        QString parameterValueHex;

        // Calcuate check sum
        QStringList dataArray;
        dataArray << QString::number(m_id) << action;
        if(!parameterValue.isEmpty())
            dataArray << parameterValue;
        QString checkSum = calculateCheckSum(dataArray);

        // Convert check sum into hex
        QString checkSumHex = QString("%1").arg(checkSum.toInt(), 0, 16);

        // Get high and low bytes
        QString highCheckSum = QString("%1").arg(checkSumHex.mid(0,2), 0, 16);
        QString lowCheckSum = QString("%1").arg(checkSumHex.mid(2,2), 0, 16);

        // Create the package
        if(!parameterValue.isEmpty())
        {
            // Set size of package
            package.resize(5);

            // Get param value in hex form
            parameterValueHex = QString("%1").arg(parameterValue.toInt(), 2, 16, QChar('0'));

            // Create package
            package = QByteArray::fromHex(id.toUtf8() + actionHex.toUtf8() + parameterValueHex.toUtf8() + highCheckSum.toUtf8() + lowCheckSum.toUtf8());
        }
        else
        {
            // Set size of package
            package.resize(4);

            // Create package
            package = QByteArray::fromHex(id.toUtf8() + actionHex.toUtf8() + highCheckSum.toUtf8() + lowCheckSum.toUtf8());
        }

        // Return string and Conver to btye array
        return package;
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
     * Calcuates a check sum 16 modbus
     *
     * @brief PressureSensor::calculateCheckSum
     * @param dataIn
     * @return
     */
    QString PressureSensor::calculateCheckSum(QStringList dataIn)
    {
        uint16_t crc = 0xFFFF;
        bool b;

        for(int pos = 0; pos<dataIn.length();pos++)
        {
            crc ^= (uint16_t) dataIn.at(pos).toInt();
            for( int i = 8; i != 0; i--)
            {
                if(crc%2==1)
                    b= true;
                else
                    b= false;
                crc/= 2;
                if(b)
                    crc^= 0xA001;
            }

        }
        return QString::number(crc);
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
        QString checkSum;

        // Get the hex values from received data
        QString crcHighHex = QString("%1").arg((int)package.at(package.size()-2).toInt(), 0, 16);
        QString crcLowHex = QString("%1").arg((int)package.at(package.size()-1).toInt(), 0, 16);

        // Combined the two hex numbers
        QString combind = "0x" + crcHighHex + crcLowHex;

        // Convert hex value to decimal value
        int intChecksum = 0;
        QTextStream convertHex(&combind);
        convertHex >> intChecksum;

        // Remove check sums from received data
        QStringList dataArray = package;
        dataArray.removeLast();
        dataArray.removeLast();

        // If the differance between the two stirng is zero then check sums match
        if (checkSumValidation(dataArray, QString::number(intChecksum)))
            return true;

        // Validation failed
        return false;
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
//        for (int i = 0; i < readData.size(); ++i)
//        {
//            qDebug() << readData.at(i);
//        }
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
     * Requests confirmation that the device is init
     * Relatvie to function 48 in the datasheet
     *
     * @brief PressureSensor::confirmIniti
     */
    void PressureSensor::confirmInit()
    {
        send("48", "");
    }


    /**
     * Requests the serial number of the sensor
     *
     * @brief PressureSensor::readSerialNumber
     */
    void PressureSensor::readSerialNumber()
    {
        send("69", "");
    }


    /**
     * Reads the current pressure of the sensor
     * Relative to function 73 in the datasheet
     *
     * @brief PressureSensor::readPressure
     */
    void PressureSensor::readPressure()
    {
        send("73", m_command.value("channel").toString());
    }
}}}






