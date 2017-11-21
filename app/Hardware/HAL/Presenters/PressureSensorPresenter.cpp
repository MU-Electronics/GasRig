#include "PressureSensorPresenter.h"

#include <QStringList>
#include <QVariantMap>
#include <QDebug>

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
        if(method == "confirmInit" && expectedPackage(commands, package, "48", 10))
        {
            return methodOverride(commands, confirmInit(commands, package));
        }
        else if (method == "readSerialNumber" && expectedPackage(commands, package, "69", 8))
        {
            return methodOverride(commands, readSerialNumber(commands, package));
        }
        else if (method == "readPressure" && expectedPackage(commands, package, "73", 9))
        {
            return methodOverride(commands, readPressure(commands, package));
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
            error["error_id"] = "PressureSensorPresenter_NoMethodFound";
            error["level"] = "critical";
            error["message"] = "The method " + method + " does not exist in the flow controller presenter class.";

            // Return the package
            return error;
        }
    }


    /**
     * Check whether the package returned is the expected package
     *
     * @brief PressureSensorPresenter::expectedPackage
     * @param commands
     * @param package
     * @return
     */
    bool PressureSensorPresenter::expectedPackage(QVariantMap commands, QStringList package, QString commandId, int expectedLength)
    {
        // Check the length of the package
        if(package.length() != expectedLength)
        {
            qDebug() << "package count is incorrect, expected: " << expectedLength << " got:" << package.count();
            // Save the returned package size
            error_returnedPackageSize = package.length();

            // Invalid package
            return false;
        }

        // Get the command ID
        if(commandId != package.at(1))
        {
            qDebug() << "The command is not correct, expected one of: " << commandId << " got:" << package.at(1);

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
     * @brief PressureSensorPresenter::generateError
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PressureSensorPresenter::generateError(QString method, QVariantMap commands, QStringList package)
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
            error["error_id"] = "PressureSensorPresenter_InvalidPackageLength";
            error["level"] = "warning";
            error["message"] = "Method:" + method + " was called but the responce length was not as expected.";
            error["send_command"] = compiledCommands;
            error["returned_package"] = compiledPackages;
        }
        else // If command id is not what was expected
        {
            // Create an error id
            error["error_id"] = "PressureSensorPresenter_InvalidCommandId";
            error["level"] = "warning";
            error["message"] = "Method:" + method + " was called but the responce contained a command id which differs from the requested command.";
            error["send_command"] = compiledCommands;
            error["returned_package"] = compiledPackages;
        }

        // Return the error package
        return error;

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

        // Assum pressure sensor was not initalised in the past
        presented["was_initalised"] = "false";
        if(package.at(7).toInt() == 1)
            // Sucessfully inited
            presented["was_initalised"] = "true";

        // Check that the pressure sensor is a series 30
        presented["hardware_correct"] = "false";
        if(package.at(2).toInt() == 5)
            // Sucessfully inited
            presented["hardware_correct"] = "true";

        // Return the presenter data
        return presented;
    }


    /**
     * Convert the readSerialNumber raw package to actual data
     *
     * @brief PressureSensorPresenter::readSerialNumber
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

        // Calculate the serial number from the package
        presented["serial"] = 256*65536*package.at(2).toInt()+65536*package.at(3).toInt()+256*package.at(4).toInt()+package.at(0).toInt();

        // Return the presenter data
        return presented;
    }


    /**
     * Convert the readPressure raw package to actual data
     *
     * @brief PressureSensorPresenter::readPressure
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

        // Flaot to hold the pressure value
        float fValue;

        // Container to hold the bytes
        unsigned char bteArr[4];

        // Assined the bytes to the container
        bteArr[0] = package.at(5).toInt();
        bteArr[1] = package.at(4).toInt();
        bteArr[2] = package.at(3).toInt();
        bteArr[3] = package.at(2).toInt();

        // Convert the char container to float value
        fValue = *(float*)(&bteArr[0]);

        // Assign the float value
        presented["pressure"] = fValue;

        // Return the presenter data
        return presented;
    }

}}}}
