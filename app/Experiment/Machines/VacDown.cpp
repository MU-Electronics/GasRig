#include "VacDown.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "MachineStates.h"

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
        QList<QState *> allStates = machine.findChildren<QState *>();
        while(!allStates.isEmpty())
        {
            // Get the current state
            QState *state = allStates.takeFirst();

            // Get all the transistions for this state
            QList<QAbstractTransition*> transitions = state->transitions();

            // Remove all the transisition from the states
            while (!transitions.isEmpty())
                state->removeTransition(transitions.takeFirst());
        }

        // Emit the machine is finished
        emit emit_vacDownFinished(params);
    }



    /**
     * Destroys a machine by removing all states
     *
     * @brief VacDown::destroyMachine
     */
    void VacDown::destroyMachine()
    {


    }



    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void VacDown::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sm_systemPressure);

        // Check the system pressure
        sm_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sm_validatePressureForVacuum);
            // Pressure is low enough
            sm_validatePressureForVacuum.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureInput);
            // Pressure is too high
            sm_validatePressureForVacuum.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Close the high pressure valve
        sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureInput);
            // Valve closed successfully
            sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureNitrogen);
            // Valve failed to close
            sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Close the nitrogen valve
        sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureNitrogen);
            // Valve closed successfully
            sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeFlowController);
            // Valve failed to close
            sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Close the flow controller valve
        sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFlowController);
            // Valve closed successfully
            sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeExhuast);
            // Valve failed to close
            sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Close the exhuast valve
        sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseExhuast);
            // Valve closed successfully check to see what state the output valve should be in
            if(params.value("mode").toInt() == 2 || params.value("mode").toInt() == 3)
            {
                // Close the output
                sm_validateCloseExhuast.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeOutput);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the output
                sm_validateCloseExhuast.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openOutput);
            }
            // Valve failed to close
            sm_validateCloseExhuast.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Set the output valve
        sm_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseOutput);
        sm_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenOutput);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the fast exhuast valve
                sm_validateCloseOutput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeFastExhuastPath);
            }
            if(params.value("mode").toInt() == 3)
            {
                // Open the fast exhuast path valve
                sm_validateCloseOutput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openFastExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the fast exhaust path valve
                sm_validateOpenOutput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openFastExhuastPath);
            }
            // Valve failed to close
            sm_validateCloseOutput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);
            sm_validateOpenOutput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Set the exhuast path valve fast
        sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFastExhuastPath);
        sm_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenFastExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Close the slow exhuast valve
                sm_validateCloseFastExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 3)
            {
                // Open the slow exhuast path valve
                sm_validateOpenFastExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openSlowExhuastPath);
            }
            else if(params.value("mode").toInt() == 1)
            {
                // Open the slow exhaust path valve
                sm_validateOpenFastExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openSlowExhuastPath);
            }
            // Valve failed to close
            sm_validateCloseFastExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);
            sm_validateOpenFastExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Set the exhuast path valve slow
        sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseSlowExhuastPath);
        sm_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenSlowExhuastPath);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum out valve
                sm_validateCloseSlowExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openVacuumOut);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                sm_validateOpenSlowExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeVacuumOut);
            }
            // Valve failed to close
            sm_validateCloseSlowExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);
            sm_validateOpenSlowExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Set vacuum out valve
        sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumOut);
        sm_openVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenVacuumOut);
            // Valve closed successfully
            if(params.value("mode").toInt() == 2)
            {
                // Open the vacuum in valve
                sm_validateOpenVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openVacuumIn);
            }
            else if(params.value("mode").toInt() == 3 || params.value("mode").toInt() == 1)
            {
                // Close the vacuum out valve
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openVacuumIn);
            }
            // Valve failed to close
            sm_validateOpenVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);
            sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);


        // Set the vacuum in valve open
        sm_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenVacuumIn);
            // Valve closed successfully
            sm_validateOpenVacuumIn.addTransition(this, &MachineStates::emit_validationSuccess, &sm_disableTurboPump);
            // Valve failed to close
            sm_validateOpenVacuumIn.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Disable the vac station turbo
        sm_disableTurboPump.addTransition(this, &MachineStates::emit_stateAlreadySet, &sm_startVacuumPressureMonitor);
        sm_disableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &sm_validateDisableTurboPump);
            // Turbo pump was disabled
            sm_validateDisableTurboPump.addTransition(this, &MachineStates::emit_validationSuccess, &sm_startVacuumPressureMonitor);
            // Turbo pump could not be disabled
            sm_validateDisableTurboPump.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Start monitoring the vacuum sensor
        sm_startVacuumPressureMonitor.addTransition(this, &MachineStates::emit_timerActive, &sm_startVacuumTimer);

        // Start vac time
        sm_startVacuumTimer.addTransition(this, &MachineStates::emit_timerActive, &sm_enableBackingPump);

        // Turn on backing pump
        sm_enableBackingPump.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &sm_validateEnableBackingPump);
            // Validate backing pump on
            sm_validateEnableBackingPump.addTransition(this, &MachineStates::emit_validationSuccess, &sm_vacPressure);
            // Backing pump failed
            sm_validateEnableBackingPump.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // Read vac pressure
        sm_vacPressure.addTransition(&m_hardware, &Hardware::Access::emit_readAnaloguePort, &sm_validateVacPressureForTurbo);
            // Pressure low enough for turbo so enable it
            sm_validateVacPressureForTurbo.addTransition(this, &MachineStates::emit_validationSuccess, &sm_enableTurboPump);
            // Pressure too high for turbo, wait for next time out untill we check again
            sm_validateVacPressureForTurbo.addTransition(this, &MachineStates::emit_validationFailed, &sm_timerWait);

        // Enable turbo pump
        sm_enableTurboPump.addTransition(this, &MachineStates::emit_stateAlreadySet, &sm_timerWait);
        sm_enableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &sm_validateEnableTurboPump);
            // Successfully enabled
            sm_validateEnableTurboPump.addTransition(this, &MachineStates::emit_validationSuccess, &sm_timerWait);
            // Could not enable
            sm_validateEnableTurboPump.addTransition(this, &MachineStates::emit_validationFailed, &sm_stop);

        // End vac session when timer limit ends t_vacTime
        sm_timerWait.addTransition(&t_vacTime, &QTimer::timeout, &sm_stop);
        // Wait for timer event
        sm_timerWait.addTransition(&t_vacPressureMonitor, &QTimer::timeout, &sm_vacPressure);
    }
}}}






