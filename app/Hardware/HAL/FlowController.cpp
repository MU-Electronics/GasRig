#include "FlowController.h"

#include <QObject>
#include <QDebug>

namespace App { namespace Hardware { namespace HAL
{
    FlowController::FlowController(QObject *parent)
    {
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

    }

}}}
