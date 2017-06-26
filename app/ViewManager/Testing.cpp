#include "Testing.h"

#include <cmath>

#include <QObject>
#include <QDebug>
#include <QVariantMap>
#include <QString>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Include command constructor
#include "../Hardware/CommandConstructor.h"

namespace App { namespace ViewManager
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings),
          m_commandConstructor(*new Hardware::CommandConstructor)
    {

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
        connect(&hardware, &Hardware::Access::emit_readAnaloguePort, this, &Testing::receiveVacuumReading);

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
     * @brief Testing::receiveVacSetPump
     * @param command
     */
    void Testing::receiveVacSetPump(QVariantMap command)
    {
        emit emit_testingMaintenanceReply("The pump state is: " + command.value("state_verbal").toString());
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetTurbo
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
     * @brief Testing::receiveVacuumReading
     * @param command
     */
    void Testing::receiveVacuumReading(QVariantMap command)
    {
        // If port is the same as the vacuum guage port
        if(command["port"] == m_settings.hardware.vacuum_guage.value("connection").toString())
        {
            double pressure = (std::pow(10, (1.667*command.value("calibrated").toDouble()-9.333)))/100;
            emit emit_testingMaintenanceReply("Vacuum pressure is: " + QString::number(pressure));
        }
    }













    /**
     * Request that the vacuum pump is turned on
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestBackingPump(bool onOff)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPump(onOff));
    }

    /**
     * Request that the turbo pump is enabled
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestTurboPump(bool onOff)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setTurboPump(onOff));
    }

    /**
     * Request a new backing pump mode
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestBackingPumpMode(int mode)
    {
       // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPumpMode(mode));
    }

    /**
     * Request to set a new gas mode
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestGasMode(int mode)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(mode));
    }

    /**
     * Request the turbo speed
     *
     * @brief Testing::requestTurboSpeed
     */
    void Testing::requestTurboSpeed()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboSpeed());
    }

    /**
     * Request the bearning temperature
     *
     * @brief Testing::requestTurboSpeed
     */
    void Testing::requestTurboBearingTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboBearingTemperature());
    }













    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureConfirmation()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.pressureConfirmation());
    }

    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureReading()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getPressureReading(1));
    }

    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureSerialNumber()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getPressureSerialNumber());
    }















    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestLabJackConfig()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setLabJackConfig(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
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

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setValveState(valveName, state));
    }



    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestVacuumPressure()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getVacuumPressure( m_settings.hardware.vacuum_guage.value("connection").toString(),
                                                                     m_settings.hardware.vacuum_guage.value("slope").toDouble(),
                                                                     m_settings.hardware.vacuum_guage.value("offset").toDouble()));
    }














    /**
     * Request current flow rate
     *
     * @brief Testing::requestActualFlowControllerFlowRate
     */
    void Testing::requestActualFlowControllerFlowRate(QString controller)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerFlowRate(controller));
    }

    /**
     * Request current flow rate
     *
     * @brief Testing::requestFlowControllerTemperature
     */
    void Testing::requestFlowControllerTemperature(QString controller)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerTemperature(controller));
    }

    /**
     * Request a valve override on the flow controllers
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerValveOverride(QString controller, int state)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride(controller, state));
    }

    /**
     * Request flow rate
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerFlowRate(QString controller, int flowrate)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerFlowRate(controller, flowrate));
    }

    /**
     * Request flow rate soft start enable
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerSoftStart(QString controller, int state)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerSoftStart(controller, state));
    }

    /**
     * Request flow rate soft start in seconds
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerSoftStartTime(QString controller, int seconds)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerSoftStartTime(controller, seconds));
    }


    void Testing::requestSetFlowControllerSourceControl(QString controller, int source)
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerSourceControl(controller, source));
    }








    void Testing::requestHighPressure(QString pressure)
    {
        qDebug() << "Running high pressure";
    }

    void Testing::requestExhuast(int filterType, int frequency)
    {
        qDebug() << "Running exhuast";
    }

    void Testing::requestOutputPressure(int frequency)
    {
        qDebug() << "Running output pressure";
    }

    void Testing::requestPurgeSystemMethodOne(bool outputValve, int cycles, QString pressure)
    {
        qDebug() << "Running purging with method one";
    }

    void Testing::requestPurgeSystemMethodTwo(int minutes, QString pressure)
    {
        qDebug() << "Running purging with method two";
    }

    void Testing::requestValveStateSafe(int port, bool state)
    {
        qDebug() << "Running valve safe method";
    }

    void Testing::requestVacDown(int mintues, bool turbo)
    {
        qDebug() << "Running vac down method";
    }

}}

