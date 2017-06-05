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
        m_responsability = "FlowController";
        //m_id = 1;
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
        QMap<QString, QString> controller;
        controller.insert("manufacture", manuf);
        controller.insert("type", type);
        controller.insert("id_1", ID1);
        controller.insert("id_2", ID2);
        controller.insert("id_3", ID3);

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


    QByteArray FlowController::createPackage(QString selectedController, QString command, QStringList data)
    {

    }


    void FlowController::getFlowRate()
    {

        // Set the method
        m_method = "getFlowRate";

        QByteArray package;

        package.resize(11);

        package[0] = 0xFF;
        package[1] = 0xFF;
        package[2] = 0x82;
        package[3] = 0x8A;
        package[4] = 0x5A;
        package[5] = 0x1B;
        package[6] = 0xB1;
        package[7] = 0x89;
        package[8] = 0x01; // COMMAND
        package[9] = 0x00; // DATA
        package[10] = 0x70; // CHECKSUM

        write(package);
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

