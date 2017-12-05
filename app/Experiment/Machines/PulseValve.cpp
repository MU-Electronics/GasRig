#include "PulseValve.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "Functions/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    PulseValve::PulseValve(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_pulseValveOpen(parent)
        ,   t_pulseValveClose(parent)
    {
        //connect(&sml_cycle, &QState::entered, this, &PulseValve::cycle);
        connect(state("cycle", true), &QState::entered, this, &PulseValve::cycle);


        // Valves
        connect(state("closeOutput_1", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("openOutput_1", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);

        connect(state("closeSlowExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("openSlowExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);

        connect(state("closeExhuast_1", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("openExhuast_1", true), &QState::entered, this->valves(), &Functions::Valves::openExhuast);

        connect(state("closeFastExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("openFastExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);

        connect(state("closeVacuumIn_1", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("openVacuumIn_1", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);

        connect(state("closeVacuumOut_1", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);
        connect(state("openVacuumOut_1", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);

        connect(state("openHighPressureInput_1", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(state("closeHighPressureInput_1", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);

        connect(state("closeFlowController_1", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("openFlowController_1", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);

        connect(state("openHighPressureNitrogen_1", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(state("closeHighPressureNitrogen_1", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);


        // Valve timers
        connect(state("startPulseValveOpenTimer", true), &QState::entered, this, &PulseValve::startPulseValveOpenTimer);
        connect(state("startPulseValveCloseTimer", true), &QState::entered, this, &PulseValve::startPulseValveCloseTimer);

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
        t_pulseValveOpen.setInterval(timeOpen);

        // To delete soon
        t_pulseValveClose.setInterval(timeClosed);
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
     * @brief PulseValve::stopped
     */
    void PulseValve::stopped()
    {
        // Make sure valve closed
        closeValve(params.value("valve").toInt());
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
        valvePointerOpen->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, state("startPulseValveOpenTimer", true));

        // Start the valve close timer
        state("startPulseValveOpenTimer", true)->addTransition(&t_pulseValveOpen, &QTimer::timeout, valvePointerClose);


        // Close the valve
        valvePointerClose->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, state("startPulseValveCloseTimer", true));

        // Start the valve open timer
        state("startPulseValveCloseTimer", true)->addTransition(&t_pulseValveClose, &QTimer::timeout, state("cycle", true));


        // Do more cycles need to be ran
        state("cycle", true)->addTransition(this, &PulseValve::emit_continueCycling, valvePointerOpen);
        state("cycle", true)->addTransition(this, &PulseValve::emit_stopCycling, &sm_stop);
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
            valvePointerClose = state("closeOutput_1", true);
            valvePointerOpen = state("openOutput_1", true);
        }
        else if(valve == 2)
        {
            valvePointerClose = state("closeSlowExhuastPath_1", true);
            valvePointerOpen = state("openSlowExhuastPath_1", true);
        }
        else if(valve == 3)
        {
            valvePointerClose = state("closeExhuast_1", true);
            valvePointerOpen = state("openExhuast_1", true);
        }
        else if(valve == 4)
        {
            valvePointerClose = state("closeFastExhuastPath_1", true);
            valvePointerOpen = state("openFastExhuastPath_1", true);
        }
        else if(valve == 5)
        {
            valvePointerClose = state("closeVacuumIn_1", true);
            valvePointerOpen = state("openVacuumIn_1", true);
        }
        else if(valve == 6)
        {
            valvePointerClose = state("closeVacuumOut_1", true);
            valvePointerOpen = state("openVacuumOut_1", true);
        }
        else if(valve == 7)
        {
            valvePointerClose = state("closeHighPressureInput_1", true);
            valvePointerOpen = state("openHighPressureInput_1", true);
        }
        else if(valve == 8)
        {
            valvePointerClose = state("closeFlowController_1", true);
            valvePointerOpen = state("openFlowController_1", true);
        }
        else if(valve == 9)
        {
            valvePointerClose = state("closeHighPressureNitrogen_1", true);
            valvePointerOpen = state("openHighPressureNitrogen_1", true);
        }
    }

    /**
     * The timer for how long to vac down for
     *
     * @brief PulseValve::startPulseValveOpenTimer
     */
    void PulseValve::startPulseValveOpenTimer()
    {
        // Setup timer
        t_pulseValveOpen.setSingleShot(true);
        t_pulseValveOpen.start();
    }

    /**
     * Stop the vac down timer
     *
     * @brief PulseValve::stopPulseValveOpenTimer
     */
    void PulseValve::stopPulseValveOpenTimer()
    {
        t_pulseValveOpen.stop();
    }

    /**
     * The timer for how long to vac down for
     *
     * @brief PulseValve::startPulseValveOpenTimer
     */
    void PulseValve::startPulseValveCloseTimer()
    {
        // Setup timer
        t_pulseValveClose.setSingleShot(true);
        t_pulseValveClose.start();

    }

    /**
     * Stop the vac down timer
     *
     * @brief PulseValve::stopPulseValveOpenTimer
     */
    void PulseValve::stopPulseValveCloseTimer()
    {
        t_pulseValveClose.stop();
    }

}}}







