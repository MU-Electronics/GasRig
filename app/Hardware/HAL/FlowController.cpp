#include "FlowController.h"

#include <QObject>
#include <QDebug>
#include <QString>

namespace App { namespace Hardware { namespace HAL
{
    FlowController::FlowController(QObject *parent)
        :   SerialController(parent)
    {
        m_responsability = "FlowControllerOne";
        m_id = 1;
        m_hexToAcsii = true;
    }


    /**
     * Sets the ID for the vac pump
     *
     * @brief VacStation::setId
     * @param id
     */
    void FlowController::setId(int id)
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

        QByteArray package;

        package.resize(17);

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

        qDebug() << package;

        write(package);
    }


    void FlowController::getFlowRate()
    {

        /*
         * Manufcaturer: 138  =  0x8A
         * Device Type:  90   =  0x5A
         *
         * Device ID 1: 27    =  0x1B
         * Device ID 2: 177   =  0xB1
         * Device ID 3: 138   =  0x89
         *
         */


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

        qDebug() << package;

        write(package);

        // Convert flow rate received into float value
        FourByteToFloat.buf[0] = 60;
        FourByteToFloat.buf[1] = 179;
        FourByteToFloat.buf[2] = 185;
        FourByteToFloat.buf[3] = 51;

        // Print float
        qDebug() << FourByteToFloat.number;
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




// Delete later
//        package.resize(15);
//        package[0] = 0xFF;
//        package[1] = 0xFF;
//        package[2] = 0xFF;
//        package[3] = 0xFF;
//        package[4] = 0xFF;
//        package[5] = 0x82;
//        package[6] = 0x8A;

//        package[7] = 0x96;

//        package[8] = 0x5A;
//        package[9] = 0x1B;
//        package[10] = 0xB1;
//        package[11] = 0x8A;
//        package[12] = 0x01;
//        package[13] = 0x00;
//        package[14] = 0xD0;

//                package.resize(17);
//                package[0] = "255";
//                package[1] = "255";
//                package[2] = "130";
//                package[3] = "0";
//                package[4] = "0";
//                package[5] = "0";
//                package[6] = "0";
//                package[7] = "0";
//                package[8] = "176";
//                package[9] = "6";
//                package[10] = "227";
//                package[11] = "93";
//                package[12] = "240";
//                package[13] = "199";
//                package[14] = "12";
//                package[15] = "49";
//                package[16] = "59";
