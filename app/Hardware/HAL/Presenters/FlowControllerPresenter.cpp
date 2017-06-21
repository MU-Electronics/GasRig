#include "FlowControllerPresenter.h"

#include <QStringList>
#include <QVariantMap>
#include <QDebug>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    FlowControllerPresenter::FlowControllerPresenter(QObject *parent)
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
    QVariantMap FlowControllerPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        // Select the correct presenter
        if(method == "getIdentifier")
        {
            return getIdentifier(commands, package);
        }
        else if (method == "getFlowRate")
        {
            return getFlowRate(commands, package);
        }
        else if (method == "getSetFlowRate")
        {
            return getSetFlowRate(commands, package);
        }
        else if (method == "getValveOverride")
        {
            return getValveOverride(commands, package);
        }
        else if (method == "getControllerTemperature")
        {
            return getControllerTemperature(commands, package);
        }
        else if(method == "setSourceControll")
        {
            return setSourceControll(commands, package);
        }
        else if (method == "setFlowRate")
        {
            return setFlowRate(commands, package);
        }
        else if (method == "setValveOverride")
        {
            return setValveOverride(commands, package);
        }
        else if (method == "setSoftStart")
        {
            return setSoftStart(commands, package);
        }
        else if (method == "setSoftStartTime")
        {
            return setSoftStartTime(commands, package);
        }
        else if (method == "setFlowUnit")
        {
            return setFlowUnit(commands, package);
        }
        else if (method == "setTemperatureUnit")
        {
            return setTemperatureUnit(commands, package);
        }

        // No method could be found
        QVariantMap error;
        error["error"] = "NoMethod";
        return error;
    }



    /**
     *
     *
     * @brief FlowControllerPresenter::getIdentifier
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getIdentifier(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerIdentifier";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        qDebug() << package;

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::getFlowRate
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getFlowRate(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerFlowRate";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        qDebug() << package;

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::getSetFlowRate
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getSetFlowRate(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerSetFlowRate";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];
        qDebug() << package;
        // Get set flow rate in unit (12)
        FourByteFloatConvertion.buf[0] = package.at(21).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(20).toInt();
        FourByteFloatConvertion.buf[2] = package.at(19).toInt();
        FourByteFloatConvertion.buf[3] = package.at(18).toInt(); // MSB

        presented["flow"] = FourByteFloatConvertion.number;
        presented["flow_unit"] = package.at(17).toInt();

        // Get set flow rate in percentage
        FourByteFloatConvertion.buf[0] = package.at(16).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(15).toInt();
        FourByteFloatConvertion.buf[2] = package.at(14).toInt();
        FourByteFloatConvertion.buf[3] = package.at(13).toInt(); // MSB

        presented["percentage"] = FourByteFloatConvertion.number;
        presented["percentage_code"] = package.at(12).toInt();

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::getValveOverride
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getValveOverride(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerValveOverride";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Get the override value
        int override = package.at(12).toInt();
        presented["override"] = override;

        if(override == 0)
            presented["override_verbal"] = "off";

        if(override == 1)
            presented["override_verbal"] = "open";

        if(override == 2)
            presented["override_verbal"] = "close";

        if(override == 3)
            presented["override_verbal"] = "manual (read only)";

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::getControllerTemperature
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getControllerTemperature(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerTemperature";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        qDebug() << package;

        // Return the presenter data
        return presented;
    }











    /**
     *
     *
     * @brief FlowControllerPresenter::setSourceControll
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::setSourceControll(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setFlowControllerSourceControl";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // New source value
        presented["source"] = package.at(12).toInt();

        qDebug() << package;

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::setFlowRate
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::setFlowRate(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setFlowControllerFlowRate";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Get set flow rate in unit (12)
        FourByteFloatConvertion.buf[0] = package.at(21).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(20).toInt();
        FourByteFloatConvertion.buf[2] = package.at(19).toInt();
        FourByteFloatConvertion.buf[3] = package.at(18).toInt(); // MSB

        presented["flow"] = FourByteFloatConvertion.number;
        presented["flow_unit"] = package.at(17).toInt();

        // Get set flow rate in percentage
        FourByteFloatConvertion.buf[0] = package.at(16).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(15).toInt();
        FourByteFloatConvertion.buf[2] = package.at(14).toInt();
        FourByteFloatConvertion.buf[3] = package.at(13).toInt(); // MSB

        presented["percentage"] = FourByteFloatConvertion.number;
        presented["percentage_code"] = package.at(12).toInt();

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief sFlowControllerPresenter::etValveOverride
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::setValveOverride(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setFlowControllerValveOverride";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Get the override value
        int override = package.at(12).toInt();
        presented["override"] = override;

        if(override == 0)
            presented["override_verbal"] = "off";

        if(override == 1)
            presented["override_verbal"] = "open";

        if(override == 2)
            presented["override_verbal"] = "close";

        if(override == 3)
            presented["override_verbal"] = "manual (read only)";

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::setSoftStart
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::setSoftStart(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setFlowControllerSoftStart";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Set the new state
        presented["state"] = package.at(12).toInt();

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::setSoftStartTime
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::setSoftStartTime(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setFlowControllerSoftStartTime";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Set the flow rate bytes
        FourByteFloatConvertion.buf[0] = package.at(15).toInt(); // MSB
        FourByteFloatConvertion.buf[1] = package.at(14).toInt();
        FourByteFloatConvertion.buf[2] = package.at(13).toInt();
        FourByteFloatConvertion.buf[3] = package.at(12).toInt(); // LSB

        // Convert float to bytes
        presented["seconds"] = FourByteFloatConvertion.number;

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::setFlowUnit
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::setFlowUnit(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setFlowControllerFlowUnit";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        qDebug() << package;

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::setTemperatureUnit
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::setTemperatureUnit(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setFlowControllerTemperatureUnit";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        qDebug() << package;

        // Return the presenter data
        return presented;
    }



    QStringList FlowControllerPresenter::parse(QStringList &package)
    {
        // Ensure only two start bytes; could be up to 5
        while(package.at(2).toInt() == 255)
        {
            package.removeFirst();
        }

        return package;
    }


}}}}
