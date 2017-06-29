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
        ,   sm_systemPressure(&machine),
            sm_validate_requestSystemPressure(&machine)
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
        connect(&sm_systemPressure, &QState::entered, this, &MachineStates::systemPressure);
        connect(&sm_validate_requestSystemPressure, &CommandValidatorState::entered, this, &MachineStates::validateSystemPressure);

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
    void MachineStates::validateSystemPressure()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check the pressure is safe to vac down
        if(package.value("pressure") < "1.5")
        {
            // Emit safe to proceed
        }
        else
        {
            // Emit not safe to proceed
        }

        qDebug() << "Validating: " << package.value("pressure");
    }

}}}
