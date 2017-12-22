#include "FlowControllerPresenter.h"

#include <math.h>

#include <QStringList>
#include <QVariantMap>
#include <QDebug>
#include <QMapIterator>
#include <QString>
#include <QVariant>

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
        // Addtion bytes other than the data section of the package (common to all package)
        int addtionPackageBytes = 13;

        // Select the correct presenter
        if(method == "getIdentifier" && expectedPackage(commands, package, 11, 12 + addtionPackageBytes))
        {
            return extend(commands, getIdentifier(commands, package));
        }
        else if (method == "getSourceControl" && expectedPackage(commands, package, 215, 14 + addtionPackageBytes))
        {
            return extend(commands, getSourceControl(commands, package));
        }
        else if (method == "getSoftStart" && expectedPackage(commands, package, 215, 14 + addtionPackageBytes))
        {
            return extend(commands, getSoftStart(commands, package));
        }
        else if (method == "getSoftStartTime" && expectedPackage(commands, package, 215, 14 + addtionPackageBytes))
        {
            return extend(commands, getSoftStartTime(commands, package));
        }
        else if (method == "getFlowRate" && expectedPackage(commands, package, 1, 5 + addtionPackageBytes))
        {
            return extend(commands, getFlowRate(commands, package));
        }
        else if (method == "getSetFlowRate" && expectedPackage(commands, package, 235, 10 + addtionPackageBytes))
        {
            return extend(commands, getSetFlowRate(commands, package));
        }
        else if (method == "getValveOverride" && expectedPackage(commands, package, 230, 1 + addtionPackageBytes))
        {
            return extend(commands, getValveOverride(commands, package));
        }
        else if (method == "getControllerTemperature" && expectedPackage(commands, package, 3, 14 + addtionPackageBytes))
        {
            return extend(commands, getControllerTemperature(commands, package));
        }
        else if(method == "setSourceControll" && expectedPackage(commands, package, 216, 1 + addtionPackageBytes))
        {
            return extend(commands, setSourceControll(commands, package));
        }
        else if (method == "setFlowRate" && expectedPackage(commands, package, 236, 10 + addtionPackageBytes))
        {
            return extend(commands, setFlowRate(commands, package));
        }
        else if (method == "setValveOverride" && expectedPackage(commands, package, 231, 1 + addtionPackageBytes))
        {
            return extend(commands, setValveOverride(commands, package));
        }
        else if (method == "setSoftStart" && expectedPackage(commands, package, 218, 1 + addtionPackageBytes))
        {
            return extend(commands, setSoftStart(commands, package));
        }
        else if (method == "setSoftStartTime" && expectedPackage(commands, package, 219, 4 + addtionPackageBytes))
        {
            return extend(commands, setSoftStartTime(commands, package));
        }
        else if (method == "setFlowUnit" && expectedPackage(commands, package, 196, 2 + addtionPackageBytes))
        {
            return extend(commands, setFlowUnit(commands, package));
        }
        else if (method == "setTemperatureUnit" && expectedPackage(commands, package, 197, 1 + addtionPackageBytes))
        {
            return extend(commands, setTemperatureUnit(commands, package));
        }

        // There was an error
        if(error_returnedPackageSize != -1 || error_returnedCommandId != -1)
        {
            // Log error
            qCCritical(halAccessFlowControllerPresenter) << "There was an error with the package size or command id" << generateError(method, commands, package);

            // Generate the error package and sent it back
            return generateError(method, commands, package);
        }
        else
        {
            // No method could be found generate error package
            QVariantMap error;
            error["error_id"] = "FlowControllerPresenter_NoMethodFound";
            error["level"] = "critical";
            error["message"] = "The method " + method + " does not exist in the flow controller presenter class.";

            // Log error
            qCCritical(halAccessFlowControllerPresenter) << "Could not find the correct flow controll presenter method. " << error;

            // Return the package
            return error;
        }
    }



    /**
     * Check whether the package returned is the expected package
     *
     * @brief FlowControllerPresenter::expectedPackage
     * @param commands
     * @param package
     * @return
     */
    bool FlowControllerPresenter::expectedPackage(QVariantMap commands, QStringList package, int commandId, int expectedLength)
    {
        // Prep package for validating
        parse(package);

        // Check the length of the package
        if(package.length() != expectedLength)
        {
            // Save the returned package size
            error_returnedPackageSize = package.length();

            // Invalid package
            return false;
        }

        // Get the command ID
        if(commandId != package.at(8).toInt())
        {
            // Save the returned id
            error_returnedCommandId = package.at(8).toInt();

            // Invalid package
            return false;
        }

        // Passed all tests
        return true;
    }


    /**
     * Creates an error package
     *
     * @brief FlowControllerPresenter::generateError
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::generateError(QString method, QVariantMap commands, QStringList package)
    {
        // Container for error
        QVariantMap error;

        // Strings to hold the compiled data
        QString compiledCommands = "";
        QString compiledPackages = "";

        // Compile commands package
        if(commands.size() > 0)
        {
            QMapIterator<QString, QVariant> i(commands);
            while (i.hasNext())
            {
                // Move to next record
                i.next();

                // Compile the data
                compiledCommands += i.key() + " : " + i.value().toString() + ";    ";
            }
        }

        // Compile returned package
        if(package.size() > 0)
        {
            for (int i = 0; i < package.size(); ++i)
                compiledPackages += package.at(i);
        }

        // If package size was too small
        if(error_returnedPackageSize == -1)
        {
            // Create an error id
            error["error_id"] = "FlowControllerPresenter_InvalidPackageLength";
            error["level"] = "warning";
            error["message"] = "Method:" + method + " was called but the responce length was not as expected.";
            error["send_command"] = compiledCommands;
            error["returned_package"] = compiledPackages;
        }
        else // If command id is not what was expected
        {
            // Create an error id
            error["error_id"] = "FlowControllerPresenter_InvalidCommandId";
            error["level"] = "warning";
            error["message"] = "Method:" + method + " was called but the responce contained a command id which differs form the requested command.";
            error["send_command"] = compiledCommands;
            error["returned_package"] = compiledPackages;
        }

        // Return the error package
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




        qDebug() << "Flow controller get identifiers need implimenting"  << package;







        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::getSourceControl
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getSourceControl(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerSourceControl";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Convert float to bytes
        presented["source"] = package.at(12).toInt();

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief FlowControllerPresenter::getSoftStart
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getSoftStart(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerSoftStart";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Set the flow rate bytes
        FourByteFloatConvertion.buf[0] = package.at(25).toInt(); // MSB
        FourByteFloatConvertion.buf[1] = package.at(24).toInt();
        FourByteFloatConvertion.buf[2] = package.at(23).toInt();
        FourByteFloatConvertion.buf[3] = package.at(22).toInt(); // LSB

        // Convert float to bytes
        presented["state"] = package.at(21).toInt();

        // Convert float to bytes
        presented["seconds"] = FourByteFloatConvertion.number;

        // Return the presenter data
        return presented;
    }


    /**
     *
     *
     * @brief getSoftStartTime
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::getSoftStartTime(QVariantMap commands, QStringList package)
    {
        // Prep package
        parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getFlowControllerSoftStartTime";

        // Which signal should be triggered by the access thread
        presented["controller"] = commands["controller"];

        // Set the flow rate bytes
        FourByteFloatConvertion.buf[0] = package.at(25).toInt(); // MSB
        FourByteFloatConvertion.buf[1] = package.at(24).toInt();
        FourByteFloatConvertion.buf[2] = package.at(23).toInt();
        FourByteFloatConvertion.buf[3] = package.at(22).toInt(); // LSB

        // Convert float to bytes
        presented["state"] = package.at(21).toInt();

        // Convert float to bytes
        presented["seconds"] = FourByteFloatConvertion.number;

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

        // Get set flow rate
        FourByteFloatConvertion.buf[0] = package.at(16).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(15).toInt();
        FourByteFloatConvertion.buf[2] = package.at(14).toInt();
        FourByteFloatConvertion.buf[3] = package.at(13).toInt(); // MSB

        presented["flow"] = (double) floor(FourByteFloatConvertion.number * 10000) / 10000;
        presented["flow_unit"] = package.at(12).toInt();

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

        // Get set flow rate in unit (12)
        FourByteFloatConvertion.buf[0] = package.at(21).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(20).toInt();
        FourByteFloatConvertion.buf[2] = package.at(19).toInt();
        FourByteFloatConvertion.buf[3] = package.at(18).toInt(); // MSB

        presented["flow"] = (double) floor(FourByteFloatConvertion.number * 10000) / 10000;
        presented["flow_unit"] = package.at(17).toInt();

        // Get set flow rate in percentage
        FourByteFloatConvertion.buf[0] = package.at(16).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(15).toInt();
        FourByteFloatConvertion.buf[2] = package.at(14).toInt();
        FourByteFloatConvertion.buf[3] = package.at(13).toInt(); // MSB

        presented["percentage"] = (double) floor(FourByteFloatConvertion.number * 100) / 100;
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

        // Get temperature
        FourByteFloatConvertion.buf[0] = package.at(25).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(24).toInt();
        FourByteFloatConvertion.buf[2] = package.at(23).toInt();
        FourByteFloatConvertion.buf[3] = package.at(22).toInt(); // MSB

        presented["temperature"] = (double) floor(FourByteFloatConvertion.number * 100) / 100;
        presented["temperature_unit"] = package.at(21).toInt();

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

        presented["flow"] = (double) floor(FourByteFloatConvertion.number * 10000) / 10000;
        presented["flow_unit"] = package.at(17).toInt();

        // Get set flow rate in percentage
        FourByteFloatConvertion.buf[0] = package.at(16).toInt(); // LSB
        FourByteFloatConvertion.buf[1] = package.at(15).toInt();
        FourByteFloatConvertion.buf[2] = package.at(14).toInt();
        FourByteFloatConvertion.buf[3] = package.at(13).toInt(); // MSB


        presented["percentage"] = (double) floor(FourByteFloatConvertion.number * 100) / 100;
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

        qDebug() << "Set flow unit has not be implimented" << package;

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

        qDebug() << "Set temperature unit has not be implimented" << package;

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
