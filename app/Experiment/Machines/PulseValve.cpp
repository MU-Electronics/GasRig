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
        connect(state("closeOutput", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("openOutput", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(validator("closeOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("openOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);

        connect(state("closeSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("openSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(validator("closeSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("openSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);

        connect(state("closeExhuast", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("openExhuast", true), &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(validator("closeExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("openExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);

        connect(state("closeFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("openFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(validator("closeFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("openFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);

        connect(state("closeVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("openVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(validator("closeVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("openVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);

        connect(state("closeVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);
        connect(state("openVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);
        connect(validator("closeVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);
        connect(validator("openVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);

        connect(state("openHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(state("closeHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(validator("closeHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("openHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);


        connect(state("closeFlowController", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("openFlowController", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(validator("closeFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("openFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);

        connect(state("openHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(state("closeHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(validator("closeHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("openHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);




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
                &ssm_stopAsFailed
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
        transitionsBuilder()->openValve(valvePointerOpen, valvePointerOpenValidator, state("startPulseValveOpenTimer", true), &sm_stopAsFailed);

        // Start the valve close timer
        state("startPulseValveOpenTimer", true)->addTransition(&t_pulseValveOpen, &QTimer::timeout, valvePointerClose);

        // Close the valve
        transitionsBuilder()->closeValve(valvePointerClose, valvePointerCloseValidator, state("startPulseValveCloseTimer", true), &sm_stopAsFailed);

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
            valvePointerClose = state("closeOutput", true);
            valvePointerOpen = state("openOutput", true);
            valvePointerOpenValidator = validator("openOutput", true);
            valvePointerCloseValidator = validator("closeOutput", true);
        }
        else if(valve == 2)
        {
            valvePointerClose = state("closeSlowExhuastPath", true);
            valvePointerOpen = state("openSlowExhuastPath", true);
            valvePointerOpenValidator = validator("openSlowExhuastPath", true);
            valvePointerCloseValidator = validator("closeSlowExhuastPath", true);
        }
        else if(valve == 3)
        {
            valvePointerClose = state("closeExhuast", true);
            valvePointerOpen = state("openExhuast", true);
            valvePointerOpenValidator = validator("openExhuast", true);
            valvePointerCloseValidator = validator("closeExhuast", true);
        }
        else if(valve == 4)
        {
            valvePointerClose = state("closeFastExhuastPath", true);
            valvePointerOpen = state("openFastExhuastPath", true);
            valvePointerOpenValidator = validator("openFastExhuastPath", true);
            valvePointerCloseValidator = validator("closeFastExhuastPath", true);
        }
        else if(valve == 5)
        {
            valvePointerClose = state("closeVacuumIn", true);
            valvePointerOpen = state("openVacuumIn", true);
            valvePointerOpenValidator = validator("openVacuumIn", true);
            valvePointerCloseValidator = validator("closeVacuumIn", true);
        }
        else if(valve == 6)
        {
            valvePointerClose = state("closeVacuumOut", true);
            valvePointerOpen = state("openVacuumOut", true);
            valvePointerOpenValidator = validator("openVacuumOut", true);
            valvePointerCloseValidator = validator("closeVacuumOut", true);
        }
        else if(valve == 7)
        {
            valvePointerClose = state("closeHighPressureInput", true);
            valvePointerOpen = state("openHighPressureInput", true);
            valvePointerOpenValidator = validator("openHighPressureInput", true);
            valvePointerCloseValidator = validator("closeHighPressureInput", true);
        }
        else if(valve == 8)
        {
            valvePointerClose = state("closeFlowController", true);
            valvePointerOpen = state("openFlowController", true);
            valvePointerOpenValidator = validator("openFlowController", true);
            valvePointerCloseValidator = validator("closeFlowController", true);
        }
        else if(valve == 9)
        {
            valvePointerClose = state("closeHighPressureNitrogen", true);
            valvePointerOpen = state("openHighPressureNitrogen", true);
            valvePointerOpenValidator = validator("openHighPressureNitrogen", true);
            valvePointerCloseValidator = validator("closeHighPressureNitrogen", true);
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







