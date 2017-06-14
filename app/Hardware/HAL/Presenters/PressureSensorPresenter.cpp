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
        presented["method"] = "emit_pressureSensorPressure";

        // Calculate the serial number from the package
        presented["serial"] = QString::number(((256^3)*package.at(2).toInt()) + ((256^2)*package.at(3).toInt()) + ((256)*package.at(4).toInt()) + package.at(0).toInt());

        // Return the presenter data
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
        presented["pressure"] = QString::number(fValue);

        // Return the presenter data
        return presented;
    }

}}}}
