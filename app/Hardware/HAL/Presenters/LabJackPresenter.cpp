#include "LabJackPresenter.h"

#include <QStringList>
#include <QVariantMap>
#include <QDebug>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    LabJackPresenter::LabJackPresenter(QObject *parent)
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
    QVariantMap LabJackPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "configureIO")
        {
            return configureIO(commands, package);
        }
        else if (method == "setDigitalPort")
        {
            return setDigitalPort(commands, package);
        }
        else if (method == "setAnaloguePort")
        {
            return setAnaloguePort(commands, package);
        }
        else if (method == "readPortDirection")
        {
            return readPortDirection(commands, package);
        }
        else if (method == "readDigitalPort")
        {
            return readDigitalPort(commands, package);
        }
        else if (method == "readAnaloguePort")
        {
            return readAnaloguePort(commands, package);
        }

        QVariantMap error;
        error["error"] = "NoMethod";
        return error;
    }


    /**
     * Convert the setDigitalPort raw package to actual data
     *
     * @brief LabJackPresenter::setDigitalPort
     * @return
     */
    QVariantMap LabJackPresenter::configureIO(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_configureIO";

        qDebug() << package;

        // Return the presenter data
        return presented;
    }


    /**
     * Convert the setDigitalPort raw package to actual data
     *
     * @brief LabJackPresenter::setDigitalPort
     * @return
     */
    QVariantMap LabJackPresenter::setDigitalPort(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setDigitalPort";

         presented["port"] = commands["port"];
         presented["value"] = commands["value"];

         // Return the presenter data
        return presented;
    }


    /**
     * Convert the setAnaloguePort raw package to actual data
     *
     * @brief LabJackPresenter::setAnaloguePort
     * @return
     */
    QVariantMap LabJackPresenter::setAnaloguePort(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setAnaloguePort";

        qDebug() << package;

        // Return the presenter data
        return presented;
    }


    /**
     * Convert the readPortDirection raw package to actual data
     *
     * @brief LabJackPresenter::readPortDirection
     * @return
     */
    QVariantMap LabJackPresenter::readPortDirection(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_readPortDirection";

        // Return the presenter data
        return presented;
    }


    /**
     * Convert the readDigitalPort raw package to actual data
     *
     * @brief LabJackPresenter::readDigitalPort
     * @return
     */
    QVariantMap LabJackPresenter::readDigitalPort(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_readDigitalPort";

        // Return the presenter data
        return presented;
    }


    /**
     * Convert the readAnaloguePort raw package to actual data
     *
     * @brief LabJackPresenter::readAnaloguePort
     * @return
     */
    QVariantMap LabJackPresenter::readAnaloguePort(QVariantMap commands, QStringList package)
    {
        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_readAnaloguePort";

        // Return the presenter data
        return presented;
    }

}}}}
