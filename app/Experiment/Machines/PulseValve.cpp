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
        // We have stop state machines
        shutDownMachines = true;



        // Count cylces
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




        // SHUTDOWN STATE MACHINE: Link close valve states
        connect(state("closeHighPressureIn", false), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("closeNitrogenIn", false), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("closeFlowController", false), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("closeExhuast", false), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("closeOuput", false), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("closeSlowExhuast", false), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("closeFastExhuast", false), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeVacuumIn", false), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("closeVacuumOut", false), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // SHUTDOWN STATE MACHINE: Link close valve validator states
        connect(validator("closeHighPressureIn", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("closeNitrogenIn", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("closeFlowController", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("closeExhuast", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("closeOutput", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("closeSlowExhuast", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("closeFastExhuast", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("closeVacuumIn", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("closeVacuumOut", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

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

    }


    /**
     * Builds the shutdown state machine
     *
     * @brief PulseValve::buildShutDownMachine
     */
    void PulseValve::buildShutDownMachine()
    {
        // Where to start the machine
        shutDownMachine.setInitialState(state("closeOuput", false));

        // Close all valves
        transitionsBuilder()->closeAllValves(state("closeOuput", false),
                validator("closeOutput", false),
                state("closeSlowExhuast", false),
                validator("closeSlowExhuast", false),
                state("closeFastExhuast", false),
                validator("closeFastExhuast", false),
                state("closeVacuumIn", false),
                validator("closeVacuumIn", false),
                state("closeVacuumOut", false),
                validator("closeVacuumOut", false),
                state("closeExhuast", false),
                validator("closeExhuast", false),
                state("closeHighPressureIn", false),
                validator("closeHighPressureIn", false),
                state("closeNitrogenIn", false),
                validator("closeNitrogenIn", false),
                state("closeFlowController", false),
                validator("closeFlowController", false),
                &ssm_stop,
                &ssm_stop
        );
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
        valvePointerOpen->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, state("startPulseValveOpenTimer", true));

        // Start the valve close timer
        state("startPulseValveOpenTimer", true)->addTransition(&t_pulseValveOpen, &QTimer::timeout, valvePointerClose);


        // Close the valve
        valvePointerClose->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, state("startPulseValveCloseTimer", true));

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







