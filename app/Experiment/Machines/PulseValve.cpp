#include "PulseValve.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "States/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    PulseValve::PulseValve(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
        ,   sml_cycle(&machine)
    {
        connect(&sml_cycle, &QState::entered, this, &PulseValve::cycle);
    }

    PulseValve::~PulseValve()
    {
        //delete valvePointerOpen;
        //delete valvePointerClose;
    }


    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void PulseValve::setParams(int valve, int cycles, int timeOpen, int timeClosed)
    {
        // How long the vac should run
        params.insert("valve", valve);

        // Set the valve pointers
        setValvePointer(valve);

        // How long the vac should run
        cyclesCount = cycles;

        // Setup timers
        timers()->t_pulseValveOpen.setInterval(timeOpen);

        // To delete soon
        timers()->t_pulseValveClose.setInterval(timeClosed);
    }


    /**
     * Start the state machine
     *
     * @brief PulseValve::start
     */
    void PulseValve::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief ReadFlowControllerTemperatures::start
     */
    void PulseValve::stop()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Make sure valve closed
        closeValve(params.value("valve").toInt());

        // Emit the machine is finished
        emit emit_pulseValveFinished(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief PulseValve::stopAsFailed
     */
    void PulseValve::stopAsFailed()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Make sure valve closed
        closeValve(params.value("valve").toInt());

        // Emit the machine is finished
        emit emit_pulseValveFailed(params);
    }




    /**
     * Builds the machine connections
     *
     * @brief PulseValve::buildMachine
     */
    void PulseValve::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(valvePointerOpen);

        // Open the valve
        valvePointerOpen->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &timers()->sm_startPulseValveOpenTimer);

        // Start the valve close timer
        timers()->sm_startPulseValveOpenTimer.addTransition(this->timers(), &States::Timers::emit_timerActive, &timers()->sm_initalWait);

        // Wait for close timer to timeout
        timers()->sm_initalWait.addTransition(&timers()->t_pulseValveOpen, &QTimer::timeout, valvePointerClose);


        // Close the valve
        valvePointerClose->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &timers()->sm_startPulseValveCloseTimer);

        // Start the valve open timer
        timers()->sm_startPulseValveCloseTimer.addTransition(this->timers(), &States::Timers::emit_timerActive, &timers()->sm_timerWait);

        // Wait for close timer to timeout
        timers()->sm_timerWait.addTransition(&timers()->t_pulseValveClose, &QTimer::timeout, &sml_cycle);


        // Do more cycles need to be ran
        sml_cycle.addTransition(this, &PulseValve::emit_continueCycling, valvePointerOpen);
        sml_cycle.addTransition(this, &PulseValve::emit_stopCycling, &sm_stop);
    }


    /**
     * Calacuate if we should continue pulsing
     *
     * @brief PulseValve::cycle
     */
    void PulseValve::cycle()
    {
        // Minus a cycle
        cyclesCount = cyclesCount - 1;

        // Should we conitnue pulsing
        if(cyclesCount > 0)
        {
            emit emit_continueCycling();
            return;
        }

        // No stop now
        emit emit_stopCycling();
    }



    /**
     * Close the pulse valve
     *
     * @brief PulseValve::setValvePointer
     * @param valve
     */
    void PulseValve::closeValve(int valve)
    {
        if(valve == 1)
        {
            valves()->closeOutput();
        }
        else if(valve == 2)
        {
            valves()->closeSlowExhuastPath();
        }
        else if(valve == 3)
        {
            valves()->closeExhuast();
        }
        else if(valve == 4)
        {
           valves()->closeFastExhuastPath();
        }
        else if(valve == 5)
        {
            valves()->closeVacuumIn();
        }
        else if(valve == 6)
        {
            valves()->closeVacuumOut();
        }
        else if(valve == 7)
        {
            valves()->closeHighPressureInput();
        }
        else if(valve == 8)
        {
            valves()->closeHighPressureNitrogen();
        }
        else if(valve == 9)
        {
            valves()->closeHighPressureNitrogen();
        }
    }




    /**
     * Set which valves are to be pulsed
     *
     * @brief PulseValve::setValvePointer
     * @param valve
     */
    void PulseValve::setValvePointer(int valve)
    {
        if(valve == 1)
        {
            valvePointerClose = &valves()->sm_closeOutput;
            valvePointerOpen = &valves()->sm_openOutput;
        }
        else if(valve == 2)
        {
            valvePointerClose = &valves()->sm_closeSlowExhuastPath;
            valvePointerOpen = &valves()->sm_openSlowExhuastPath;
        }
        else if(valve == 3)
        {
            valvePointerClose = &valves()->sm_closeExhuast;
            valvePointerOpen = &valves()->sm_openExhuast;
        }
        else if(valve == 4)
        {
            valvePointerClose = &valves()->sm_closeFastExhuastPath;
            valvePointerOpen = &valves()->sm_openFastExhuastPath;
        }
        else if(valve == 5)
        {
            valvePointerClose = &valves()->sm_closeVacuumIn;
            valvePointerOpen = &valves()->sm_openVacuumIn;
        }
        else if(valve == 6)
        {
            valvePointerClose = &valves()->sm_closeVacuumOut;
            valvePointerOpen = &valves()->sm_openVacuumOut;
        }
        else if(valve == 7)
        {
            valvePointerClose = &valves()->sm_closeHighPressureInput;
            valvePointerOpen = &valves()->sm_openHighPressureInput;
        }
        else if(valve == 8)
        {
            valvePointerClose = &valves()->sm_closeFlowController;
            valvePointerOpen = &valves()->sm_openFlowController;
        }
        else if(valve == 9)
        {
            valvePointerClose = &valves()->sm_closeHighPressureNitrogen;
            valvePointerOpen = &valves()->sm_openHighPressureNitrogen;
        }
    }

}}}







