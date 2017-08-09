#include "VacStationPresenter.h"

#include <QStringList>
#include <QVariantMap>
#include <QDebug>

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
        if(method == "getTemperature" && expectedPackage(commands, package, "342,330,326,346", 20))
        {
            return getTemperature(commands, package);
        }
        else if (method == "getTurboSpeed" && expectedPackage(commands, package, "398,336,399,397", 20))
        {
            return getTurboSpeed(commands, package);
        }
        else if (method == "getError" && expectedPackage(commands, package, "360,361,362,363,364,365,366,367,368,369", 20))
        {
            return getError(commands, package);
        }
        else if (method == "getGasMode" && expectedPackage(commands, package, "027", 17))
        {
            return getGasMode(commands, package);
        }
        else if (method == "getBackingPumpMode" && expectedPackage(commands, package, "025", 17))
        {
            return getBackingPumpMode(commands, package);
        }
        else if (method == "getTurboPumpState" && expectedPackage(commands, package, "023", 20))
        {
            return getTurboPumpState(commands, package);
        }
        else if (method == "getPumpingState" && expectedPackage(commands, package, "010", 20))
        {
            return getPumpingState(commands, package);
        }
        else if (method == "setGasMode" && expectedPackage(commands, package, "027", 17))
        {
            return setGasMode(commands, package);
        }
        else if (method == "setBackingPumpMode" && expectedPackage(commands, package, "025", 17))
        {
            return setBackingPumpMode(commands, package);
        }
        else if (method == "setTurboPumpState" && expectedPackage(commands, package, "023", 20))
        {
            return setTurboPumpState(commands, package);
        }
        else if (method == "setPumpingState" && expectedPackage(commands, package, "010", 20))
        {
            return setPumpingState(commands, package);
        }

        // There was an error
        if(error_returnedPackageSize != -1 || error_returnedCommandId != -1)
        {
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

            // Return the package
            return error;
        }
    }




    /**
     * Check whether the package returned is the expected package
     *
     * @brief VacStationPresenter::expectedPackage
     * @param commands
     * @param package
     * @return
     */
    bool VacStationPresenter::expectedPackage(QVariantMap commands, QStringList package, QString commandId, int expectedLength)
    {       
        // Combind the data
        QString readData;
        if(package.length() > 0)
            readData = package.join("");

        // Check the length of the package
        if(readData.count() != expectedLength)
        {
            qDebug() << "package count is incorrect, expected: " << expectedLength << " got:" << readData.count();
            // Save the returned package size
            error_returnedPackageSize = readData.length();

            // Invalid package
            return false;
        }

        // Get the parameter number
        QString commandIdReturned = readData.mid(5, 3);

        // Check command ID
        if(!commandId.contains(commandIdReturned))
        {
            qDebug() << "The command is not correct, expected one of: " << commandId << " got:" << commandIdReturned;
            // Save the returned id
            error_returnedCommandId = commandIdReturned.toInt();

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
    QVariantMap VacStationPresenter::generateError(QString method, QVariantMap commands, QStringList package)
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
            error["error_id"] = "VacStationPresenter_InvalidPackageLength";
            error["level"] = "warning";
            error["message"] = "Method:" + method + " was called but the responce length was not as expected.";
            error["send_command"] = compiledCommands;
            error["returned_package"] = compiledPackages;
        }
        else // If command id is not what was expected
        {
            // Create an error id
            error["error_id"] = "VacStationPresenter_InvalidCommandId";
            error["level"] = "warning";
            error["message"] = "Method:" + method + " was called but the responce contained a command id which differs from the requested command.";
            error["send_command"] = compiledCommands;
            error["returned_package"] = compiledPackages;
        }

        // Return the error package
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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getTemperature";

        // Set which area
        presented.insert("location", commands.value("location").toInt());

        // Get the verbal location
        presented.insert("location_verbal", "unknown");
        if(presented["location"] == 1)
            presented.insert("location_verbal", "pump bottom");
        if(presented["location"] == 2)
            presented.insert("location_verbal", "electronics");
        if(presented["location"] == 3)
            presented.insert("location_verbal", "bearing");
        if(presented["location"] == 4)
            presented.insert("location_verbal", "motor");

        // Get the temperature
        presented.insert("temperature", data.value("data").toDouble());

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getTurboSpeed";

        // Save the turbo speed
        presented.insert("speed", data.value("data").toDouble());

        // Set which area
        presented.insert("type", commands.value("type").toInt());

        // Get the unit
        presented.insert("type_unit", "rpm");

        // Get the verbal location
        presented.insert("location_verbal", "unknown");
        if(presented["type"] == 1)
        {
            presented.insert("type_desc_verbal", "accel-decel");
        }
        else if(presented["type"] == 2)
        {
            presented.insert("type_desc_verbal", "actual rotation");
        }
        else if(presented["type"] == 3)
        {
            presented.insert("type_desc_verbal", "nominal");
        }
        else if(presented["type"] == 4)
        {
            presented.insert("type_desc_verbal", "set rotation");
        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getError";


        qDebug() << "GET ERROR MESSAGE NEEDS IMPLIMENTING IN PRESENTER";


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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getGasMode";

        // State of backing pump mode
        if(data.value("data").toString() == "000")
        {
            presented.insert("mode", 0);
            presented.insert("mode_verbal", "molecular mass greater than or equal to 39");
        }
        else if(data.value("data").toString() == "001")
        {
            presented.insert("mode", 1);
            presented.insert("mode_verbal", "molecular mass less than 39");
        }
        else if(data.value("data").toString() == "002")
        {
            presented.insert("mode", 2);
            presented.insert("mode_verbal", "helium gas only");
        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getBackingPumpMode";

        // State of backing pump mode
        if(data.value("data").toString() == "000")
        {
            presented.insert("mode", 0);
            presented.insert("mode_verbal", "continuous");
        }
        else if(data.value("data").toString() == "001")
        {
            presented.insert("mode", 1);
            presented.insert("mode_verbal", "intermittent");
        }
        else if(data.value("data").toString() == "002")
        {
            presented.insert("mode", 2);
            presented.insert("mode_verbal", "delayed");
        }
        else if(data.value("data").toString() == "003")
        {
            presented.insert("mode", 3);
            presented.insert("mode_verbal", "delayed + intermittent");
        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getTurboPumpState";

        qDebug() << "GET TURBO STATE NEEDS IMPLIMENTING IN PRESENTER";

        // State of pump
//        if(data.value("data").toString() == "111111")
//        {
//            presented.insert("state", true);
//            presented.insert("state_verbal", "on");
//        }
//        else
//        {
//            presented.insert("state", false);
//            presented.insert("state_verbal", "off");
//        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_getPumpingState";

        // State of backing pump mode
        if(data.value("data").toString() == "000")
        {
            presented.insert("mode", 0);
            presented.insert("mode_verbal", "off");
        }
        else if(data.value("data").toString() == "001")
        {
            presented.insert("mode", 1);
            presented.insert("mode_verbal", "on");
        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setGasMode";

        // State of backing pump mode
        if(data.value("data").toString() == "000")
        {
            presented.insert("mode", 0);
            presented.insert("mode_verbal", "molecular mass greater than or equal to 39");
        }
        else if(data.value("data").toString() == "001")
        {
            presented.insert("mode", 1);
            presented.insert("mode_verbal", "molecular mass less than 39");
        }
        else if(data.value("data").toString() == "002")
        {
            presented.insert("mode", 2);
            presented.insert("mode_verbal", "helium gas only");
        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setBackingPumpMode";

        // State of backing pump mode
        if(data.value("data").toString() == "000")
        {
            presented.insert("mode", 0);
            presented.insert("mode_verbal", "continuous");
        }
        else if(data.value("data").toString() == "001")
        {
            presented.insert("mode", 1);
            presented.insert("mode_verbal", "intermittent");
        }
        else if(data.value("data").toString() == "002")
        {
            presented.insert("mode", 2);
            presented.insert("mode_verbal", "delayed");
        }
        else if(data.value("data").toString() == "003")
        {
            presented.insert("mode", 3);
            presented.insert("mode_verbal", "delayed + intermittent");
        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setTurboPumpState";

        // ID of pump
        presented.insert("id", data.value("id").toInt());

        // State of pumo
        if(data.value("data").toString() == "111111")
        {
            presented.insert("state", true);
            presented.insert("state_verbal", "on");
        }
        else
        {
            presented.insert("state", false);
            presented.insert("state_verbal", "off");
        }

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
        // Parse data
        QVariantMap data = parse(package);

        // Container for returned data
        QVariantMap presented;

        // Which signal should be triggered by the access thread
        presented["method"] = "emit_setPumpingState";

        // ID of pump
        presented.insert("id", data.value("id").toInt());

        // State of pumo
        if(data.value("data").toString() == "111111")
        {
            presented.insert("state", true);
            presented.insert("state_verbal", "on");
        }
        else
        {
            presented.insert("state", false);
            presented.insert("state_verbal", "off");
        }

        // Return the presenter data
        return presented;
    }


    /**
     * PRIVATE: Parse the stirng list
     *
     * @brief VacStationPresenter::praseData
     * @param package
     * @return
     */
    QVariantMap VacStationPresenter::parse(QStringList package)
    {
        // Combind the data
        QString readData = package.join("");

        // Container to return
        QVariantMap toReturn;

        // Get the ID
        toReturn["id"] = readData.mid(0, 3);

        // Get the parameter number
        toReturn["param"] = readData.mid(5, 3);

        // Find how long the data part is
        toReturn["dataLength"] = readData.mid(8, 2);

        // Get the data
        toReturn["data"] = readData.mid(10, toReturn["dataLength"].toInt());

        return toReturn;
    }

}}}}
