#include "VacDown.h"

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
    VacDown::VacDown(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_vacDown(parent)
    {
        // We have stop state machines
        shutDownMachines = true;

        childClassName = QString::fromStdString(typeid(this).name());

        // Pressure validator states
        connect(validator("vacPressureForTurbo", true), &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validateVacPressureForTurbo);
        connect(validator("pressureForVacuum", true), &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validatePressureForVacuum);
        connect(validator("pressureForStop", true), &Functions::CommandValidatorState::entered, this, &VacDown::validatePressureForStop);

        // Vacuum states
        connect(state("enableBackingPump", true), &QState::entered, this->vacuum(), &Functions::Vacuum::enableBackingPump);
        connect(state("enableTurboPump", true), &QState::entered, this->vacuum(), &Functions::Vacuum::enableTurboPump);
        connect(state("disableTurboPump", true), &QState::entered, this->vacuum(), &Functions::Vacuum::disableTurboPump);

        // Validator vacuum states
        connect(validator("enableBackingPump", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableBackingPump);
        connect(validator("enableTurboPump", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableTurboPump);
        connect(validator("disableTurboPump", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableTurboPump);
        connect(validator("turboSpeedZero", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateTurboSpeedZero);


        // Link close valve states
        connect(state("closeHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("closeHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("closeFlowController", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("closeExhuast", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("closeOutput", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("closeSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("closeFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("closeVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // Link close valve validator states
        connect(validator("closeHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("closeHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("closeFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("closeExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("closeOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("closeSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("closeFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("closeVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("closeVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

        // Link open valve states
        connect(state("openHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(state("openHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(state("openFlowController", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("openExhuast", true), &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(state("openOutput", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(state("openSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(state("openFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(state("openVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);

        // Link open valve validator states
        connect(validator("openHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);
        connect(validator("openHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);
        connect(validator("openFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("openExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(validator("openOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("openSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(validator("openFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(validator("openVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("openVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);

        // Timer states
        connect(state("startVacuumTimer", true), &QState::entered, this, &VacDown::startVacuumTimer);







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

        // SHUTDOWN STATE MACHINE: links vac states
        connect(state("disableTurboPump", false), &QState::entered, this->vacuum(), &Functions::Vacuum::disableTurboPump);
        connect(state("disableBackingPump", false), &QState::entered, this->vacuum(), &Functions::Vacuum::disableBackingPump);

        connect(validator("disableTurboPump", false), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableTurboPump);
        connect(validator("disableBackingPump", false), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableBackingPump);
        connect(validator("turboSpeedZero", false), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateTurboSpeedZero);
    }

    VacDown::~VacDown()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void VacDown::setParams(double value, int valueType, bool turbo, int gasMode, int mode)
    {
        if(valueType == 1)
        {
            // How long the vac should run
            params.insert("milliseconds", value);

            // Setup timers
            t_vacDown.setInterval(value);
        }
        else if(valueType == 2)
        {
            // How long the vac should run
            params.insert("vac_down_to", value);
        }

        // Time or value based vac down
        params.insert("type", valueType);

        // Should the turbo ever be enabled
        params.insert("turbo", turbo);

        // What gas mode should be set
        params.insert("gasMode", gasMode);

        // Where the vacuum should go
        params.insert("mode", mode);


    }


    /**
     * Start the state machine
     *
     * @brief VacDown::start
     */
    void VacDown::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief VacDown::stopped
     */
    void VacDown::stopped()
    {
       // Stop timers
       t_vacDown.stop();
    }


    /**
     * Builds the shutdown state machine
     *
     * @brief VacDown::buildShutDownMachine
     */
    void VacDown::buildShutDownMachine()
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
                state("disableTurboPump", false),
                &ssm_stopAsFailed
        );

        // Disable turbo pump
        transitionsBuilder()->disableTurboPump(state("disableTurboPump", false), validator("disableTurboPump", false), state("disableBackingPump", false), state("disableBackingPump", false), &ssm_stopAsFailed);

        // Disable backing pump
        transitionsBuilder()->disableBackingPump(state("disableBackingPump", false), validator("disableBackingPump", false), state("waitForTurboSpeed", false), &ssm_stopAsFailed);

        // Check that turbo is not spinner before we stop
        state("waitForTurboSpeed", false)->addTransition(&m_hardware, &Hardware::Access::emit_getTurboSpeed, validator("turboSpeedZero", false));
            // Is the turbo spinning
            validator("turboSpeedZero", false)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, state("waitForTurboSpeed", false));
            validator("turboSpeedZero", false)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, &ssm_stop);
    }





    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void VacDown::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(state("checkPressureForVacuum", true));

        // Check the system pressure
        transitionsBuilder()->validatePressure(state("checkPressureForVacuum", true), validator("pressureForVacuum", true), state("closeHighPressureInput", true), &sm_stopAsFailed);

        // Close the high pressure valve
        transitionsBuilder()->closeValve(state("closeHighPressureInput", true), validator("closeHighPressureInput", true), state("closeHighPressureNitrogen", true), &sm_stopAsFailed);

        // Close the nitrogen valve
        transitionsBuilder()->closeValve(state("closeHighPressureNitrogen", true), validator("closeHighPressureNitrogen", true), state("closeFlowController", true), &sm_stopAsFailed);

        // Close the flow controller valve
        transitionsBuilder()->closeValve(state("closeFlowController", true), validator("closeFlowController", true), state("closeExhuast", true), &sm_stopAsFailed);

        // Close the exhuast valve
        state("closeExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeExhuast", true));
            // Wrong signal was picked up
            validator("closeExhuast", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("closeExhuast", true));
            // Valve closed successfully check to see what state the output valve should be in
            if(params.value("mode").toInt() == 2 || params.value("mode").toInt() == 3)
            {
                // Close the output
                validator("closeExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeOutput", true));
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the output
                validator("closeExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openOutput", true));
            }
            // Valve failed to close
            validator("closeExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the output valve
        state("closeOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeOutput", true));
        state("openOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openOutput", true));
            // Wrong signal was picked up
            validator("openOutput", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("openOutput", true));
            validator("closeOutput", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("closeOutput", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the fast exhuast valve
                validator("closeOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeFastExhuastPath", true));
            }
            if(params.value("mode").toInt() == 3)
            {
                // Open the fast exhuast path valve
                validator("closeOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openFastExhuastPath", true));
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the fast exhaust path valve
                validator("openOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openFastExhuastPath", true));
            }
            // Valve failed to close
            validator("closeOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("openOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve fast
        state("closeFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeFastExhuastPath", true));
        state("openFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openFastExhuastPath", true));
            // Wrong signal was picked up
            validator("closeFastExhuastPath", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("closeFastExhuastPath", true));
            validator("openFastExhuastPath", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("openFastExhuastPath", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the slow exhuast valve
                validator("closeFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeSlowExhuastPath", true));
            }
            else if(params.value("mode").toInt() == 3)
            {
                // Open the slow exhuast path valve
                validator("openFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openSlowExhuastPath", true));
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the slow exhaust path valve
                validator("openFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openSlowExhuastPath", true));
            }
            // Valve failed to close
            validator("closeFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("openFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve slow
        state("closeSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeSlowExhuastPath", true));
        state("openSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openSlowExhuastPath", true));
            // Wrong signal was picked up
            validator("closeSlowExhuastPath", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("closeSlowExhuastPath", true));
            validator("openSlowExhuastPath", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("openSlowExhuastPath", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum out valve
                validator("closeSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openVacuumOut", true));
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                validator("openSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeVacuumOut", true));
            }
            // Valve failed to close
            validator("closeSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("openSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set vacuum out valve
        state("closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumOut", true));
        state("openVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openVacuumOut", true));
            // Wrong signal was picked up
            validator("closeVacuumOut", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("closeVacuumOut", true));
            validator("openVacuumOut", true)->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, state("openVacuumOut", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum in valve
                validator("openVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openVacuumIn", true));
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openVacuumIn", true));
            }
            // Valve failed to close
            validator("openVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Set the vacuum in valve open
        transitionsBuilder()->openValve(state("openVacuumIn", true), validator("openVacuumIn", true), state("disableTurboPump", true), &sm_stopAsFailed);

        // Disable the vac station turbo
        transitionsBuilder()->disableTurboPump(state("disableTurboPump", true), validator("disableTurboPump", true), state("startVacuumTimer", true), state("startVacuumTimer", true), &sm_stopAsFailed);

        // Start vac time
        state("startVacuumTimer", true)->addTransition(this, &VacDown::emit_timerActive, state("enableBackingPump", true));

        // Turn on backing pump
        transitionsBuilder()->enableBackingPump(state("enableBackingPump", true), validator("enableBackingPump", true), state("timerWait", true), &sm_stopAsFailed);

        // If turbo is allowed
        if(params.value("turbo").toBool() == true)
        {
            // Read vac pressure
            state("timerWait", true)->addTransition(&m_hardware, &Hardware::Access::emit_readVacuumPressure, validator("vacPressureForTurbo", true));
                // Pressure low enough for turbo so enable it
                validator("vacPressureForTurbo", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, state("enableTurboPump", true));
                // Pressure too high for turbo, wait for next time out untill we check again
                validator("vacPressureForTurbo", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, state("timerWait", true));


            // Enable turbo pump
            state("enableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_turboPumpAlreadyEnabled, state("timerWait", true));
            state("enableTurboPump", true)->addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, validator("enableTurboPump", true));
                // Successfully enabled
                validator("enableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, state("timerWait", true));
                // Could not enable
                validator("enableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, &sm_stopAsFailed);
        }

        // End vac session when timer limit ends t_vacTime
        if(params.value("type").toInt() == 1)
        {
            // Time based vac down
            state("timerWait", true)->addTransition(&t_vacDown, &QTimer::timeout, &sm_stop);
        }
        else if(params.value("type").toInt() == 2)
        {
            // Value based
            state("timerWait", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("pressureForStop", true));
                // Pressure is too low carry on vac'ing
                validator("pressureForStop", true)->addTransition(this, &VacDown::emit_pressureToLow, state("timerWait", true));
                // Pressure is perfect so stop
                validator("pressureForStop", true)->addTransition(this, &VacDown::emit_pressureReached, &sm_stop);
        }

    }


    /**
     * Validate a reading of the system pressure
     *
     * @brief Pressure::validatePressureForStop
     */
    void VacDown::validatePressureForStop()
    {
        // Get the validator state instance
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Get the pressure
        double pressureIn = package.value("pressure").toDouble() * 1000;

        // Get the max pressure allowed
        double stopPressure = params.value("vac_down_to").toDouble();

        // Check the pressure is safe to vac down
        if( (pressureIn - 0.1) < stopPressure)
        {
            emit emit_pressureReached();

            return;
        }

        // Pressure too low
        emit emit_pressureToLow();
    }


    /**
     * The timer for how long to vac down for
     *
     * @brief VacDown::startVacuumTimer
     */
    void VacDown::startVacuumTimer()
    {
        if(t_vacDown.isActive())
            return;

        // Setup timer
        t_vacDown.setSingleShot(true);

        // Start timer
        t_vacDown.start();

        // Announce the timer has started
        emit emit_timerActive();
    }

}}}
