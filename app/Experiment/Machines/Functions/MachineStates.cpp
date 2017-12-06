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
        ,   shutDownMachine(parent)
        ,   m_commandConstructor(*new Hardware::CommandConstructor)

            // Valve states
        ,   m_valves(new Valves(parent, settings, hardware, safety, machine, params, m_commandConstructor))
        ,   m_vacuum(new Vacuum(parent, settings, hardware, safety, machine, params, m_commandConstructor))
        ,   m_pressure(new Pressure(parent, settings, hardware, safety, machine, params, m_commandConstructor))
        ,   m_flow(new Flow(parent, settings, hardware, safety, machine, params, m_commandConstructor))

            // Re-implimention of stop for each machine
        ,   sm_stop(&machine)
        ,   sm_stopAsFailed(&machine)

        ,   ssm_stop(&shutDownMachine)
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
        connect(this, &MachineStates::emit_machineAlreadyStopped, this, &MachineStates::emitStopped);


        // Shut down sub state machines
        connect(&ssm_stop, &QState::entered, this, &MachineStates::stopShutDownSubMachine);
        connect(&shutDownMachine, &QStateMachine::stopped, this, &MachineStates::afterSubMachinesStopped);

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
        removeAllTransitions(shutDownMachine);

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
        shutDownMachine.stop();
    }


    /**
     * Emits a signal when the machine is stopped
     *
     * @brief MachineStates::emitStopped
     */
    void MachineStates::emitStopped()
    {
        // Get all states from machine and loop through them
        removeAllTransitions(machine);

        // Run the stopped function in the state machine
        stopped();

        // Stop main machine
        if(error && !shutDownMachines)
        {
            // Tell every we have stopped becuase of an error
            emit emit_machineFailed(errorDetails);
        }
        else if(!error && !shutDownMachines)
        {
            // Tell every we have stopped becuase machine finished
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
        if(machine.isRunning())
        {
            machine.stop();
        }
        else
        {
            emit emit_machineAlreadyStopped();
        }
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
        qDebug() << "Error";

        // Stop the machine
        if(machine.isRunning())
        {
            machine.stop();
        }
        else
        {
            emit emit_machineAlreadyStopped();
        }
    }


    /**
     * This helper method removes all transistions from all the states
     *
     * @brief MachineStates::removeAllTransitions
     */
    void MachineStates::removeAllTransitions(QStateMachine &stateMachine)
    {
        // Get all states from machine and loop through them
        QList<QState *> allStates = stateMachine.findChildren<QState *>();
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


    /**
     * Returns dynamically create states and creates them if they dont exists
     *
     * @brief MachineStates::state
     * @param id
     * @param machine
     * @return
     */
    QState* MachineStates::state(QString id, bool type)
    {
        // Append stop to stop state machine
        if(!type)
            id = "stop_" + id;

        // If does not exist then make it
        if(!m_states.contains(id))
        {
            m_states.insert(id, ( new QState( (type) ? &machine : &shutDownMachine ) ));
        }

        // return the state
        return m_states.value(id);
    }


    /**
     * Returns dynamically create validators and creates them if they dont exists
     *
     * @brief MachineStates::validator
     * @param id
     * @param machine
     * @return
     */
    CommandValidatorState* MachineStates::validator(QString id, bool type)
    {
        // Append stop to stop state machine
        if(!type)
            id = "stop_" + id;

        // If does not exist then make it
        if(!m_validators.contains(id))
        {
            m_validators.insert(id, ( new CommandValidatorState( (type) ? &machine : &shutDownMachine ) ));
        }

        // return the state
        return m_validators.value(id);
    }


}}}}
