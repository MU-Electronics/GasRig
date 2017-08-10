#include "MachineStates.h"

#include <cmath>

// Include extenral deps
#include <QObject>
#include <QDebug>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace States
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

            // Timers for states
        ,   t_vacPressureMonitor(parent)
        ,   t_pressureMonitor(parent)
        ,   t_flowControllerFlowMonitor(parent)
        ,   t_flowControllerTemperatureMonitor(parent)
        ,   t_vacStationTemperatureMonitor(parent)
        ,   t_turboSpeedMonitor(parent)
        ,   t_vacTime(parent)

            // Timers
        ,   sm_initalWait(&machine)
        ,   sm_timerWait(&machine)
        ,   sm_startVacuumPressureMonitor(&machine)
        ,   sm_startPressureMonitor(&machine)
        ,   sm_startFlowControllerFlowMonitor(&machine)
        ,   sm_startFlowControllerTemperatureMonitor(&machine)
        ,   sm_startVacStationTemperatureMonitor(&machine)
        ,   sm_startTurboSpeedMonitor(&machine)
        ,   sm_startVacuumTimer(&machine)

            // Finishing sequence
        ,   sm_finishVacSession(&machine)

            // Re-implimention of stop for each machine
        ,   sm_stop(&machine)
        ,   sm_stopAsFailed(&machine)
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
        // Link the timer states
        connect(&sm_initalWait, &QState::entered, this, &MachineStates::timerWait);
        connect(&sm_timerWait, &QState::entered, this, &MachineStates::timerWait);
        connect(&sm_startVacuumPressureMonitor, &QState::entered, this, &MachineStates::startVacuumPressureMonitor);
        connect(&sm_startPressureMonitor, &QState::entered, this, &MachineStates::startPressureMonitor);
        connect(&sm_startFlowControllerFlowMonitor, &QState::entered, this, &MachineStates::startFlowControllerFlowMonitor);
        connect(&sm_startVacuumTimer, &QState::entered, this, &MachineStates::startVacuumTimer);

        connect(&sm_startFlowControllerTemperatureMonitor, &QState::entered, this, &MachineStates::startFlowControllerTemperatureMonitor);
        connect(&sm_startVacStationTemperatureMonitor, &QState::entered, this, &MachineStates::startVacStationTemperatureMonitor);
        connect(&sm_startTurboSpeedMonitor, &QState::entered, this, &MachineStates::startTurboSpeedMonitor);

        // Finishing sequence
        connect(&sm_finishVacSession, &QState::entered, this, &MachineStates::finishVacSession);

        // Re-implimention of stop for each machine
        connect(&sm_stop, &QState::entered, this, &MachineStates::stop);
        connect(&sm_stopAsFailed, &QState::entered, this, &MachineStates::stopAsFailed);
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
     * Empty state to wait for timer events
     *
     * @brief MachineStates::timerWait
     */
    void MachineStates::timerWait()
    {
    }



    /**
     * The timer for how long to vac down for
     *
     * @brief MachineStates::startVacuumTimer
     */
    void MachineStates::startVacuumTimer()
    {
        if(!t_vacTime.isActive())
        {
            // Setup timer
            t_vacTime.setSingleShot(true);
            t_vacTime.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the vac down timer
     *
     * @brief MachineStates::stopVacuumTimer
     */
    void MachineStates::stopVacuumTimer()
    {
        t_vacTime.stop();
    }




    /**
     * Timer to use to trigger reading the vacuum sensor
     *
     * @brief MachineStates::startVacuumPressureMonitor
     */
    void MachineStates::startVacuumPressureMonitor()
    {
        if(!t_vacPressureMonitor.isActive())
        {
            // Setup timer
            t_vacPressureMonitor.setSingleShot(false);
            t_vacPressureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vacuum sensor
     *
     * @brief MachineStates::stopVacuumPressureMonitor
     */
    void MachineStates::stopVacuumPressureMonitor()
    {
        t_vacPressureMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the pressure sensor
     *
     * @brief MachineStates::startPressureMonitor
     */
    void MachineStates::startPressureMonitor()
    {
        if(!t_pressureMonitor.isActive())
        {
            // Setup timer
            t_pressureMonitor.setSingleShot(false);
            t_pressureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the pressure sensor
     *
     * @brief MachineStates::stopPressureMonitor
     */
    void MachineStates::stopPressureMonitor()
    {
        t_pressureMonitor.stop();
    }



    /**
     * Timer to use to trigger reading the flow controller flow sensor
     *
     * @brief MachineStates::startPressureMonitor
     */
    void MachineStates::startFlowControllerFlowMonitor()
    {
        if(!t_flowControllerFlowMonitor.isActive())
        {
            // Setup timer
            t_flowControllerFlowMonitor.setSingleShot(false);
            t_flowControllerFlowMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the flow controller flow sensor
     *
     * @brief MachineStates::stopPressureMonitor
     */
    void MachineStates::stopFlowControllerFlowMonitor()
    {
        t_flowControllerFlowMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the vac station turbo speed sensor
     *
     * @brief MachineStates::startTurboSpeedMonitor
     */
    void MachineStates::startTurboSpeedMonitor()
    {
        if(!t_turboSpeedMonitor.isActive())
        {
            // Setup timer
            t_turboSpeedMonitor.setSingleShot(false);
            t_turboSpeedMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vac station turbo speed sensor
     *
     * @brief MachineStates::stopTurboSpeedMonitor
     */
    void MachineStates::stopTurboSpeedMonitor()
    {
        t_turboSpeedMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the vac station temperature sensor
     *
     * @brief MachineStates::startVacStationTemperatureMonitor
     */
    void MachineStates::startVacStationTemperatureMonitor()
    {
        if(!t_vacStationTemperatureMonitor.isActive())
        {
            // Setup timer
            t_vacStationTemperatureMonitor.setSingleShot(false);
            t_vacStationTemperatureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vac station temperature sensor
     *
     * @brief MachineStates::stopVacStationTemperatureMonitor
     */
    void MachineStates::stopVacStationTemperatureMonitor()
    {
        t_vacStationTemperatureMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the flow controller temperature sensor
     *
     * @brief MachineStates::startFlowControllerTemperatureMonitor
     */
    void MachineStates::startFlowControllerTemperatureMonitor()
    {
        if(!t_flowControllerTemperatureMonitor.isActive())
        {
            // Setup timer
            t_flowControllerTemperatureMonitor.setSingleShot(false);
            t_flowControllerTemperatureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the flow controller temperature sensor
     *
     * @brief MachineStates::stopFlowControllerTemperatureMonitor
     */
    void MachineStates::stopFlowControllerTemperatureMonitor()
    {
        t_flowControllerTemperatureMonitor.stop();
    }















    void MachineStates::finishVacSession()
    {
        // Turn off vacuum
       m_vacuum->disableTurboPump();
       m_vacuum->disableBackingPump();

       // Close valves
       m_valves->closeOutput();
       m_valves->closeVacuumOut();
       m_valves->closeFastExhuastPath();
       m_valves->closeSlowExhuastPath();
       m_valves->closeVacuumIn();

       // Stop timers
       stopVacuumPressureMonitor();
       stopVacuumTimer();
    }

}}}}
