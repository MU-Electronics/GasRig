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

        ,   sml_validateEnableTurboPump(&machine)
        ,   sml_validateDisableTurboPump(&machine)


            // Timer states
        ,   sml_checkPressureForVacuum(&machine)
        ,   sml_startVacuumTimer(&machine)
        ,   sml_timerWait(&machine)

            // Timers
        ,   t_vacDown(parent)
    {
        // Pressure validator states
        connect(&sml_validateVacPressureForTurbo, &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validateVacPressureForTurbo);
        connect(&sml_validatePressureForVacuum, &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validatePressureForVacuum);

        // Vacuum states
        connect(&sml_enableBackingPump, &QState::entered, this->vacuum(), &Functions::Vacuum::enableBackingPump);
        connect(&sml_enableTurboPump, &QState::entered, this->vacuum(), &Functions::Vacuum::enableTurboPump);
        connect(&sml_disableTurboPump, &QState::entered, this->vacuum(), &Functions::Vacuum::disableTurboPump);

        // Validator vacuum states
        connect(&sml_validateEnableBackingPump, &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableBackingPump);
        connect(&sml_validateEnableTurboPump, &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableTurboPump);
        connect(&sml_validateDisableTurboPump, &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableTurboPump);


        // Link close valve states
        connect(&sml_closeHighPressureInput, &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(&sml_closeHighPressureNitrogen, &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(&sml_closeFlowController, &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(&sml_closeExhuast, &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(&sml_closeOutput, &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(&sml_closeSlowExhuastPath, &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(&sml_closeFastExhuastPath, &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(&sml_closeVacuumIn, &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(&sml_closeVacuumOut, &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // Link close valve validator states
        connect(&sml_validateCloseHighPressureInput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(&sml_validateCloseHighPressureNitrogen, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(&sml_validateCloseFlowController, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(&sml_validateCloseExhuast, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(&sml_validateCloseOutput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(&sml_validateCloseSlowExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(&sml_validateCloseFastExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(&sml_validateCloseVacuumIn, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(&sml_validateCloseVacuumOut, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

        // Link open valve states
        connect(&sml_openHighPressureInput, &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(&sml_openHighPressureNitrogen, &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(&sml_openFlowController, &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(&sml_openExhuast, &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(&sml_openOutput, &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(&sml_openSlowExhuastPath, &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(&sml_openFastExhuastPath, &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(&sml_openVacuumIn, &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(&sml_openVacuumOut, &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);

        // Link open valve validator states
        connect(&sml_validateOpenHighPressureInput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);
        connect(&sml_validateOpenHighPressureNitrogen, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);
        connect(&sml_validateOpenFlowController, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(&sml_validateOpenExhuast, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(&sml_validateOpenOutput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(&sml_validateOpenSlowExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(&sml_validateOpenFastExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(&sml_validateOpenVacuumIn, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(&sml_validateOpenVacuumOut, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);


        // Timer states
        connect(&sml_startVacuumTimer, &QState::entered, this, &VacDown::startVacuumTimer);


        // Finishing sequence
        //connect(&sm_finishVacSession, &QState::entered, this, &VacDown::finish);
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
    void VacDown::setParams(int mintues, bool turbo, int gasMode, int mode)
    {
        // How long the vac should run
        params.insert("mintues", mintues);

        // Should the turbo ever be enabled
        params.insert("turbo", turbo);

        // What gas mode should be set
        params.insert("gasMode", gasMode);

        // Where the vacuum should go
        params.insert("mode", mode);

        // Setup timers
        t_vacDown.setInterval( (mintues * 60) * 1000 );
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
     * @brief VacDown::start
     */
    void VacDown::stop()
    {
        // Run the finish sequence again
        finish();

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_vacDownFinished(params);
    }

    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief VacDown::stopAsFailed
     */
    void VacDown::stopAsFailed()
    {
        // Run the finish sequence again
        finish();

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_vacDownFailed(params);
    }



    /**
     * Runs on finishing session
     *
     * @brief VacDown::finishVacSession
     */
    void VacDown::finish()
    {
        // Turn off vacuum
       vacuum()->disableTurboPump();
       vacuum()->disableBackingPump();

       // Close valves
       valves()->closeOutput();
       valves()->closeVacuumOut();
       valves()->closeFastExhuastPath();
       valves()->closeSlowExhuastPath();
       valves()->closeVacuumIn();

       // Stop timers
       t_vacDown.stop();
    }


    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void VacDown::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sml_checkPressureForVacuum);

        // Check the system pressure
        sml_checkPressureForVacuum.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureForVacuum);
            // Pressure is low enough
            sml_validatePressureForVacuum.addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, &sml_closeHighPressureInput);
            // Pressure is too high
            sml_validatePressureForVacuum.addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, &sm_stopAsFailed);

        // Close the high pressure valve
        sml_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureInput);
            // Valve closed successfully
            sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeHighPressureNitrogen);
            // Valve failed to close
            sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the nitrogen valve
        sml_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureNitrogen);
            // Valve closed successfully
            sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeFlowController);
            // Valve failed to close
            sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the flow controller valve
        sml_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFlowController);
            // Valve closed successfully
            sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeExhuast);
            // Valve failed to close
            sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the exhuast valve
        sml_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseExhuast);
            // Valve closed successfully check to see what state the output valve should be in
            if(params.value("mode").toInt() == 2 || params.value("mode").toInt() == 3)
            {
                // Close the output
                sml_validateCloseExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeOutput);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the output
                sml_validateCloseExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openOutput);
            }
            // Valve failed to close
            sml_validateCloseExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the output valve
        sml_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseOutput);
        sml_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenOutput);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the fast exhuast valve
                sml_validateCloseOutput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeFastExhuastPath);
            }
            if(params.value("mode").toInt() == 3)
            {
                // Open the fast exhuast path valve
                sml_validateCloseOutput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openFastExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the fast exhaust path valve
                sml_validateOpenOutput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openFastExhuastPath);
            }
            // Valve failed to close
            sml_validateCloseOutput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            sml_validateOpenOutput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve fast
        sml_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFastExhuastPath);
        sml_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenFastExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the slow exhuast valve
                sml_validateCloseFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 3)
            {
                // Open the slow exhuast path valve
                sml_validateOpenFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the slow exhaust path valve
                sml_validateOpenFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openSlowExhuastPath);
            }
            // Valve failed to close
            sml_validateCloseFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            sml_validateOpenFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve slow
        sml_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseSlowExhuastPath);
        sml_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenSlowExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum out valve
                sml_validateCloseSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openVacuumOut);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeVacuumOut);
            }
            // Valve failed to close
            sml_validateCloseSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set vacuum out valve
        sml_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumOut);
        sml_openVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenVacuumOut);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum in valve
                sml_validateOpenVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openVacuumIn);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openVacuumIn);
            }
            // Valve failed to close
            sml_validateOpenVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Set the vacuum in valve open
        sml_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenVacuumIn);
            // Valve closed successfully
            sml_validateOpenVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_disableTurboPump);
            // Valve failed to close
            sml_validateOpenVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Disable the vac station turbo
        sml_disableTurboPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_turboPumpAlreadyDisabled, &sml_startVacuumTimer);
        sml_disableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &sml_validateDisableTurboPump);
            // Turbo pump was disabled
            sml_validateDisableTurboPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, &sml_startVacuumTimer);
            // Turbo pump could not be disabled
            sml_validateDisableTurboPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Start vac time
        sml_startVacuumTimer.addTransition(this, &VacDown::emit_timerActive, &sml_enableBackingPump);

        // Turn on backing pump
        sml_enableBackingPump.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &sml_validateEnableBackingPump);
            // Validate backing pump on
            sml_validateEnableBackingPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, &sml_timerWait);
            // Backing pump failed
            sml_validateEnableBackingPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Read vac pressure
        sml_timerWait.addTransition(&m_hardware, &Hardware::Access::emit_readVacuumPressure, &sml_validateVacPressureForTurbo);
            // Pressure low enough for turbo so enable it
            sml_validateVacPressureForTurbo.addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, &sml_enableTurboPump);
            // Pressure too high for turbo, wait for next time out untill we check again
            sml_validateVacPressureForTurbo.addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, &sml_timerWait);

        // Enable turbo pump
        sml_enableTurboPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_turboPumpAlreadyEnabled, &sml_timerWait);
        sml_enableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &sml_validateEnableTurboPump);
            // Successfully enabled
            sml_validateEnableTurboPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, &sml_timerWait);
            // Could not enable
            sml_validateEnableTurboPump.addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // End vac session when timer limit ends t_vacTime
        sml_timerWait.addTransition(&t_vacDown, &QTimer::timeout, &sm_stop);
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






