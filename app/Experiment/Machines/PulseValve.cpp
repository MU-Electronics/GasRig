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

            // Misc
        ,   sml_cycle(&machine)



            // Valves
        ,   sml_closeOutput_1(&machine)
        ,   sml_openOutput_1(&machine)
        ,   sml_closeSlowExhuastPath_1(&machine)
        ,   sml_openSlowExhuastPath_1(&machine)
        ,   sml_closeExhuast_1(&machine)
        ,   sml_openExhuast_1(&machine)
        ,   sml_closeFastExhuastPath_1(&machine)
        ,   sml_openFastExhuastPath_1(&machine)
        ,   sml_closeVacuumIn_1(&machine)
        ,   sml_openVacuumIn_1(&machine)
        ,   sml_closeVacuumOut_1(&machine)
        ,   sml_openVacuumOut_1(&machine)
        ,   sml_closeHighPressureInput_1(&machine)
        ,   sml_openHighPressureInput_1(&machine)
        ,   sml_closeFlowController_1(&machine)
        ,   sml_openFlowController_1(&machine)
        ,   sml_closeHighPressureNitrogen_1(&machine)
        ,   sml_openHighPressureNitrogen_1(&machine)



            // Timer states
        ,   sml_startPulseValveOpenTimer(&machine)
        ,   sml_startPulseValveCloseTimer(&machine)



            // Timers
        ,   t_pulseValveOpen(parent)
        ,   t_pulseValveClose(parent)
    {
        //connect(&sml_cycle, &QState::entered, this, &PulseValve::cycle);
        connect(state("cycle"), &QState::entered, this, &PulseValve::cycle);

        // Valves
        connect(&sml_closeOutput_1, &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(&sml_openOutput_1, &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(&sml_closeSlowExhuastPath_1, &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(&sml_openSlowExhuastPath_1, &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(&sml_closeExhuast_1, &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(&sml_openExhuast_1, &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(&sml_closeFastExhuastPath_1, &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(&sml_openFastExhuastPath_1, &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(&sml_closeVacuumIn_1, &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(&sml_openVacuumIn_1, &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(&sml_closeVacuumOut_1, &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);
        connect(&sml_openVacuumOut_1, &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);
        connect(&sml_openHighPressureInput_1, &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(&sml_closeFlowController_1, &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(&sml_closeHighPressureNitrogen_1, &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(&sml_openFlowController_1, &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(&sml_openHighPressureNitrogen_1, &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);



        // Valve timers
        connect(&sml_startPulseValveOpenTimer, &QState::entered, this, &PulseValve::startPulseValveOpenTimer);
        connect(&sml_startPulseValveCloseTimer, &QState::entered, this, &PulseValve::startPulseValveCloseTimer);

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
        valvePointerOpen->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_startPulseValveOpenTimer);

        // Start the valve close timer
        sml_startPulseValveOpenTimer.addTransition(&t_pulseValveOpen, &QTimer::timeout, valvePointerClose);



        // Close the valve
        valvePointerClose->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_startPulseValveCloseTimer);

        // Start the valve open timer
        sml_startPulseValveCloseTimer.addTransition(&t_pulseValveClose, &QTimer::timeout, state("cycle"));



        // Do more cycles need to be ran
        state("cycle")->addTransition(this, &PulseValve::emit_continueCycling, valvePointerOpen);
        state("cycle")->addTransition(this, &PulseValve::emit_stopCycling, &sm_stop);
        //sml_cycle.addTransition(this, &PulseValve::emit_continueCycling, valvePointerOpen);
        //sml_cycle.addTransition(this, &PulseValve::emit_stopCycling, &sm_stop);
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
            valvePointerClose = &sml_closeOutput_1;
            valvePointerOpen = &sml_openOutput_1;
        }
        else if(valve == 2)
        {
            valvePointerClose = &sml_closeSlowExhuastPath_1;
            valvePointerOpen = &sml_openSlowExhuastPath_1;
        }
        else if(valve == 3)
        {
            valvePointerClose = &sml_closeExhuast_1;
            valvePointerOpen = &sml_openExhuast_1;
        }
        else if(valve == 4)
        {
            valvePointerClose = &sml_closeFastExhuastPath_1;
            valvePointerOpen = &sml_openFastExhuastPath_1;
        }
        else if(valve == 5)
        {
            valvePointerClose = &sml_closeVacuumIn_1;
            valvePointerOpen = &sml_openVacuumIn_1;
        }
        else if(valve == 6)
        {
            valvePointerClose = &sml_closeVacuumOut_1;
            valvePointerOpen = &sml_openVacuumOut_1;
        }
        else if(valve == 7)
        {
            valvePointerClose = &sml_closeHighPressureInput_1;
            valvePointerOpen = &sml_openHighPressureInput_1;
        }
        else if(valve == 8)
        {
            valvePointerClose = &sml_closeFlowController_1;
            valvePointerOpen = &sml_openFlowController_1;
        }
        else if(valve == 9)
        {
            valvePointerClose = &sml_closeHighPressureNitrogen_1;
            valvePointerOpen = &sml_openHighPressureNitrogen_1;
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







