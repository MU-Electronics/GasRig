#include "FlowController.h"

#include <QObject>
#include <QDebug>

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
        return true;
    }


    QString FlowController::calculateCheckSum(QStringList dataIn)
    {

        return QString::number(1);
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
        qDebug() << readData;
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
        m_method = "testConnection";

        qDebug() << "resetting";

        QByteArray package;
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




        package.resize(17);
        package[0] = 0xFF;
        package[1] = 0xFF;
        package[2] = 0x82;
        package[3] = 0x00;
        package[4] = 0x00;
        package[5] = 0x00;
        package[6] = 0x00;
        package[7] = 0x00;
        package[8] = 0xb0;
        package[9] = 0x06;
        package[10] = 0xe3;
        package[11] = 0x5d;
        package[12] = 0xF0;
        package[13] = 0xC7;
        package[14] = 0x0C;
        package[15] = 0x31;
        package[16] = 0x3B;

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




        qDebug() << package;

        write(package);

    }

}}}
