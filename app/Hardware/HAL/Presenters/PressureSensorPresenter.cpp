#include "PressureSensorPresenter.h"

#include <QStringList>
#include <QVariantMap>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    PressureSensorPresenter::PressureSensorPresenter(QObject *parent)
    {

    }


    /**
     * Determin which presenter should be ran
     * NOTE: All packages are validated before this point
     *
     * @brief PressureSensorPresenter::proccess
     * @param method
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PressureSensorPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "confirmInit")
        {
            return confirmInit(commands, package);
        }
        else if (method == "readSerialNumber")
        {
            return readSerialNumber(commands, package);
        }
        else if (method == "readPressure")
        {
            return readPressure(commands, package);
        }
    }


    /**
     * Convert the confirmInit raw package to actual data
     *
     * @brief PressureSensorPresenter::confirmInit
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PressureSensorPresenter::confirmInit(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_pressureSensorInit";

        // Sucessfully inited
        presented["status"] = "true";

        // Return the correctly formatted data
        return presented;
    }


    /**
     * Convert the readSerialNumber raw package to actual data
     *
     * @brief PressureSensorPresenter::confirmInit
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PressureSensorPresenter::readSerialNumber(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_pressureSensorSerialNumber";


        presented["value"] = (float) 1.2025;

        return presented;
    }


    /**
     * Convert the readPressure raw package to actual data
     *
     * @brief PressureSensorPresenter::confirmInit
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PressureSensorPresenter::readPressure(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_pressureSensorPressure";


        presented["value"] = (float) 1.2025;

        return presented;
    }

}}}}
