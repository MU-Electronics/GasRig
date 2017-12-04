#include "MachineStates.h"

#include <cmath>

// Include extenral deps
#include <QObject>
#include <QDebug>
#include <QVariantMap>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    MachineStates::MachineStates(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   machine(parent)
        ,   m_commandConstructor(*new Hardware::CommandConstructor)

            // Valve states
        ,   m_valves(new Valves(parent, settings, hardware, safety, machine, params, m_commandConstructor))
        ,   m_vacuum(new Vacuum(parent, settings, hardware, safety, machine, params, m_commandConstructor))
        ,   m_pressure(new Pressure(parent, settings, hardware, safety, machine, params, m_commandConstructor))
        ,   m_flow(new Flow(parent, settings, hardware, safety, machine, params, m_commandConstructor))

            // Re-implimention of stop for each machine
        ,   sm_stop(&machine)
        ,   sm_stopAsFailed(&machine)

        ,   ssm_stop(&subMachineShutdown)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &MachineStates::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    MachineStates::~MachineStates()
    {
        // Delete the external state pointers
        delete m_valves;
        delete m_vacuum;
        delete m_pressure;
        delete m_flow;
    }



    /**
     * Connect states to their matching methods
     *      NOTE: Methods are virtual so can be overloaded in machine classes
     *
     * @brief MachineStates::connectStatesToMethods
     */
    void MachineStates::connectStatesToMethods()
    {
        // Tell the machine to stop becuase of success or error
        connect(&sm_stop, &QState::entered, this, &MachineStates::stopMachineWithoutError);
        connect(&sm_stopAsFailed, &QState::entered, this, &MachineStates::stopMachineWithError);

        // When machine has stopped running the stopped method in each machine
        connect(&machine, &QStateMachine::stopped, this, &MachineStates::emitStopped);

        // Shut down sub state machines
        connect(&ssm_stop, &QState::entered, this, &MachineStates::stopShutDownSubMachine);
        connect(&subMachineShutdown, &QStateMachine::stopped, this, &MachineStates::afterSubMachinesStopped);

    }



    /**
     * Returns the valve states instance
     *
     * @brief MachineStates::valves
     * @return
     */
    Valves* MachineStates::valves()
    {
        return m_valves;
    }

    /**
     * Returns the vacuum states instance
     *
     * @brief MachineStates::vacuum
     * @return
     */
    Vacuum* MachineStates::vacuum()
    {
        return m_vacuum;
    }


    /**
     * Returns the pressure states instance
     *
     * @brief MachineStates::pressure
     * @return
     */
    Pressure* MachineStates::pressure()
    {
        return m_pressure;
    }


    /**
     * Returns the vacuum states instance
     *
     * @brief MachineStates::flow
     * @return
     */
    Flow* MachineStates::flow()
    {
        return m_flow;
    }


    /**
     * Runs after sub machines have been stopped if there are any
     *
     * @brief MachineStates::afterSubMachinesStopped
     */
    void MachineStates::afterSubMachinesStopped()
    {
        if(error)
        {
            emit emit_machineFailed(errorDetails);
        }
        else
        {
            emit emit_machineFinished(errorDetails);
        }
    }


    /**
     * Shuts down the sub state machines shutdown machine
     *
     * @brief MachineStates::stopShutDownSubMachine
     */
    void MachineStates::stopShutDownSubMachine()
    {
        subMachineShutdown.stop();
    }


    /**
     * Emits a signal when the machine is stopped
     *
     * @brief MachineStates::emitStopped
     */
    void MachineStates::emitStopped()
    {
        // Trigger shutdown sub machines
        if(subMachines)
        {
            // Build the shutdown machine
            buildSubMachineShutDown();

            // Run the sub machine shutdown state machine
            subMachineShutdown.start();
        }

        // Stop main machine
        if(error)
        {
            // Run the stopped function in the state machine
            stopped();

            // Tell every we have stopped becuase of an error
            if(!subMachines)
                emit emit_machineFailed(errorDetails);
        }
        else
        {
            // Run the stopped function in the state machine
            stopped();

            // Tell every we have stopped becuase the machine finished it's task
            if(!subMachines)
                emit emit_machineFinished(errorDetails);
        }
    }


    /**
     * Stops and resets the machine with no errors flagged
     *
     * @brief MachineStates::stopMachineWithoutError
     */
    void MachineStates::stopMachineWithoutError()
    {
        // There was no error
        error = false;

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();
    }


    /**
     * Stop and resets the machine with errors flagged
     *
     * @brief MachineStates::stopMachineWithError
     */
    void MachineStates::stopMachineWithError()
    {
        // There was an error
        error = true;

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();
    }


    /**
     * This helper method removes all transistions from all the states
     *
     * @brief MachineStates::removeAllTransitions
     */
    void MachineStates::removeAllTransitions()
    {
        // Get all states from machine and loop through them
        QList<QState *> allStates = machine.findChildren<QState *>();
        while(!allStates.isEmpty())
        {
            // Get the current state
            QState *state = allStates.takeFirst();

            // Get all the transistions for this state
            QList<QAbstractTransition *> transitions = state->transitions();

            // Remove all the transisition from the states
            while (!transitions.isEmpty())
                state->removeTransition(transitions.takeFirst());
        }
    }


    /**
     * Allows override of params in state machine
     *
     * @brief MachineStates::paramsOverride
     */
    void MachineStates::paramsOverride(QVariantMap override)
    {
        for (auto i = override.constBegin(); i != override.constEnd(); ++i)
        {
             params.insert(i.key(), i.value());
        }
    }

}}}}
