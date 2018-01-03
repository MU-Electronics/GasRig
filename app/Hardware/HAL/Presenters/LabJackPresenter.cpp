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
        if(method == "configureIO" && expectedPackage(commands, package, 12))
        {
            return extend(commands, configureIO(commands, package));
        }
        else if (method == "setDigitalPort" && expectedPackage(commands, package, 10))
        {
            return extend(commands, setDigitalPort(commands, package));
        }
        else if (method == "setAnaloguePort" && expectedPackage(commands, package, 10))
        {
            return extend(commands, setAnaloguePort(commands, package));
        }
        else if (method == "readPortDirection" && expectedPackage(commands, package, 10))
        {
            return extend(commands, readPortDirection(commands, package));
        }
        else if (method == "readDigitalPort" && expectedPackage(commands, package, 10))
        {
            return extend(commands, readDigitalPort(commands, package));
        }
        else if (method == "readAnaloguePort"  && expectedPackage(commands, package, 12))
        {
            return extend(commands, readAnaloguePort(commands, package));
        }

        // There was an error
        if(error_returnedPackageSize != -1)
        {
            // Log error
            qCCritical(halAccessLabJackPresenter) << "Could not find the correct flow controll presenter method. " << generateError(method, commands, package);

            // Generate the error package and sent it back
            return generateError(method, commands, package);
        }
        else
        {
            // No method could be found generate error package
            QVariantMap error;
            error["error_id"] = "LabJackPresenter_NoMethodFound";
            error["level"] = "critical";
            error["message"] = "The method " + method + " does not exist in the flow controller presenter class.";

            // Log error
            qCCritical(halAccessLabJackPresenter) << "Could not find the correct labjack presenter method. " << error;

            // Return the package
            return error;
        }
    }


    /**
     * Check whether the package returned is the expected package
     *
     * @brief LabJackPresenter::expectedPackage
     * @param commands
     * @param package
     * @return
     */
    bool LabJackPresenter::expectedPackage(QVariantMap commands, QStringList package, int expectedLength)
    {
        // Check the length of the package
        if(package.length() != expectedLength)
        {
            // Save the returned package size
            error_returnedPackageSize = package.length();

            // Invalid package
            return false;
        }

        // Passed all tests
        return true;
    }



    /**
     * Creates an error package
     *
     * @brief LabJackPresenter::generateError
     * @param commands
     * @param package
     * @return
     */
    QVariantMap LabJackPresenter::generateError(QString method, QVariantMap commands, QStringList package)
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
            error["error_id"] = "LabJackPresenter_InvalidPackageLength";
            error["level"] = "warning";
            error["message"] = "Method:" + method + " was called but the responce length was not as expected.";
            error["send_command"] = compiledCommands;
            error["returned_package"] = compiledPackages;
        }

        // Return the error package
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

        // Convert setting to binary
        QString FIOConfigWord = QString("%1").arg(package.at(10).toInt(), 8, 2, QChar('0'));
        QString EIOConfigWord = QString("%1").arg(package.at(11).toInt(), 8, 2, QChar('0'));

        // Get the data for FIO ports
        presented["FIO0"] = FIOConfigWord.at(7);
        presented["FIO1"] = FIOConfigWord.at(6);
        presented["FIO2"] = FIOConfigWord.at(5);
        presented["FIO3"] = FIOConfigWord.at(4);
        presented["FIO4"] = FIOConfigWord.at(3);
        presented["FIO5"] = FIOConfigWord.at(2);
        presented["FIO6"] = FIOConfigWord.at(1);
        presented["FIO7"] = FIOConfigWord.at(0);

        // Get the data for EIO ports
        presented["EIO0"] = EIOConfigWord.at(7);
        presented["EIO1"] = EIOConfigWord.at(6);
        presented["EIO2"] = EIOConfigWord.at(5);
        presented["EIO3"] = EIOConfigWord.at(4);
        presented["EIO4"] = EIOConfigWord.at(3);
        presented["EIO5"] = EIOConfigWord.at(2);
        presented["EIO6"] = EIOConfigWord.at(1);
        presented["EIO7"] = EIOConfigWord.at(0);


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

        // Has no responce data (https://labjack.com/support/datasheets/u3/low-level-function-reference/feedback/dac16)
        presented["port"] = commands["port"];
        presented["value"] = commands["value"];

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

        // Which port was requested
        presented["port"] = commands["port"];

        // Returns 1 bit = direction (https://labjack.com/support/datasheets/u3/low-level-function-reference/feedback/bitdirread)
        presented["direction"] = package.at(9).toInt();

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

        // Which port was requested
        presented["port"] = commands["port"];

        // Returns 1 bit = state (https://labjack.com/support/datasheets/u3/low-level-function-reference/feedback/bitstateread)
        presented["state"] = package.at(9).toInt();

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

        // Which port was requested
        presented["port"] = commands["port"];

        // Get the voltage in digital form
        unsigned short bytes = package.at(9).toInt() + (package.at(10).toInt()*256);
        double voltage = (double) bytes;

        // Find cal voltage
        double slope = 	commands["slope"].toDouble();
        double offset = commands["offset"].toDouble();
        double voltageCalibrated = ( slope * voltage ) + offset;

        // Save the calibrated voltage
        presented["calibrated"] = voltageCalibrated;

        // Return the presenter data
        return presented;
    }

}}}}
