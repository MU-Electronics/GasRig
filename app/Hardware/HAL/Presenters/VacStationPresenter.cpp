#include "VacStationPresenter.h"

#include <QStringList>
#include <QVariantMap>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    VacStationPresenter::VacStationPresenter(QObject *parent)
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
    QVariantMap VacStationPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "getTemperature")
        {
            return getTemperature(commands, package);
        }
        else if (method == "getTurboSpeed")
        {
            return getTurboSpeed(commands, package);
        }
        else if (method == "getError")
        {
            return getError(commands, package);
        }
        else if (method == "getGasMode")
        {
            return getGasMode(commands, package);
        }
        else if (method == "getBackingPumpMode")
        {
            return getBackingPumpMode(commands, package);
        }
        else if (method == "getTurboPumpState")
        {
            return getTurboPumpState(commands, package);
        }
        else if (method == "getPumpingState")
        {
            return getPumpingState(commands, package);
        }
        else if (method == "setGasMode")
        {
            return setGasMode(commands, package);
        }
        else if (method == "setBackingPumpMode")
        {
            return setBackingPumpMode(commands, package);
        }
        else if (method == "setTurboPumpState")
        {
            return setTurboPumpState(commands, package);
        }
        else if (method == "setPumpingState")
        {
            return setPumpingState(commands, package);
        }
        QVariantMap error;
        return error;
    }


    /**
     * Convert the getTemperature raw package to actual data
     *
     * @brief VacStationPresenter::getTemperature
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::getTemperature(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getTemperature";


        // Return the presenter data
        return presented;
    }


    /**
     * Convert the getTurboSpeed raw package to actual data
     *
     * @brief VacStationPresenter::getTurboSpeed
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::getTurboSpeed(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getTurboSpeed";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the getError raw package to actual data
     *
     * @brief VacStationPresenter::getError
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::getError(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getError";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the getGasMode raw package to actual data
     *
     * @brief VacStationPresenter::getGasMode
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::getGasMode(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getGasMode";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the getBackingPumpMode raw package to actual data
     *
     * @brief VacStationPresenter::getBackingPumpMode
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::getBackingPumpMode(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getBackingPumpMode";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the getTurboPumpState raw package to actual data
     *
     * @brief VacStationPresenter::getTurboPumpState
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::getTurboPumpState(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getTurboPumpState";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the getPumpingState raw package to actual data
     *
     * @brief VacStationPresenter::getPumpingState
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::getPumpingState(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getPumpingState";


        // Return the presenter data
        return presented;
    }


    /**
     * Convert the setGasMode raw package to actual data
     *
     * @brief VacStationPresenter::setGasMode
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::setGasMode(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setGasMode";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the setBackingPumpMode raw package to actual data
     *
     * @brief VacStationPresenter::setBackingPumpMode
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::setBackingPumpMode(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setBackingPumpMode";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the setTurboPumpState raw package to actual data
     *
     * @brief VacStationPresenter::setTurboPumpState
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::setTurboPumpState(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setTurboPumpState";


        // Return the presenter data
        return presented;
    }



    /**
     * Convert the setPumpingState raw package to actual data
     *
     * @brief VacStationPresenter::setPumpingState
     * @param commands
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::setPumpingState(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setPumpingState";


        // Return the presenter data
        return presented;
    }


}}}}
