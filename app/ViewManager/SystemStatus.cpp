#include "SystemStatus.h"

#include <cmath>

#include <QVariantMap>
#include <QString>
#include <QtGlobal>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Include command constructor
#include "../Hardware/CommandConstructor.h"

namespace App { namespace ViewManager
{
    SystemStatus::SystemStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings),
          m_commandConstructor(*new Hardware::CommandConstructor)
    {
        // Default valves values
        m_valveState.insert("1", 0);
        m_valveState.insert("2", 0);
        m_valveState.insert("3", 0);
        m_valveState.insert("4", 0);
        m_valveState.insert("5", 0);
        m_valveState.insert("6", 0);
        m_valveState.insert("7", 0);
        m_valveState.insert("8", 0);
        m_valveState.insert("9", 0);

        // Pressure reading
        m_pressureSensor.insert("pressure", 50);

        // Default vacuum states
        m_vacuumState.insert("backing_pump", 0);
        m_vacuumState.insert("turbo_pump", 0);
        m_vacuumState.insert("backing_pump_mode", 0);
        m_vacuumState.insert("gas_type_mode", 1);
        m_vacuumState.insert("vacuum", 0);

        // Default flow controller values
        m_flowController.insert("controller_1_set_flowrate", 0);
        m_flowController.insert("controller_2_set_flowrate", 0);
        m_flowController.insert("controller_1_flow", 0);
        m_flowController.insert("controller_2_flow", 0);
        m_flowController.insert("controller_1_override", 0);
        m_flowController.insert("controller_2_override", 0);
        m_flowController.insert("controller_1_source", 0);
        m_flowController.insert("controller_2_source", 0);
        m_flowController.insert("controller_1_softstart", 0);
        m_flowController.insert("controller_2_softstart", 0);
        m_flowController.insert("controller_1_softstart_time", 0);
        m_flowController.insert("controller_2_softstart_time", 0);

    }

