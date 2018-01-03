#include "Purge.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include valaitor state
#include "Functions/CommandValidatorState.h"

// Include possable machine states
#include "Functions/MachineStates.h"

// Required state machines
#include "Pressurise.h"
#include "Vent.h"
#include "VacDown.h"

namespace App { namespace Experiment { namespace Machines
{
    Purge::Purge(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Pressurise state machine
        ,   m_vent(*new Vent(parent, settings, hardware, safety))
        ,   m_pressurise(*new Pressurise(parent, settings, hardware, safety))
        ,   m_vacDown(*new VacDown(parent, settings, hardware, safety))
    {
        // We have stop state machines
        shutDownMachines = true;

        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Vent state machine
        connect(state("vent", true), &QState::entered, this, &Purge::ventOutput);
        connect(state("finishingVent", true), &QState::entered, this, &Purge::ventOutput);

        // Vac down state machine
        connect(state("vacDown", true), &QState::entered, this, &Purge::setVacuum);

        // Set pressure state machine
        connect(state("setPressure", true), &QState::entered, this, &Purge::setHighPressure);

        // Check how many cycles left state
        connect(state("checkCycles", true), &QState::entered, this, &Purge::checkCycles);




        // Shutdown states
        connect(state("vent", false), &QState::entered, this, &Purge::stopVent);
        connect(state("pressurise", false), &QState::entered, this, &Purge::stopPressurise);
        connect(state("vacDown", false), &QState::entered, this, &Purge::stopVacuum);


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

    Purge::~Purge()
    {
    }




    /**
     * Set state machine params
     *
     * @brief Purge::setParams
     * @param outputValve
     * @param numberCycles
     * @param nitrogenPressure
     * @param vacTo
     */
    void Purge::setParams(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo)
    {       
        // Copnfiguration settings
        params.insert("open_output_valve", outputValve);
        params.insert("number_cycles", numberCycles);
        params.insert("nitrogen_pressure", nitrogenPressure);
        params.insert("vac_pressure", vacTo);

        // Local count
        cycles = numberCycles;
    }


    /**
     * Start the state machine
     *
     * @brief Purge::start
     */
    void Purge::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief Purge::stopped
     */
    void Purge::stopped()
    {

    }


    /**
     * Builds the shutdown state machine
     *
     * @brief Purge::buildShutDownMachine
     */
    void Purge::buildShutDownMachine()
    {
        // Where to start the machine
        shutDownMachine.setInitialState(state("vent", false));

        // Vent
        state("vent", false)->addTransition(&m_vent, &Vent::emit_machineAlreadyStopped, state("vacDown", false));
        state("vent", false)->addTransition(&m_vent, &Vent::emit_machineFinished, state("vacDown", false));

        // Vac down to X
        state("vacDown", false)->addTransition(&m_vacDown, &VacDown::emit_machineAlreadyStopped, state("pressurise", false));
        state("vacDown", false)->addTransition(&m_vacDown, &VacDown::emit_machineFinished, state("pressurise", false));

        // Set high pressure
        state("pressurise", false)->addTransition(&m_pressurise, &Pressurise::emit_machineAlreadyStopped, state("closeOuput", false));
        state("pressurise", false)->addTransition(&m_pressurise, &Pressurise::emit_machineFinished, state("closeOuput", false));

        // Ensure all valves are closed
        transitionsBuilder()->closeAllValves(
            state("closeOuput", false),
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
     * @brief Purge::buildMachine
     */
    void Purge::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(state("vent", true));

        // Vent
        state("vent", true)->addTransition(&m_vent, &Vent::emit_machineFailed, &sm_stopAsFailed);
        state("vent", true)->addTransition(&m_vent, &Vent::emit_machineFinished, state("vacDown", true));



        // Vac down to X
        state("vacDown", true)->addTransition(&m_vacDown, &VacDown::emit_machineFailed, &sm_stopAsFailed);
        state("vacDown", true)->addTransition(&m_vacDown, &VacDown::emit_machineFinished, state("setPressure", true));



        // Set high pressure
        state("setPressure", true)->addTransition(&m_pressurise, &Pressurise::emit_machineFailed, &sm_stopAsFailed);
        state("setPressure", true)->addTransition(&m_pressurise, &Pressurise::emit_machineFinished, state("checkCycles", true));



        // Check cycles
        state("checkCycles", true)->addTransition(this, &Purge::emit_continueCycling, state("vent", true));
        state("checkCycles", true)->addTransition(this, &Purge::emit_stopCycling, state("finishingVent", true));

            // Vent pressure
            state("finishingVent", true)->addTransition(&m_vent, &Vent::emit_machineFailed, &sm_stopAsFailed);
            state("finishingVent", true)->addTransition(&m_vent, &Vent::emit_machineFinished, &sm_stop);

    }

    void Purge::checkCycles()
    {
        // Continue or stop?
        if(cycles == 1)
        {
            emit emit_stopCycling();
        }
        else
        {
            // Once cycle just completed
            cycles--;

            emit emit_continueCycling();
        }
    }


    /**
     * Set the purge high pressure
     *
     * @brief Purge::setHighPressure
     */
    void Purge::setHighPressure()
    {
        // Set params
        m_pressurise.setParams(params.value("nitrogen_pressure").toDouble(), true, 2000, false, params.value("open_output_valve").toBool());

        // Override deep params within the pressurise state machine setup
        QVariantMap override;

        // Set valve two tolerance to be larger
        override.insert("tolerance_valve_two", 500);
        override.insert("tolerance_valve_two_step", 500);
        override.insert("tolerance_valve_two_final", 500);

        // Set input valve tolerance to be larger
        override.insert("tolerance_valve_seven", 500);
        override.insert("tolerance_valve_seven_step", 500);
        override.insert("tolerance_valve_seven_final", 500);

        // Set final tolerance to be larger
        override.insert("tolerance_final", 500);

        // Set input valve step size to be larger
        override.insert("valve_7_normal_step_size", 200);
        override.insert("valve_7_final_step_size", 200);
        override.insert("valve_7_step_size", params.value("valve_7_normal_step_size").toInt());

        // Set valve 2 step size to be larger
        override.insert("valve_2_normal_step_size", 200);
        override.insert("valve_2_final_step_size", 200);
        override.insert("valve_2_step_size", params.value("valve_2_normal_step_size").toInt());

        // Set level to vac down to
        override.insert("vac_down_to", params.value("vac_pressure").toDouble());

        // Override the core params with above
        m_pressurise.paramsOverride(override);

        // Build the machine
        m_pressurise.buildMachine();

        // Start the machine
        m_pressurise.start();
    }

    /**
     * Stop the pressurise machine
     *
     * @brief m_pressurise::stopPressurise
     */
    void Purge::stopPressurise()
    {
        // Start the machine
        m_pressurise.stopMachineWithoutError();
    }


    /**
     * Vents pressure
     *
     * @brief Purge::ventOutput
     */
    void Purge::ventOutput()
    {
        // Set the params
        m_vent.setParams(true, false, false, false, false, false, false);

        // Build the machine
        m_vent.buildMachine();

        // Start the machine
        m_vent.start();
    }

    /**
     * Stop the vent machine
     *
     * @brief m_vent::stopVent
     */
    void Purge::stopVent()
    {
        m_vent.buildMachine();
        // Start the machine
        m_vent.stopMachineWithoutError();
    }


    /**
     * Check if the system has exhusted low enough to be vaced down
     *
     * @brief Purge::setVacuum
     */
    void Purge::setVacuum()
    {
        // Set the params
        m_vacDown.setParams(params.value("vac_pressure").toDouble(), 2, false, 2, 1);

        // Build the machine
        m_vacDown.buildMachine();

        // Start the machine
        m_vacDown.start();
    }


    /**
     * Stop the vac machine
     *
     * @brief Purge::stopVacuum
     */
    void Purge::stopVacuum()
    {
        // Start the machine
        m_vacDown.stopMachineWithoutError();
    }

}}}






