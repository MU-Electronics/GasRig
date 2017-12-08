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

            // States
        ,   sml_enableBackingPump(&machine)

        ,   sml_enableTurboPump(&machine)
        ,   sml_disableTurboPump(&machine)

        ,   sml_validateVacPressureForTurbo(&machine)
        ,   sml_validatePressureForVacuum(&machine)

        ,   sml_waitForTurboSpeed(&machine)
        ,   sml_validateTurboSpeedZero(&machine)


            // Close valve related states
        ,   sml_closeHighPressureInput(&machine)
        ,   sml_closeHighPressureNitrogen(&machine)
        ,   sml_closeFlowController(&machine)
        ,   sml_closeExhuast(&machine)
        ,   sml_closeOutput(&machine)
        ,   sml_closeSlowExhuastPath(&machine)
        ,   sml_closeFastExhuastPath(&machine)
        ,   sml_closeVacuumIn(&machine)
        ,   sml_closeVacuumOut(&machine)

            // Open valve related states
        ,   sml_openHighPressureInput(&machine)
        ,   sml_openHighPressureNitrogen(&machine)
        ,   sml_openFlowController(&machine)
        ,   sml_openExhuast(&machine)
        ,   sml_openOutput(&machine)
        ,   sml_openSlowExhuastPath(&machine)
        ,   sml_openFastExhuastPath(&machine)
        ,   sml_openVacuumIn(&machine)
        ,   sml_openVacuumOut(&machine)

            // Validate closed valve
        ,   sml_validateCloseHighPressureInput(&machine)
        ,   sml_validateCloseHighPressureNitrogen(&machine)
        ,   sml_validateCloseFlowController(&machine)
        ,   sml_validateCloseExhuast(&machine)
        ,   sml_validateCloseOutput(&machine)
        ,   sml_validateCloseSlowExhuastPath(&machine)
        ,   sml_validateCloseFastExhuastPath(&machine)
        ,   sml_validateCloseVacuumIn(&machine)
        ,   sml_validateCloseVacuumOut(&machine)

            // Validate open valve
        ,   sml_validateOpenHighPressureInput(&machine)
        ,   sml_validateOpenHighPressureNitrogen(&machine)
        ,   sml_validateOpenFlowController(&machine)
        ,   sml_validateOpenExhuast(&machine)
        ,   sml_validateOpenOutput(&machine)
        ,   sml_validateOpenSlowExhuastPath(&machine)
        ,   sml_validateOpenFastExhuastPath(&machine)
        ,   sml_validateOpenVacuumIn(&machine)
        ,   sml_validateOpenVacuumOut(&machine)


            // Validator states
        ,   sml_validateEnableBackingPump(&machine)

        ,   sml_validatePressureForStop(&machine)

        ,   sml_validateEnableTurboPump(&machine)
        ,   sml_validateDisableTurboPump(&machine)


            // Timer states
        ,   sml_checkPressureForVacuum(&machine)
        ,   sml_startVacuumTimer(&machine)
        ,   sml_timerWait(&machine)

            // Timers
        ,   t_vacDown(parent)
    {
        // We have stop state machines
        shutDownMachines = true;

        childClassName = QString::fromStdString(typeid(this).name());

        // Pressure validator states
        connect(validator("sml_validateVacPressureForTurbo", true), &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validateVacPressureForTurbo);
        connect(validator("sml_validatePressureForVacuum", true), &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validatePressureForVacuum);
        connect(validator("sml_validatePressureForStop", true), &Functions::CommandValidatorState::entered, this, &VacDown::validatePressureForStop);

        // Vacuum states
        connect(state("sml_enableBackingPump", true), &QState::entered, this->vacuum(), &Functions::Vacuum::enableBackingPump);
        connect(state("sml_enableTurboPump", true), &QState::entered, this->vacuum(), &Functions::Vacuum::enableTurboPump);
        connect(state("sml_disableTurboPump", true), &QState::entered, this->vacuum(), &Functions::Vacuum::disableTurboPump);

        // Validator vacuum states
        connect(validator("sml_validateEnableBackingPump", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableBackingPump);
        connect(validator("sml_validateEnableTurboPump", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableTurboPump);
        connect(validator("sml_validateDisableTurboPump", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableTurboPump);
        connect(validator("sml_validateTurboSpeedZero", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateTurboSpeedZero);


        // Link close valve states
        connect(state("sml_closeHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("sml_closeHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("sml_closeFlowController", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("sml_closeExhuast", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("sml_closeOutput", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("sml_closeSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("sml_closeFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("sml_closeVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // Link close valve validator states
        connect(validator("sml_validateCloseHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("sml_validateCloseHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("sml_validateCloseFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("sml_validateCloseExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("sml_validateCloseOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("sml_validateCloseSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("sml_validateCloseFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("sml_validateCloseVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("sml_validateCloseVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

        // Link open valve states
        connect(state("sml_openHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(state("sml_openHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(state("sml_openFlowController", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("sml_openExhuast", true), &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(state("sml_openOutput", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(state("sml_openSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(state("sml_openFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(state("sml_openVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);

        // Link open valve validator states
        connect(validator("sml_validateOpenHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);
        connect(validator("sml_validateOpenHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);
        connect(validator("sml_validateOpenFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("sml_validateOpenExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(validator("sml_validateOpenOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("sml_validateOpenSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(validator("sml_validateOpenFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(validator("sml_validateOpenVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("sml_validateOpenVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);






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





        // Timer states
        connect(&sml_startVacuumTimer, &QState::entered, this, &VacDown::startVacuumTimer);
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
    void VacDown::start()
    {
        // Start the machine
        machine.start();
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
                &ssm_stop
        );

        // Disable turbo pump
        transitionsBuilder()->disableTurboPump(state("disableTurboPump", false), validator("disableTurboPump", false), state("disableBackingPump", false), &ssm_stop);

        // Disable backing pump
        transitionsBuilder()->disableBackingPump(state("disableBackingPump", false), validator("disableBackingPump", false), state("waitForTurboSpeed", false), &ssm_stop);

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
        machine.setInitialState(state("sml_checkPressureForVacuum", true));

        // Check the system pressure
        state("sml_checkPressureForVacuum", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validatePressureForVacuum", true));
            // Pressure is low enough
            validator("sml_validatePressureForVacuum", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, state("sml_closeHighPressureInput", true));
            // Pressure is too high
            validator("sml_validatePressureForVacuum", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, &sm_stopAsFailed);

        // Close the high pressure valve
        state("sml_closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureInput", true));
            // Valve closed successfully
            validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeHighPressureNitrogen", true));
            // Valve failed to close
            validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the nitrogen valve
        state("sml_closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureNitrogen", true));
            // Valve closed successfully
            validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeFlowController", true));
            // Valve failed to close
            validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the flow controller valve
        state("sml_closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFlowController", true));
            // Valve closed successfully
            validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeExhuast", true));
            // Valve failed to close
            validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the exhuast valve
        state("sml_closeExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseExhuast", true));
            // Valve closed successfully check to see what state the output valve should be in
            if(params.value("mode").toInt() == 2 || params.value("mode").toInt() == 3)
            {
                // Close the output
                validator("sml_validateCloseExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeOutput", true));
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the output
                validator("sml_validateCloseExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openOutput", true));
            }
            // Valve failed to close
            validator("sml_validateCloseExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the output valve
        state("sml_closeOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseOutput", true));
        state("sml_openOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenOutput", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the fast exhuast valve
                validator("sml_validateCloseOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeFastExhuastPath", true));
            }
            if(params.value("mode").toInt() == 3)
            {
                // Open the fast exhuast path valve
                validator("sml_validateCloseOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openFastExhuastPath", true));
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the fast exhaust path valve
                validator("sml_validateOpenOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openFastExhuastPath", true));
            }
            // Valve failed to close
            validator("sml_validateCloseOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("sml_validateOpenOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve fast
        state("sml_closeFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFastExhuastPath", true));
        state("sml_openFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenFastExhuastPath", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the slow exhuast valve
                validator("sml_validateCloseFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeSlowExhuastPath", true));
            }
            else if(params.value("mode").toInt() == 3)
            {
                // Open the slow exhuast path valve
                validator("sml_validateOpenFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openSlowExhuastPath", true));
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the slow exhaust path valve
                validator("sml_validateOpenFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openSlowExhuastPath", true));
            }
            // Valve failed to close
            validator("sml_validateCloseFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("sml_validateOpenFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve slow
        state("sml_closeSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseSlowExhuastPath", true));
        state("sml_openSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenSlowExhuastPath", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum out valve
                validator("sml_validateCloseSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openVacuumOut", true));
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                validator("sml_validateOpenSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeVacuumOut", true));
            }
            // Valve failed to close
            validator("sml_validateCloseSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("sml_validateOpenSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set vacuum out valve
        state("sml_closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumOut", true));
        state("sml_openVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenVacuumOut", true));
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum in valve
                validator("sml_validateOpenVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openVacuumIn", true));
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openVacuumIn", true));
            }
            // Valve failed to close
            validator("sml_validateOpenVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Set the vacuum in valve open
        state("sml_openVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenVacuumIn", true));
            // Valve closed successfully
            validator("sml_validateOpenVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_disableTurboPump", true));
            // Valve failed to close
            validator("sml_validateOpenVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Disable the vac station turbo
        state("sml_disableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_turboPumpAlreadyDisabled, state("sml_startVacuumTimer", true));
        state("sml_disableTurboPump", true)->addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, validator("sml_validateDisableTurboPump", true));
            // Turbo pump was disabled
            validator("sml_validateDisableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, state("sml_startVacuumTimer", true));
            // Turbo pump could not be disabled
            validator("sml_validateDisableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Start vac time
        state("sml_startVacuumTimer", true)->addTransition(this, &VacDown::emit_timerActive, state("sml_enableBackingPump", true));

        // Turn on backing pump
        state("sml_enableBackingPump", true)->addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, validator("sml_validateEnableBackingPump", true));
            // Validate backing pump on
            validator("sml_validateEnableBackingPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, state("sml_timerWait", true));
            // Backing pump failed
            validator("sml_validateEnableBackingPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // If turbo is allowed
        if(params.value("turbo").toBool() == true)
        {
            // Read vac pressure
            state("sml_timerWait", true)->addTransition(&m_hardware, &Hardware::Access::emit_readVacuumPressure, validator("sml_validateVacPressureForTurbo", true));
                // Pressure low enough for turbo so enable it
                validator("sml_validateVacPressureForTurbo", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, state("sml_enableTurboPump", true));
                // Pressure too high for turbo, wait for next time out untill we check again
                validator("sml_validateVacPressureForTurbo", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, state("sml_timerWait", true));


            // Enable turbo pump
            state("sml_enableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_turboPumpAlreadyEnabled, state("sml_timerWait", true));
            state("sml_enableTurboPump", true)->addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, validator("sml_validateEnableTurboPump", true));
                // Successfully enabled
                validator("sml_validateEnableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, state("sml_timerWait", true));
                // Could not enable
                validator("sml_validateEnableTurboPump", true)->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, &sm_stopAsFailed);
        }

        // End vac session when timer limit ends t_vacTime
        if(params.value("type").toInt() == 1)
        {
            // Time based vac down
            state("sml_timerWait", true)->addTransition(&t_vacDown, &QTimer::timeout, &sm_stop);
        }
        else if(params.value("type").toInt() == 2)
        {
            // Value based
            state("sml_timerWait", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validatePressureForStop", true));
                // Pressure is too low carry on vac'ing
                validator("sml_validatePressureForStop", true)->addTransition(this, &VacDown::emit_pressureToLow, state("sml_timerWait", true));
                // Pressure is perfect so stop
                validator("sml_validatePressureForStop", true)->addTransition(this, &VacDown::emit_pressureReached, &sm_stop);
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






