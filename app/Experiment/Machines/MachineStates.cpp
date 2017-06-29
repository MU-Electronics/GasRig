#include "MachineStates.h"

// Include extenral deps
#include <QObject>
#include <QDebug>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines
{

    MachineStates::MachineStates(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety)
        :   QObject(parent)
        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   machine(parent)
        ,   m_commandConstructor(*new Hardware::CommandConstructor)

        ,   t_vacMonitor(parent)
        ,   t_vacTime(parent)

            // Check the system pressure
        ,   sm_systemPressure(&machine)
        ,   sm_validatePressureForVacuum(&machine)

            // Close the high pressure valve
        ,   sm_closeHighPressureInput(&machine)
        ,   sm_validateCloseHighPressureInput(&machine)
        ,   sm_closeHighPressureNitrogen(&machine)
        ,   sm_validateCloseHighPressureNitrogen(&machine)
        ,   sm_closeFlowController(&machine)
        ,   sm_validateCloseFlowController(&machine)

    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &MachineStates::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();

        // Confiugre the timers of the states
        setupTimers();
    }


    /**
     * Connect states to their matching methods
     *      NOTE: Methods are virtual so can be overloaded in machine classes
     *
     * @brief MachineStates::connectStatesToMethods
     */
    void MachineStates::connectStatesToMethods()
    {
        // Pressure related states
        connect(&sm_systemPressure, &QState::entered, this, &MachineStates::systemPressure);
        connect(&sm_validatePressureForVacuum, &CommandValidatorState::entered, this, &MachineStates::validatePressureForVacuum);

        // Valve related states
        connect(&sm_closeHighPressureInput, &QState::entered, this, &MachineStates::closeHighPressureInput);
        connect(&sm_validateCloseHighPressureInput, &CommandValidatorState::entered, this, &MachineStates::validateCloseHighPressureInput);
    }


    /**
     * Setup timers to the correct configuration for the states that use them
     *
     * @brief MachineStates::setupTimers
     */
    void MachineStates::setupTimers()
    {
        // The time the vacumm should vac down to
        t_vacTime.setSingleShot(true);
    }





    /**
     * Request closing of the high pressure input valve
     *
     * @brief MachineStates::systemPressure
     */
    void MachineStates::closeHighPressureInput()
    {
        // Find the correct valve name
        QString valveName = m_settings.hardware.valve_connections.value("7").toString();

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setValveState(valveName, false));
    }

    /**
     * Request closing of the high pressure input valve
     *
     * @brief MachineStates::systemPressure
     */
    void MachineStates::validateCloseHighPressureInput()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        qDebug() << package;
    }












    /**
     * Request a reading of the system pressure
     *
     * @brief MachineStates::systemPressure
     */
    void MachineStates::systemPressure()
    {
        emit hardwareRequest(m_commandConstructor.getPressureReading(1));
    }


    /**
     * Validate a reading of the system pressure
     *
     * @brief MachineStates::validateSystemPressure
     */
    void MachineStates::validatePressureForVacuum()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Get the pressure
        float pressure = package.value("pressure").toFloat();

        // Get the max pressure allowed
        float maxPressure = m_settings.safety.vacuum.value("vacuum_from").toFloat();

        // Check the pressure is safe to vac down
        if(pressure < maxPressure)
        {
            // Store the stage info
            QVariantMap data;
            data.insert("pressure", pressure);

            // Emit safe to proceed
            emit emit_validationSuccess(data);
        }
        else
        {
            // Store the error
            QVariantMap error;
            error.insert("message", "pressure in system is too high for the vac station; Exhuast the system first.");
            error.insert("system_pressure", pressure);
            error.insert("system_pressure_max", maxPressure);

            // Emit not safe to proceed
            emit emit_validationFailed(error);
        }
    }


}}}
