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

        // Connect incomming signals to actions for the pressure sensor
        connect(&hardware, &Hardware::Access::emit_pressureSensorInit, this, &Testing::receivePressureSensorInit);
        connect(&hardware, &Hardware::Access::emit_pressureSensorPressure, this, &Testing::receivePressureSensorReading);
        connect(&hardware, &Hardware::Access::emit_pressureSensorSerialNumber, this, &Testing::receivePressureSensorSerialNumber);

        // Connect incomming signals to actions for the vac station
        connect(&hardware, &Hardware::Access::emit_setPumpingState, this, &Testing::receiveVacSetPump);
        connect(&hardware, &Hardware::Access::emit_setTurboPumpState, this, &Testing::receiveVacSetTurbo);
        connect(&hardware, &Hardware::Access::emit_setGasMode, this, &Testing::receiveVacSetGasMode);
        connect(&hardware, &Hardware::Access::emit_setBackingPumpMode, this, &Testing::receiveVacSetPumpMode);

        // Connect incomming signals to actions for the LabJack
        connect(&hardware, &Hardware::Access::emit_setDigitalPort, this, &Testing::receiveValveStatus);
        connect(&hardware, &Hardware::Access::emit_configureIO, this, &Testing::receiveLabJackConfig);
        connect(&hardware, &Hardware::Access::emit_setAnaloguePort, this, &Testing::receiveVacuumReading);

        // Connect incomming signals to actions for the flow controllers
        connect(&hardware, &Hardware::Access::emit_setFlowControllerValveOverride, this, &Testing::receiveFlowControllerValveOverride);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerFlowRate, this, &Testing::receiveSetFlowControllerFlowRate);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStart, this, &Testing::receiveSetFlowControllerSoftStart);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStartTime, this, &Testing::receiveSetFlowControllerSoftStartTime);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSourceControl, this, &Testing::receiveSetFlowControllerSourceControl);


    }


    /**
     * Debug method for flow controller set valve override
     *
     * @brief Testing::pressureSensorInit
     * @param command
     */
    void Testing::receiveFlowControllerValveOverride(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("Valve override set to " + command.value("override_verbal").toString() + " for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set flow rate
     *
     * @brief Testing::pressureSensorInit
     * @param command
     */
    void Testing::receiveSetFlowControllerFlowRate(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("Flow rate set to " + command.value("flow").toString() + " for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set soft start
     *
     * @brief SystemStatus::receiveSetFlowControllerSoftStart
     * @param command
     */
    void Testing::receiveSetFlowControllerSoftStart(QVariantMap command)
    {
        QString softstart = "disabled";
        if(command.value("state").toInt() == 4)
            softstart = "linear ramp";

        emit emit_testingMaintenanceReply("Flow controller's soft start set to " + softstart + " for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set soft start time
     *
     * @brief SystemStatus::receiveSetFlowControllerSoftStartTime
     * @param command
     */
    void Testing::receiveSetFlowControllerSoftStartTime(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("Flow controller's soft start time constant set to " + command.value("seconds").toString() + " seconds for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set source
     *
     * @brief Testing::receiveSetFlowControllerSourceControl
     * @param command
     */
    void Testing::receiveSetFlowControllerSourceControl(QVariantMap command)
    {
        QString source = "digital";
        if(command.value("source").toInt() == 1)
            QString source = "analogue";
        emit emit_testingMaintenanceReply("Flow controller's source set to " + source + " for " + command.value("controller").toString());

    }










    /**
     * Debug method for pressure sensor init
     *
     * @brief Testing::pressureSensorInit
     * @param command
     */
    void Testing::receivePressureSensorInit(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("Pressure sensor init: " + command.value("hardware_correct").toString());
    }

    /**
     * Debug method for pressure sensor reading
     *
     * @brief Testing::pressureSensorReading
     * @param command
     */
    void Testing::receivePressureSensorSerialNumber(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("Pressure sensor serial number is: " + command.value("serial").toString());
    }

    /**
     * Debug method for pressure sensor serial number
     *
     * @brief Testing::pressureSensorReading
     * @param command
     */
    void Testing::receivePressureSensorReading(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("Pressure sensor was reading was: " + command.value("pressure").toString());
    }













    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveVacSetGasMode(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("The gas mode was set to: " + command.value("mode_verbal").toString());
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveVacSetPumpMode(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("The pump mode was set to: " + command.value("mode_verbal").toString());
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveVacSetPump(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("The pump state is: " + command.value("state_verbal").toString());
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveVacSetTurbo(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("The turbo state is: " + command.value("state_verbal").toString());
    }












    /**
     * Debug method for setting the valve status
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveValveStatus(QVariantMap command)
    {
        // Get the port name
        QString portNumber  = m_settings.hardware.valve_connections.key(command.value("port").toString());

        if(!portNumber.isNull())
        {
            // Get verbal value
            QString value = "closed";
            if(command.value("value").toBool())
                value = "open";

            // Update the displays
            emit emit_testingMaintenanceReply("The status of valve " + portNumber + " was updated to " + value);
        }
    }

    /**
     * Debug method for setting the config
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveLabJackConfig(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("The LabJack config is: "/* + command.value("state_verbal").toString()*/);
    }

    /**
     * Debug method for reading the vacuum sensor
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveVacuumReading(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("The LabJack config is: "/* + command.value("state_verbal").toString()*/);
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
     * Request that the vacuum pump is turned on
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestGasMode(int mode)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setGasMode");
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

    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureSerialNumber()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "PressureSensor");
        command.insert("method", "readSerialNumber");

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }















    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestLabJackConfig()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "LabJack");
        command.insert("method", "configureIO");

        // Config for gas rig
        command.insert("FIO7", 0);
        command.insert("FIO6", 0);
        command.insert("FIO5", 0);
        command.insert("FIO4", 0);
        command.insert("EIO7", 0);
        command.insert("EIO6", 0);
        command.insert("EIO5", 0);
        command.insert("EIO4", 0);
        command.insert("EIO3", 0);
        command.insert("EIO2", 0);
        command.insert("EIO1", 0);
        command.insert("EIO0", 0);

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }


    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestValveState(int port, bool state)
    {
        // Find the correct valve name
        QString valveName = m_settings.hardware.valve_connections.value(QString::number(port)).toString();

        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "LabJack");
        command.insert("method", "setDigitalPort");
        command.insert("port", valveName);
        command.insert("value", state);

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }



    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestVacuumPressure()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "LabJack");
        command.insert("method", "readAnaloguePort");
        command.insert("port", "AIN0");

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }















    /**
     * Request a valve override on the flow controllers
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerValveOverride(QString controller, int state)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setValveOverride");

        // Set the state of the overrie
        command.insert("state", QString::number(state));

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }


    /**
     * Request flow rate
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerFlowRate(QString controller, int flowrate)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setFlowRate");

        // Set relative to the flow unit
        command.insert("unit", "250");

        // Set the flow rate
        command.insert("rate", QString::number(flowrate));

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    /**
     * Request flow rate soft start enable
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerSoftStart(QString controller, int state)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setSoftStart");

        // Enabled / disabled
        if(state == 4)
        {
            command.insert("type", "4");
        }
        else
        {
            command.insert("type", "1");
        }

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

    /**
     * Request flow rate soft start in seconds
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerSoftStartTime(QString controller, int seconds)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setSoftStartTime");

        // Set number of seconds
        command.insert("seconds", QString::number(seconds));

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }


    void Testing::requestSetFlowControllerSourceControl(QString controller, int source)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setSourceControll");

        // Set number of seconds
        command.insert("source", QString::number(source));

        // Emit siganl to HAL
        emit hardwareRequest(command);
    }

}}

