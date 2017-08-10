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
        t_vacTime.setInterval( (mintues * 60) * 1000 );

        // To delete soon
        t_vacPressureMonitor.setInterval(500);
    }


    /**
     * Start the state machine
     *
     * @brief VacDown::start
     */
    void VacDown::start()
    {
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
        finishVacSession();

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
        finishVacSession();

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_vacDownFailed(params);
    }



    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void VacDown::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sm_initalWait);

        // Check the system pressure
        sm_initalWait.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &m_pressure->sm_validatePressureForVacuum);
            // Pressure is low enough
            m_pressure->sm_validatePressureForVacuum.addTransition(this->pressure(), &States::Pressure::emit_validationSuccess, &m_valves->sm_closeHighPressureInput);
            // Pressure is too high
            m_pressure->sm_validatePressureForVacuum.addTransition(this->pressure(), &States::Pressure::emit_validationFailed, &sm_stopAsFailed);

        // Close the high pressure valve
        m_valves->sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseHighPressureInput);
            // Valve closed successfully
            m_valves->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_closeHighPressureNitrogen);
            // Valve failed to close
            m_valves->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the nitrogen valve
        m_valves->sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseHighPressureNitrogen);
            // Valve closed successfully
            m_valves->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_closeFlowController);
            // Valve failed to close
            m_valves->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the flow controller valve
        m_valves->sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseFlowController);
            // Valve closed successfully
            m_valves->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_closeExhuast);
            // Valve failed to close
            m_valves->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the exhuast valve
        m_valves->sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseExhuast);
            // Valve closed successfully check to see what state the output valve should be in
            if(params.value("mode").toInt() == 2 || params.value("mode").toInt() == 3)
            {
                // Close the output
                m_valves->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_closeOutput);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the output
                m_valves->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openOutput);
            }
            // Valve failed to close
            m_valves->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the output valve
        m_valves->sm_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseOutput);
        m_valves->sm_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateOpenOutput);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the fast exhuast valve
                m_valves->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_closeFastExhuastPath);
            }
            if(params.value("mode").toInt() == 3)
            {
                // Open the fast exhuast path valve
                m_valves->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openFastExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the fast exhaust path valve
                m_valves->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openFastExhuastPath);
            }
            // Valve failed to close
            m_valves->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            m_valves->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve fast
        m_valves->sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseFastExhuastPath);
        m_valves->sm_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateOpenFastExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the slow exhuast valve
                m_valves->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_closeSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 3)
            {
                // Open the slow exhuast path valve
                m_valves->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the slow exhaust path valve
                m_valves->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openSlowExhuastPath);
            }
            // Valve failed to close
            m_valves->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            m_valves->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set the exhuast path valve slow
        m_valves->sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseSlowExhuastPath);
        m_valves->sm_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateOpenSlowExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum out valve
                m_valves->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openVacuumOut);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                m_valves->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_closeVacuumOut);
            }
            // Valve failed to close
            m_valves->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            m_valves->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Set vacuum out valve
        m_valves->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateCloseVacuumOut);
        m_valves->sm_openVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateOpenVacuumOut);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum in valve
                m_valves->sm_validateOpenVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openVacuumIn);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                m_valves->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_valves->sm_openVacuumIn);
            }
            // Valve failed to close
            m_valves->sm_validateOpenVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
            m_valves->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Set the vacuum in valve open
        m_valves->sm_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &m_valves->sm_validateOpenVacuumIn);
            // Valve closed successfully
            m_valves->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &m_vacuum->sm_disableTurboPump);
            // Valve failed to close
            m_valves->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Disable the vac station turbo
        m_vacuum->sm_disableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_turboPumpAlreadyDisabled, &sm_startVacuumTimer);
        m_vacuum->sm_disableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &m_vacuum->sm_validateDisableTurboPump);
            // Turbo pump was disabled
            m_vacuum->sm_validateDisableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &sm_startVacuumTimer);
            // Turbo pump could not be disabled
            m_vacuum->sm_validateDisableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Start vac time
        sm_startVacuumTimer.addTransition(this, &States::MachineStates::emit_timerActive, &m_vacuum->sm_enableBackingPump);

        // Turn on backing pump
        m_vacuum->sm_enableBackingPump.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &m_vacuum->sm_validateEnableBackingPump);
            // Validate backing pump on
            m_vacuum->sm_validateEnableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &sm_timerWait);
            // Backing pump failed
            m_vacuum->sm_validateEnableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Read vac pressure
        sm_timerWait.addTransition(&m_hardware, &Hardware::Access::emit_readAnaloguePort, &m_pressure->sm_validateVacPressureForTurbo);
            // Pressure low enough for turbo so enable it
            m_pressure->sm_validateVacPressureForTurbo.addTransition(this->pressure(), &States::Pressure::emit_validationSuccess, &m_vacuum->sm_enableTurboPump);
            // Pressure too high for turbo, wait for next time out untill we check again
            m_pressure->sm_validateVacPressureForTurbo.addTransition(this->pressure(), &States::Pressure::emit_validationFailed, &sm_timerWait);

        // Enable turbo pump
        m_vacuum->sm_enableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_turboPumpAlreadyEnabled, &sm_timerWait);
        m_vacuum->sm_enableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &m_vacuum->sm_validateEnableTurboPump);
            // Successfully enabled
            m_vacuum->sm_validateEnableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &sm_timerWait);
            // Could not enable
            m_vacuum->sm_validateEnableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // End vac session when timer limit ends t_vacTime
        sm_timerWait.addTransition(&t_vacTime, &QTimer::timeout, &sm_stop);
    }
}}}






