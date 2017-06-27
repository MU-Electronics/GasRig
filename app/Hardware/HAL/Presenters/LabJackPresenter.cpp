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

        qDebug() << "Set analgoue port presenter is currently not implimented" <<package;

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

        qDebug() << "Read port direction presenter is currently not implimented" << package;

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

        qDebug() << "Read digital port presenter is currently not implimented" << package;

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

        // Which signal should be triggered by the access thread
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
