#include "PressureSensor.h"

#include <QDebug>

namespace App { namespace Hardware { namespace HAL
{
    PressureSensor::PressureSensor(QObject *parent)
    {
        // Sets what this class is responable for; @NOTE: Could be done in base class
        m_responsability = "PressureSensor";
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
}}}
