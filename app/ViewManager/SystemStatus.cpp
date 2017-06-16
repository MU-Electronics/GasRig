#include "SystemStatus.h"
#include <QVariantMap>
#include <QString>
#include <QtGlobal>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace ViewManager
{
    SystemStatus::SystemStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
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

        // Default vacuum states
        m_vacuumState.insert("backing_pump", 0);
        m_vacuumState.insert("turbo_pump", 0);
        m_vacuumState.insert("backing_pump_mode", 0);
        m_vacuumState.insert("gas_type_mode", 1);

        // Default flow controller values
        m_flowController.insert("controller_1_override", 0);
        m_flowController.insert("controller_2_override", 0);
        m_flowController.insert("controller_1_set_flowrate", 0);
        m_flowController.insert("controller_2_set_flowrate", 0);
    }

    void SystemStatus::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Labjack statuses
        connect(&hardware, &Hardware::Access::emit_setDigitalPort, this, &SystemStatus::receiveValveStatus);
        //connect(&hardware, &Hardware::Access::emit_setAnaloguePort, this, &ConnectionStatus::receiveVacuumReading);

        // Vacuum status
        connect(&hardware, &Hardware::Access::emit_setPumpingState, this, &SystemStatus::receiveVacSetPump);
        connect(&hardware, &Hardware::Access::emit_setTurboPumpState, this, &SystemStatus::receiveVacSetTurbo);
        connect(&hardware, &Hardware::Access::emit_setGasMode, this, &SystemStatus::receiveVacSetGasMode);
        connect(&hardware, &Hardware::Access::emit_setBackingPumpMode, this, &SystemStatus::receiveVacSetPumpMode);

        // Flow controller statuses
        connect(&hardware, &Hardware::Access::emit_setFlowControllerValveOverride, this, &SystemStatus::receiveSetFlowControllerValveOverride);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerFlowRate, this, &SystemStatus::receiveSetFlowControllerFlowRate);
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
     * Debug method for setting gas mode
     *
     * @brief ConnectionStatus::receiveVacSetGasMode
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
     * Debug method for setting gas mode
     *
     * @brief ConnectionStatus::receiveVacSetGasMode
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
     * Debug method for setting gas mode
     *
     * @brief ConnectionStatus::receiveVacSetGasMode
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
     * Debug method for setting gas mode
     *
     * @brief ConnectionStatus::receiveVacSetGasMode
     * @param command
     */
    void SystemStatus::receiveVacSetTurbo(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["turbo_pump"] = command.value("state").toInt();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
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

}}


