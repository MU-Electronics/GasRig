#include "Testing.h"

#include <QObject>
#include <QDebug>
#include <QVariantMap>
#include <QString>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace ViewManager
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        exampleVar("Hello from C++");
    }


    void Testing::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Testing::hardwareRequest, &hardware, &Hardware::Access::hardwareAccess);

        // Connect incomming signals to actions
        connect(&hardware, &Hardware::Access::emit_pressureSensorInit, this, &Testing::pressureSensorInit);
        connect(&hardware, &Hardware::Access::emit_pressureSensorPressure, this, &Testing::pressureSensorReading);
    }


    /**
     * Debug method for pressure sensor init
     *
     * @brief Testing::pressureSensorInit
     * @param command
     */
    void Testing::pressureSensorInit(QVariantMap command)
    {
        qDebug() << "Pressure sensor was init: " << command.value("hardware_correct").toString();
    }

    /**
     * Debug method for pressure sensor reading
     *
     * @brief Testing::pressureSensorReading
     * @param command
     */
    void Testing::pressureSensorReading(QVariantMap command)
    {
        qDebug() << "Pressure sensor was reading was: " << command.value("pressure").toFloat();
    }






    /**
     * Request that the vacuum pump is turned on
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestBackingPump(bool onOff)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setPumpingState");
        command.insert("state", onOff);

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    /**
     * Request that the vacuum pump is turned on
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestTurboPump(bool onOff)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setTurboPumpState");
        command.insert("state", onOff);

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    /**
     * Request that the vacuum pump is turned on
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestBackingPumpMode(int mode)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setBackingPumpMode");
        command.insert("mode", mode);

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }






    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureConfirmation()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "PressureSensor");
        command.insert("method", "confirmInit");
        command.insert("channel", "1");

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureReading()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "PressureSensor");
        command.insert("method", "readPressure");
        command.insert("channel", "1");

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }
}}

