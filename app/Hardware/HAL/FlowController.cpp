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
        if(package.isEmpty() || package.size() < 5)
            return false;

        // Ensure only two start bytes; could be up to 5
        while(package.at(2).toInt() == 255)
        {
            package.removeFirst();
        }

        // Get the received check sum
        QString recievedChecksum = package.at(package.length() - 1);

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
        for ( int i = 0; i < package.size() - 1; i++ )
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
        emit emit_flowControllerData(m_responsability, m_method, readData);

        //QVariantMap
        qDebug() << "Read on port: " << readData;
    }


    /**
     * Test whether the connection is working
     *
     * @brief FlowController::testConnection
     */
    void FlowController::testConnection()
    {

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

        //getIdentifier();
        getFlowRate();
    }


    /**
     * Gets the identification for a flow controller from the tags
     * NOTE: This does not follow the createPackage method very well so it done on its own
     *
     * @brief FlowController::getIdentifier
     */
    void FlowController::getIdentifier()
    {
        // Set the method
        m_method = "getIdentifier";

        // Create container
        QByteArray package;

        // Make correct size
        package.resize(17);

        // Define bytes
        package[0] = 0xFF;
        package[1] = 0xFF;
        package[2] = 0x82;
        package[3] = 0x00;
        package[4] = 0x00;
        package[5] = 0x00;
        package[6] = 0x00;
        package[7] = 0x00;
        package[8] = 0x0b;
        package[9] = 0x06;
        package[10] = 0xe3;
        package[11] = 0x5d;
        package[12] = 0xF0;
        package[13] = 0xC7;
        package[14] = 0x0C;
        package[15] = 0x32;
        package[16] = 0x38;

        // Write the package
        write(package);
    }


    /**
     * Get the current flow rate for the controller
     *
     * @brief FlowController::getFlowRate
     */
    void FlowController::getFlowRate()
    {
        // Set the method
        m_method = "getFlowRate";

        // No data needs to be sent for this request
        QStringList data;

        // Send the package
        send("FlowControllerOne", "1", data);
    }





    void FlowController::getSetFlowRate()
    {

    }


    void FlowController::getValveOverride()
    {

    }


    void FlowController::setFlowRate()
    {

    }


    void FlowController::setValveOverride()
    {

    }


}}}

