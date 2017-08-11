#include "VacDown.h"

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
    VacDown::VacDown(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {
        // Finishing sequence
        connect(&sm_finishVacSession, &QState::entered, this, &VacDown::finish);
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
        timers()->t_vacTime.setInterval( (mintues * 60) * 1000 );

        // To delete soon
        timers()->t_vacPressureMonitor.setInterval(500);
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
       timers()->stopVacuumPressureMonitor();
       timers()->stopVacuumTimer();
    }


    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void VacDown::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&timers()->sm_initalWait);

        // Check the system pressure
        timers()->sm_initalWait.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &pressure()->sm_validatePressureForVacuum);
            // Pressure is low enough
            pressure()->sm_validatePressureForVacuum.addTransition(this->pressure(), &States::Pressure::emit_validationSuccess, &valves()->sm_closeHighPressureInput);
            // Pressure is too high
            pressure()->sm_validatePressureForVacuum.addTransition(this->pressure(), &States::Pressure::emit_validationFailed, &sm_stopAsFailed);

        // Close the high pressure valve
        valves()->sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureInput);
            // Valve closed successfully
            valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeHighPressureNitrogen);
            // Valve failed to close
            valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the nitrogen valve
        valves()->sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureNitrogen);
            // Valve closed successfully
            valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeFlowController);
            // Valve failed to close
            valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the flow controller valve
        valves()->sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFlowController);
            // Valve closed successfully
            valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeExhuast);
            // Valve failed to close
            valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the exhuast valve
        valves()->sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseExhuast);
            // Valve closed successfully check to see what state the output valve should be in
            if(params.value("mode").toInt() == 2 || params.value("mode").toInt() == 3)
            {
                // Close the output
                valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeOutput);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the output
                valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openOutput);
            }
            // Valve failed to close
            valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the output valve
        valves()->sm_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseOutput);
        valves()->sm_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenOutput);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the fast exhuast valve
                valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeFastExhuastPath);
            }
            if(params.value("mode").toInt() == 3)
            {
                // Open the fast exhuast path valve
                valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openFastExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the fast exhaust path valve
                valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openFastExhuastPath);
            }
            // Valve failed to close
            valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve fast
        valves()->sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFastExhuastPath);
        valves()->sm_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenFastExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the slow exhuast valve
                valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 3)
            {
                // Open the slow exhuast path valve
                valves()->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the slow exhaust path valve
                valves()->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openSlowExhuastPath);
            }
            // Valve failed to close
            valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            valves()->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve slow
        valves()->sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseSlowExhuastPath);
        valves()->sm_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenSlowExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum out valve
                valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openVacuumOut);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeVacuumOut);
            }
            // Valve failed to close
            valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set vacuum out valve
        valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
        valves()->sm_openVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenVacuumOut);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum in valve
                valves()->sm_validateOpenVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openVacuumIn);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openVacuumIn);
            }
            // Valve failed to close
            valves()->sm_validateOpenVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Set the vacuum in valve open
        valves()->sm_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenVacuumIn);
            // Valve closed successfully
            valves()->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &vacuum()->sm_disableTurboPump);
            // Valve failed to close
            valves()->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Disable the vac station turbo
        vacuum()->sm_disableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_turboPumpAlreadyDisabled, &timers()->sm_startVacuumTimer);
        vacuum()->sm_disableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &vacuum()->sm_validateDisableTurboPump);
            // Turbo pump was disabled
            vacuum()->sm_validateDisableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &timers()->sm_startVacuumTimer);
            // Turbo pump could not be disabled
            vacuum()->sm_validateDisableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Start vac time
        timers()->sm_startVacuumTimer.addTransition(this->timers(), &States::Timers::emit_timerActive, &vacuum()->sm_enableBackingPump);

        // Turn on backing pump
        vacuum()->sm_enableBackingPump.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &vacuum()->sm_validateEnableBackingPump);
            // Validate backing pump on
            vacuum()->sm_validateEnableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &timers()->sm_timerWait);
            // Backing pump failed
            vacuum()->sm_validateEnableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Read vac pressure
        timers()->sm_timerWait.addTransition(&m_hardware, &Hardware::Access::emit_readAnaloguePort, &pressure()->sm_validateVacPressureForTurbo);
            // Pressure low enough for turbo so enable it
            pressure()->sm_validateVacPressureForTurbo.addTransition(this->pressure(), &States::Pressure::emit_validationSuccess, &vacuum()->sm_enableTurboPump);
            // Pressure too high for turbo, wait for next time out untill we check again
            pressure()->sm_validateVacPressureForTurbo.addTransition(this->pressure(), &States::Pressure::emit_validationFailed, &timers()->sm_timerWait);

        // Enable turbo pump
        vacuum()->sm_enableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_turboPumpAlreadyEnabled, &timers()->sm_timerWait);
        vacuum()->sm_enableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &vacuum()->sm_validateEnableTurboPump);
            // Successfully enabled
            vacuum()->sm_validateEnableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &timers()->sm_timerWait);
            // Could not enable
            vacuum()->sm_validateEnableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // End vac session when timer limit ends t_vacTime
        timers()->sm_timerWait.addTransition(&timers()->t_vacTime, &QTimer::timeout, &sm_stop);
    }


}}}