    void SystemStatus::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &SystemStatus::emit_hardwareRequest, &hardware, &Hardware::Access::hardwareAccess);

        // Labjack statuses
        connect(&hardware, &Hardware::Access::emit_setDigitalPort, this, &SystemStatus::receiveValveStatus);
        connect(&hardware, &Hardware::Access::emit_readAnaloguePort, this, &SystemStatus::receiveVacuumPressure);

        // Pressure sensor
        m_pressureSensor.insert("vacuum", 0);
        connect(&hardware, &Hardware::Access::emit_pressureSensorPressure, this, &SystemStatus::receivePressureSensorPressure);

        // Vacuum status
        connect(&hardware, &Hardware::Access::emit_setPumpingState, this, &SystemStatus::receiveVacSetPump);
        connect(&hardware, &Hardware::Access::emit_setTurboPumpState, this, &SystemStatus::receiveVacSetTurbo);
        connect(&hardware, &Hardware::Access::emit_setGasMode, this, &SystemStatus::receiveVacSetGasMode);
        connect(&hardware, &Hardware::Access::emit_setBackingPumpMode, this, &SystemStatus::receiveVacSetPumpMode);

        // Flow controller statuses
        connect(&hardware, &Hardware::Access::emit_setFlowControllerValveOverride, this, &SystemStatus::receiveSetFlowControllerValveOverride);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerFlowRate, this, &SystemStatus::receiveSetFlowControllerFlowRate);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStart, this, &SystemStatus::receiveSetFlowControllerSoftStart);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStartTime, this, &SystemStatus::receiveSetFlowControllerSoftStartTime);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSourceControl, this, &SystemStatus::receiveSetFlowControllerSourceControl);

        // Get current values for pressure and vacuum
        emit emit_hardwareRequest(m_commandConstructor.getPressureReading(1));
        emit emit_hardwareRequest(m_commandConstructor.getVacuumPressure( m_settings.hardware.vacuum_guage.value("connection").toString(),
                                                                     m_settings.hardware.vacuum_guage.value("slope").toDouble(),
                                                                     m_settings.hardware.vacuum_guage.value("offset").toDouble()));
        // Ensure all valves are closed
        for(int i=1; i<=9; i++)
            emit emit_hardwareRequest(m_commandConstructor.setValveState(m_settings.hardware.valve_connections.value(QString::number(i)).toString(), false));

        // Get the currently backing pump and gas type mode from vac station


        // Get the flow controllers override values

        // Get the flow controllers flow rates

        // Get the flow controllers soft start enabled / disabled

        // Get the flow controllers soft start times
    }


    /**
     * Update the most recent pressure reading
     *
     * @brief SystemStatus::receivePressureSensorPressure
     * @param package
     */
    void SystemStatus::receivePressureSensorPressure(QVariantMap package)
    {
        // Update the pressure @todo moving average
        m_pressureSensor["pressure"] = package["pressure"];

        // Tell the views we've updated
        emit_pressureSensorChanged(m_pressureSensor);
    }







    /**
     * Check if a digital port update is relivent to a valve port and if so update the display
     *
     * @brief ConnectionStatus::receiveValveStatus
     * @param package
     */
    void SystemStatus::receiveValveStatus(QVariantMap package)
    {
        // Get the port name
        QString portNumber  = m_settings.hardware.valve_connections.key(package.value("port").toString());

        if(!portNumber.isNull())
        {
            // Update valve value
            m_valveState[portNumber] = package.value("value").toInt();

            // Update the displays
            emit_valveStateChanged(m_valveState);
        }

    }







    /**
     * Sets the gas mode varaible and informs views
     *
     * @brief SystemStatus::receiveVacSetGasMode
     * @param command
     */
    void SystemStatus::receiveVacSetGasMode(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["gas_type_mode"] = command.value("mode").toInt();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Sets the pump mode variable and informs views
     *
     * @brief SystemStatus::receiveVacSetPumpMode
     * @param command
     */
    void SystemStatus::receiveVacSetPumpMode(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["backing_pump_mode"] = command.value("mode").toInt();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Setsn the pumping state variable and informs views
     *
     * @brief SystemStatus::receiveVacSetPump
     * @param command
     */
    void SystemStatus::receiveVacSetPump(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["backing_pump"] = command.value("state").toInt();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Sets the turbo state variable and informs views
     *
     * @brief SystemStatus::receiveVacSetTurbo
     * @param command
     */
    void SystemStatus::receiveVacSetTurbo(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["turbo_pump"] = command.value("state").toInt();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Sets the vacuum pressure variable and informs views
     *
     * @brief SystemStatus::receiveVacuumPressure
     * @param command
     */
    void SystemStatus::receiveVacuumPressure(QVariantMap command)
    {
        // If port is the same as the vacuum guage port
        if(command["port"] == m_settings.hardware.vacuum_guage.value("connection").toString())
        {
            // Update vacuum reading
            double pressure = (std::pow(10, (1.667*command.value("calibrated").toDouble()-9.333)))/100;
            m_vacuumState["vacuum"] = pressure;

            // Update the display
            emit_vacuumStateChanged(m_vacuumState);
        }
    }








    void SystemStatus::receiveSetFlowControllerValveOverride(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_override"] = command.value("override").toInt();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_override"] = command.value("override").toInt();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerFlowRate(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_set_flowrate"] = command.value("flow").toInt();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_set_flowrate"] = command.value("flow").toInt();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerSoftStart(QVariantMap command)
    {

        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_softstart"] = command.value("state").toInt();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_softstart"] = command.value("state").toInt();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerSoftStartTime(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_softstart_time"] = command.value("seconds").toInt();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_softstart_time"] = command.value("seconds").toInt();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerSourceControl(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_source"] = command.value("source").toInt();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_source"] = command.value("source").toInt();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }


}}


