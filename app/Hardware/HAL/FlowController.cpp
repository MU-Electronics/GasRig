#include "FlowController.h"

#include <QObject>
#include <QDebug>
#include <QString>
#include <QMap>

namespace App { namespace Hardware { namespace HAL
{
    FlowController::FlowController(QObject *parent)
        :   SerialController(parent)
    {
        // What is this class bus responsable for?
        m_responsability = "FlowController";

        // Convert incomming btyes from HEX to ASCII?
        m_hexToAcsii = true;
    }

    /**
     * Register multiple brooks s-protocal controllers on the bus
     *
     * @brief FlowController::registerController
     * @param referance
     * @param manuf
     * @param type
     * @param ID1
     * @param ID2
     * @param ID3
     */
    void FlowController::registerController(QString referance, QString manuf, QString type, QString ID1, QString ID2, QString ID3)
    {
        // Create temp controller container
        QMap<QString, QString> controller;

        // Save information on flow controller to container
        controller.insert("manufacture", manuf);
        controller.insert("type", type);
        controller.insert("id_1", ID1);
        controller.insert("id_2", ID2);
        controller.insert("id_3", ID3);

        // Add the new controller to the list of bus controller
        m_controllers.insert(referance, controller);
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
    bool FlowController::validate(QStringList package)
    {
        // Ensure package is big enough to be processed
        if(package.isEmpty() || package.size() < 6)
            return false;

        // Ensure only two start bytes; could be up to 5
        while(package.at(2).toInt() == 255 && package.size() > 3)
        {
            package.removeFirst();
        }

        // Ensure package is big enough to be processed
        if(package.isEmpty() || package.size() < 3)
            return false;

        // Get the received check sum
        QString recievedChecksum = package.at(package.length() - 1);

        // Remove check sum from package
        package.removeLast();

        // Check if the two check sums match
        if (checkSumValidation(package, recievedChecksum))
            return true;

        // Validation failed
        return false;
    }


    /**
     * Calculates the check sum in exclusive-or format
     *
     * @brief FlowController::calculateCheckSum
     * @param dataIn
     * @return
     */
    QString FlowController::calculateCheckSum(QStringList package)
    {
        // Checksum container
        unsigned int crc = 0;

        // For each byte
        for ( int i = 0; i < package.size(); i++ )
        {
            // Exlusive-or
            crc = crc ^ package.at(i).toInt();
        }

        // Return the value
        return QString::number(crc);
    }


    /**
     * Creates package for sending to the flow controller
     *
     * @brief FlowController::createPackage
     * @param selectedController
     * @param command
     * @param data
     * @return
     */
    QByteArray FlowController::createPackage(QString selectedController, QString command, QStringList data)
    {
        // Create package container
        QStringList stringPackage;

        // Set the start bits
        stringPackage.append("255");
        stringPackage.append("255");

        // Set delimerter
        stringPackage.append("130");

        // Set the controller ID
        stringPackage.append(m_controllers.value(selectedController).value("manufacture"));
        stringPackage.append(m_controllers.value(selectedController).value("type"));
        stringPackage.append(m_controllers.value(selectedController).value("id_1"));
        stringPackage.append(m_controllers.value(selectedController).value("id_2"));
        stringPackage.append(m_controllers.value(selectedController).value("id_3"));

        // Set command
        stringPackage.append(command);

        // Set data info
        if(!data.isEmpty())
        {
            // Set data length
            stringPackage.append(QString::number(data.size()));

            // Set data
            for ( int i = 0; i < data.size(); i++ )
            {
                stringPackage.append(data.at(i));
            }
        }
        else
        {
            // Set no data
            stringPackage.append("0");
        }

        // Set check sum
        stringPackage.append(calculateCheckSum(stringPackage));

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

        // Return the package
        return package;
    }


    /**
     * Create the relivent package and sends it to the device
     *
     * @brief FlowController::send
     * @param selectedController
     * @param command
     * @param data
     */
    bool FlowController::send(QString selectedController, QString command, QStringList data)
    {
        // Create package to send
        QByteArray package = createPackage(selectedController, command, data);

        // Write the package to the bus
        write(package);

        // package write
        return true;
    }


    /**
     * When data has been recived it will be handled here
     * by this time validation has been performed on the data via a check sum
     *
     * @brief FlowController::proccessReadData
     * @param readData
     */
    void FlowController::proccessReadData(QStringList readData)
    {
        // Send the data to the handware access manager
        emit emit_flowControllerData(m_responsability, m_method, m_command, readData);
    }


    /**
     * Init flow controller on connect
     *
     * @brief FlowControllers::connectInitControllers
     */
    void FlowController::connectInitControllers()
    {

    }


    /**
     * Test whether the connection is working
     *
     * @brief FlowController::testConnection
     */
    void FlowController::testConnection()
    {
        // Set the method
        m_method = "testConnection";

        // Does the com port & connection exist?
        if(!checkDeviceAvaliable(false))
        {
            // Send a critial issue, MOST PROBABLY USB not connected issue
            //emit emit_critialSerialError(errorPackageGenerator(m_connectionValues.value("com").toString(), m_connectionValues.value("com").toString(), "Cant not find the device on this PC, check cable is plugged in."));
            emit emit_comConnectionStatus(comConnectionPackageGenerator(m_connectionValues.value("com").toString(), false));

            // return
            return;
        }

        // Test sending data works, for now just get the controllers temperature for the first controller
        m_command.insert("controller", m_controllers.firstKey());
        getControllerTemperature();
    }


    /**
     * Reset the current connection
     *
     * @brief FlowController::resetConnection
     */
    void FlowController::resetConnection()
    {
        // Set the method
        m_method = "resetConnection";

        // Refresh connection attempt
        if(!checkDeviceAvaliable(true))
        {
            // Send a critial issue, MOST PROBABLY USB not connected issue
            emit emit_comConnectionStatus(comConnectionPackageGenerator(m_connectionValues.value("com").toString(), false));

            return;
        }

        // Test sending data works, for now just get the controllers temperature for the first controller
        m_command.insert("controller", m_controllers.firstKey());
        getControllerTemperature();
    }


    /**
     * Gets the identification for a flow controller from the tags
     * NOTE: This does not follow the createPackage method very well so it done on its own
     *
     * @brief FlowController::getIdentifier
     * @param tag_1 The 1st tag for the device
     * @param tag_2 The 2nd tag for the device
     * @param tag_3 The 3rd tag for the device
     * @param tag_4 The 4th tag for the device
     * @param tag_5 The 5th tag for the device
     * @param tag_6 The 6th tag for the device
     */
    void FlowController::getIdentifier()
    {
        // Set the method
        m_method = "getIdentifier";

        // Create package container
        QStringList stringPackage;
        stringPackage.append("255");
        stringPackage.append("255");
        stringPackage.append("130");
        stringPackage.append("0");
        stringPackage.append("0");
        stringPackage.append("0");
        stringPackage.append("0");
        stringPackage.append("0");
        stringPackage.append("11");
        stringPackage.append("6");
        stringPackage.append(m_command.value("tag_1").toString()); // Tag name byte 1    For example: 0xe3
        stringPackage.append(m_command.value("tag_2").toString()); // Tag name byte 2    For example: 0x5d
        stringPackage.append(m_command.value("tag_3").toString()); // Tag name byte 3    For example: 0xF0
        stringPackage.append(m_command.value("tag_4").toString()); // Tag name byte 4    For example: 0xC7
        stringPackage.append(m_command.value("tag_5").toString()); // Tag name byte 5    For example: 0x0C
        stringPackage.append(m_command.value("tag_6").toString()); // Tag name byte 6    For example: 0x32

        // Append the check sum
        stringPackage.append(calculateCheckSum(stringPackage));

        // Convert string list to hex string
        QString hex;
        for ( int i = 0; i < stringPackage.size(); i++ )
        {
            hex += QString("%1").arg(stringPackage.at(i).toInt(), 2, 16, QChar('0'));
        }

        // Create container
        QByteArray package;

        // Make correct size
        package.resize(stringPackage.size());

        // Insert hex string package into QByteArray
        package = QByteArray::fromHex(hex.toUtf8());

        // Write the data
        write(package);
    }


    /**
     * Get the current flow rate for the controller
     *
     * @brief FlowController::getFlowRate
     * @param controller the referance to the registered controller
     */
    void FlowController::getFlowRate()
    {
        // Set the method
        m_method = "getFlowRate";

        // No data needs to be sent for this request
        QStringList data;

        // Send the package
        send(m_command.value("controller").toString(), "1", data);
    }


    /**
     * Gets the set flow rate for the controller
     *
     * @brief FlowController::getSetFlowRate
     * @param controller the referance to the registered controller
     */
    void FlowController::getSetFlowRate()
    {
        // Set the method
        m_method = "getSetFlowRate";

        // No data needs to be sent for this request
        QStringList data;

        // Send the package
        //send(m_command.value("controller").toString(), "215", data);
        send(m_command.value("controller").toString(), "235", data);
    }


    /**
     * Gets the state of the valve for a flow controller
     *
     * @brief FlowController::getValveOverride
     * @param controller the referance to the registered controller
     */
    void FlowController::getValveOverride()
    {
        // Set the method
        m_method = "getValveOverride";

        // No data needs to be sent for this request
        QStringList data;

        // Send the package
        send(m_command.value("controller").toString(), "230", data);
    }


    /**
     * Gets the temperature of the flow controller
     *
     * @brief FlowController::getValveOverride
     * @param controller the referance to the registered controller
     */
    void FlowController::getControllerTemperature()
    {
        // Set the method
        m_method = "getControllerTemperature";

        // No data needs to be sent for this request
        QStringList data;

        // Send the package
        send(m_command.value("controller").toString(), "3", data);
    }


    /**
     * Sets the source of controll for the flow controller
     *
     * @brief FlowController::setSourceControll
     * @param controller the referance to the registered controller
     * @param source the source code
     *              1-2 = Analog Input
     *                3 = Digital
     */
    void FlowController::setSourceControll()
    {
        // Set the method
        m_method = "setSourceControll";

        // No data needs to be sent for this request
        QStringList data;
        data.append(m_command.value("source").toString());

        // Send the package
        send(m_command.value("controller").toString(), "216", data);
    }


    /**
     * Sets the flow rate for a flow controller
     *
     * @brief FlowController::setFlowRate
     * @param controller the referance to the registered controller
     * @param unit the unit for the value being set
     *              57 = percentage
     *             250 = No Used E.g relative to the set flow rate unit on the controller
     * @param rate The rate at which to set the flow rate relative to the unit
     */
    void FlowController::setFlowRate()
    {
        // Set the method
        m_method = "setFlowRate";

        // Convert float to bytes
        float flowRate = m_command.value("rate").toFloat();
        FourByteFloatConvertion.number = flowRate;

        // Container for data bytes
        QStringList data;

        // Set the units for the flow rate
        data.append(m_command.value("unit").toString());

        // Set the flow rate bytes
        data.append(QString::number(FourByteFloatConvertion.buf[3])); // MSB
        data.append(QString::number(FourByteFloatConvertion.buf[2]));
        data.append(QString::number(FourByteFloatConvertion.buf[1]));
        data.append(QString::number(FourByteFloatConvertion.buf[0])); // LSB

        // Send the package
        send(m_command.value("controller").toString(), "236", data);

        // CONVERT BTYES TO FLOAT EXAMPLE
//        FourByteFloatConvertion.buf[0] = 225; // LSB
//        FourByteFloatConvertion.buf[1] = 225;
//        FourByteFloatConvertion.buf[2] = 199;
//        FourByteFloatConvertion.buf[3] = 66; // MSB

//        qDebug() << QString::number(FourByteFloatConvertion.number);
    }


    /**
     * Sets an override for the valve
     *
     * @brief FlowController::setValveOverride
     * @param controller the referance to the registered controller
     * @param state the state for the valve override
     *              0 = override off (default)
     *              1 = override open
     *              2 = override closed
     */
    void FlowController::setValveOverride()
    {
        // Set the method
        m_method = "setValveOverride";

        // No data needs to be sent for this request
        QStringList data;
        data.append(m_command.value("state").toString());

        // Send the package
        send(m_command.value("controller").toString(), "231", data);
    }


    /**
     * Defines whether there is a soft start
     * NOTE: 219 write time for ramp in seconds
     *
     * @brief FlowController::setSoftStart
     * @param controller the referance to the registered controller
     * @param type code for the wanted option
     *                  1 = off
     *                  4 = linear up and down
     */
    void FlowController::setSoftStart()
    {
        // Set the method
        m_method = "setSoftStart";

        // No data needs to be sent for this request
        QStringList data;
        data.append(m_command.value("type").toString());

        // Send the package
        send(m_command.value("controller").toString(), "218", data);
    }


    /**
     * Set the amount of time for the soft start ramp
     *
     * @brief FlowController::setSoftStartTime
     * @param controller the referance to the registered controller
     * @param seconds detmins the graident of the soft start ramp
     */
    void FlowController::setSoftStartTime()
    {
        // Set the method
        m_method = "setSoftStartTime";

        // Data container
        QStringList data;

        // Convert float to bytes
        float seconds = m_command.value("seconds").toFloat();
        FourByteFloatConvertion.number = seconds;

        // Set the flow rate bytes
        data.append(QString::number(FourByteFloatConvertion.buf[3])); // MSB
        data.append(QString::number(FourByteFloatConvertion.buf[2]));
        data.append(QString::number(FourByteFloatConvertion.buf[1]));
        data.append(QString::number(FourByteFloatConvertion.buf[0])); // LSB

        // Send the package
        send(m_command.value("controller").toString(), "219", data);
    }


    /**
     * Defines the flow unit that will be used
     *
     * @brief FlowController::setSoftStart
     * @param controller the referance to the registered controller
     * @param reference flow voloumn units are reported at a specific referance condition
     *                  0 = Normal (273.15 Kelvin/1013.33 mBar)
     *                  1 = Standard (User defined through seperate command)
     *                  2 = Calibration (As defined at calibration)
     * @param unit The flow unit selected
     *              17 = Litres/minute
     *              19 = Cubic meters/hour
     *              24 = Litres/second
     *              28 = Cubic meters/second
     *              57 = Percent of flow range
     *             131 = Cubic meters/minute
     *             138 = Liters/hour
     *             170 = Millilitres/second
     *             171 = Millilitres/minute
     *             172 = Millilitres/hour
     */
    void FlowController::setFlowUnit()
    {
        // Set the method
        m_method = "setFlowUnit";

        // No data needs to be sent for this request
        QStringList data;
        data.append(m_command.value("reference").toString());
        data.append(m_command.value("unit").toString());

        // Send the package
        send(m_command.value("controller").toString(), "196", data);
    }


    /**
     * Defines whether there is a soft start
     *
     * @brief FlowController::setSoftStart
     * @param controller the referance to the registered controller
     * @param unit The temperature unit that will be used
     *              32 = Degrees Celsius
     *              33 = Degrees Fahrenheit
     *              35 = Kelvin
     */
    void FlowController::setTemperatureUnit()
    {
        // Set the method
        m_method = "setTemperatureUnit";

        // No data needs to be sent for this request
        QStringList data;
        data.append(m_command.value("unit").toString());

        // Send the package
        send(m_command.value("controller").toString(), "197", data);
    }

}}}

