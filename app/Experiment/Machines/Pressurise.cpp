#include "Pressurise.h"

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
    Pressurise::Pressurise(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Addtion states for state machine
        ,   sml_waitForPressureAfterValveOne(&machine)
        ,   sml_waitForPressureAfterValveTwo(&machine)
        ,   sml_waitForPressureAfterValveSeven(&machine)
        ,   sml_waitForInitalPressure(&machine)
        ,   sml_waitForInitialSystemVacDown(&machine)

        ,   sml_waitForValveOneTimer(&machine)
        ,   sml_waitForValveTwoTimer(&machine)
        ,   sml_waitForValveSevenTimer(&machine)
        ,   sml_waitForExhuastVoidVacDown(&machine)

            // Copy states that are used more than once to make then unique
        ,   sml_closeHighPressureInput_2(&machine)
        ,   sml_closeSlowExhuastPath_2(&machine)
        ,   sml_closeOutput_2(&machine)
        ,   sml_openVacuumIn_2(&machine)

        ,   sml_validateCloseSlowExhuastPath_2(&machine)
        ,   sml_validateCloseHighPressureInput_2(&machine)
        ,   sml_validateCloseOutput_2(&machine)
        ,   sml_validateOpenVacuumIn_2(&machine)


            // Addtional validator states for state machine
        ,   sml_validatePressureAfterValveSeven(&machine)
        ,   sml_validatePressureAfterValveTwo(&machine)
        ,   sml_validatePressureAfterValveOne(&machine)
        ,   sml_validateInitialSystemVacuum(&machine)

            // Timer states
        ,   sml_startValveOneTimer(&machine)
        ,   sml_startValveTwoTimer(&machine)
        ,   sml_startValveSevenTimer(&machine)
        ,   sml_startExhuastVoidVacDownTimer(&machine)

            // Timers for state machine
        ,   t_pulseValveOne(parent)
        ,   t_pulseValveTwo(parent)
        ,   t_pulseValveSeven(parent)
        ,   t_exhuastVoidVacDownTimer(parent)
    {
        // Connect signals and slots
        connectStatesToMethods();
    }

    Pressurise::~Pressurise()
    {

    }


    /**
     * Connect local states to methods
     *
     * @brief Pressurise::connectStatesToMethods
     */
    void Pressurise::connectStatesToMethods()
    {
        // Timers
        connect(&sml_startValveOneTimer, &QState::entered, this, &Pressurise::startValveOnePulseTimer);
        connect(&sml_startValveTwoTimer, &QState::entered, this, &Pressurise::startValveTwoPulseTimer);
        connect(&sml_startValveSevenTimer, &QState::entered, this, &Pressurise::startValveSevenPulseTimer);
        connect(&sml_startExhuastVoidVacDownTimer, &QState::entered, this, &Pressurise::startExhuastVoidVacDownTimer);

        // Copy states that are used more than once to make then unique
        connect(&sml_closeHighPressureInput_2, &QState::entered, this->valves(), &States::Valves::closeHighPressureInput);
        connect(&sml_closeOutput_2, &QState::entered, this->valves(), &States::Valves::closeOutput);
        connect(&sml_closeSlowExhuastPath_2, &QState::entered, this->valves(), &States::Valves::closeSlowExhuastPath);
        connect(&sml_openVacuumIn_2, &QState::entered, this->valves(), &States::Valves::openVacuumIn);

        connect(&sml_validateCloseHighPressureInput_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseHighPressureInput);
        connect(&sml_validateCloseOutput_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseOutput);
        connect(&sml_validateCloseSlowExhuastPath_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseSlowExhuastPath);
        connect(&sml_validateOpenVacuumIn_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateOpenVacuumIn);

        // Addtional validator states
        connect(&sml_validatePressureAfterValveOne, &States::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveOne);
        connect(&sml_validatePressureAfterValveTwo, &States::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveTwo);
        connect(&sml_validatePressureAfterValveSeven, &States::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveSeven);
        connect(&sml_validateInitialSystemVacuum, &States::CommandValidatorState::entered, this, &Pressurise::validateInitialSystemVacuum);

    }



    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void Pressurise::setParams(double pressure)
    {
        // What is the target pressure
        params.insert("pressure", pressure);

        // What is the step size in pressure
        params.insert("step_size", 2000);


        // When do we need a vacuum backing for the exhaust
        params.insert("vacuum_backing", 1500);

        // Inital vac down time
        params.insert("exhuast_void_vac_down_time", 5000);

        // Initall vac down system to
        params.insert("vac_down_to", 20);


        // What is the tolerance of valve 7
        params.insert("tolerance_valve_seven", 500);

        // What is the tolerance of valve 2
        params.insert("tolerance_valve_two", 100);

        // What is the tolerance of valve 1
        params.insert("tolerance_valve_one", 30);


        // How fast to pulse valve 7
        params.insert("valve_7_pulse", 100);

        // How fast to pulse valve 2
        params.insert("valve_2_pulse", 50);

        // How fast to pulse valve 1
        params.insert("valve_1_pulse", 5000);


        // Set the timer for the pulse width for valve 1
        t_pulseValveOne.setInterval(params.value("valve_1_pulse").toInt());

        // Set the timer for the pulse width for valve 2
        t_pulseValveTwo.setInterval(params.value("valve_2_pulse").toInt());

        // Set the timer for the pulse width for valve 7
        t_pulseValveSeven.setInterval(params.value("valve_7_pulse").toInt());


        // Set the timer for the inital vac down time
        t_exhuastVoidVacDownTimer.setInterval(params.value("exhuast_void_vac_down_time").toInt());
    }


    /**
     * Start the state machine
     *
     * @brief Pressurise::start
     */
    void Pressurise::start()
    {
        // Reset vars
        pressureReading = 1;

        // Start machine
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief Pressurise::start
     */
    void Pressurise::stop()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Close valves
        valves()->closeOutput();
        valves()->closeExhuast();
        valves()->closeFastExhuastPath();
        valves()->closeSlowExhuastPath();
        valves()->closeHighPressureInput();
        valves()->closeVacuumIn();

        // Turn off vacuum pum
        vacuum()->disableBackingPump();
        vacuum()->disableTurboPump();

        // Stop timers
        stopValveOnePulseTimer();
        stopValveTwoPulseTimer();
        stopValveSevenPulseTimer();
        startExhuastVoidVacDownTimer();

        qDebug() << "Stopped";

        // Emit the machine is finished
        emit emit_pressuriseFinished(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief Pressurise::stopAsFailed
     */
    void Pressurise::stopAsFailed()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Close valves
        valves()->closeOutput();
        valves()->closeExhuast();
        valves()->closeFastExhuastPath();
        valves()->closeSlowExhuastPath();
        valves()->closeHighPressureInput();
        valves()->closeVacuumIn();

        // Turn off vacuum pum
        vacuum()->disableBackingPump();
        vacuum()->disableTurboPump();

        // Stop timers
        stopValveOnePulseTimer();
        stopValveTwoPulseTimer();
        stopValveSevenPulseTimer();
        startExhuastVoidVacDownTimer();

        qDebug() << "Stopped as failed";

        // Emit the machine is finished
        emit emit_pressuriseFailed(params);
    }




    /**
     * Builds the machine connections
     *
     * @brief Pressurise::buildMachine
     */
    void Pressurise::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sml_waitForInitalPressure);

        // Check the system pressure
        sml_waitForInitalPressure.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &pressure()->sm_validatePressureForVacuum);
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
            valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeVacuumOut);
            // Valve failed to close
            valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close Vacuum Output
        valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
            // Open the fast exhaust path valve
            valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeFlowController);
            // Valve failed to close
            valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Close the flow controller valve
        valves()->sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFlowController);
            // Valve closed successfully
            valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeExhuast);
            // Valve failed to close
            valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Close the exhuast valve
        valves()->sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseExhuast);
            // Close the output
            valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeSlowExhuastPath);
            // Valve failed to close
            valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Close Slow Exhuast
        valves()->sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseSlowExhuastPath);
            // Open the fast exhaust path valve
            valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &vacuum()->sm_disableTurboPump);
            // Valve failed to close
            valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Disable the vac station turbo
        vacuum()->sm_disableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_turboPumpAlreadyDisabled, &vacuum()->sm_validateDisableTurboPump);
        vacuum()->sm_disableTurboPump.addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, &vacuum()->sm_validateDisableTurboPump);
            // Turbo pump was disabled
            vacuum()->sm_validateDisableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &vacuum()->sm_enableBackingPump);
            // Turbo pump could not be disabled
            vacuum()->sm_validateDisableTurboPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Enabled the backing pump
        vacuum()->sm_enableBackingPump.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &vacuum()->sm_validateEnableBackingPump);
            // Validate backing pump on
            vacuum()->sm_validateEnableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &valves()->sm_openVacuumIn);
            // Backing pump failed
            vacuum()->sm_validateEnableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

        // Open the vacuum valve
        valves()->sm_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenVacuumIn);
            // Valve closed successfully
            valves()->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openFastExhuastPath);
            // Valve failed to close
            valves()->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Open fast exhuast path
        valves()->sm_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenFastExhuastPath);
            // Valve closed successfully
            valves()->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForInitialSystemVacDown);
            // Valve failed to close
            valves()->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Read the pressure
        sml_waitForInitialSystemVacDown.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validateInitialSystemVacuum);
            // Vacuum low enough to move on to the next stage?
            sml_validateInitialSystemVacuum.addTransition(this, &Pressurise::emit_initialSystemVacuumWithinTolerance, &valves()->sm_closeFastExhuastPath);
            // Vacuum too high keep on checking pressure
            sml_validateInitialSystemVacuum.addTransition(this, &Pressurise::emit_initialSystemVacuumTooHigh, &sml_waitForInitialSystemVacDown);

        // Close Fast Exhuast
        valves()->sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFastExhuastPath);
            // Opened the fast exhaust path valve    @ see if statment below
            // Valve failed to close
            valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Do we require a vacumm if below 1.5 bar
        if(params.value("vacuum_backing").toDouble() > params.value("pressure").toDouble())
        {
            // Valve closed successfully
            valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_startExhuastVoidVacDownTimer);

                // Spending some time vacing down the exhuast void as a buffer for next stage
                sml_startExhuastVoidVacDownTimer.addTransition(this, &Pressurise::emit_timerActive, &sml_waitForExhuastVoidVacDown);
                sml_waitForExhuastVoidVacDown.addTransition(&t_exhuastVoidVacDownTimer, &QTimer::timeout, &valves()->sm_closeOutput);

        }
        else
        {
            // Valve closed successfully
            valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeVacuumIn);

                // Close the vacuum input valve
                valves()->sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumIn);
                    // Valve closed successfully
                    valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &vacuum()->sm_disableBackingPump);
                    // Valve failed to close
                    valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

                // Disable the backing pump
                vacuum()->sm_disableBackingPump.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &vacuum()->sm_validateDisableBackingPump);
                    // Validate backing pump on
                    vacuum()->sm_validateDisableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &valves()->sm_openExhuast);
                    // Backing pump failed
                    vacuum()->sm_validateDisableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);

                // Open the exhuast valve and move on
                valves()->sm_openExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenExhuast);
                    // Close the output
                    valves()->sm_validateOpenExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeOutput);
                    // Valve failed to close
                    valves()->sm_validateOpenExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }

        // Set the output valve
        valves()->sm_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseOutput);
            // Close the fast exhuast valve
            valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_startValveTwoTimer);
            // Valve failed to close
            valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);




        // Set timer for valve 2 &timers()->t_vacTime
        sml_startValveTwoTimer.addTransition(this, &Pressurise::emit_timerActive, &sml_startValveOneTimer);

        // Set timer for valve 1
        sml_startValveOneTimer.addTransition(this, &Pressurise::emit_timerActive, &sml_startValveSevenTimer);

        // Set timer for valve 7
        sml_startValveSevenTimer.addTransition(this, &Pressurise::emit_timerActive, &valves()->sm_openHighPressureInput);




        // Open valve 7
        valves()->sm_openHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenHighPressureInput);
            // Valve closed successfully
            valves()->sm_validateOpenHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForValveSevenTimer);
            // Valve failed to close
            valves()->sm_validateOpenHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Wait for timer valve 7
        sml_waitForValveSevenTimer.addTransition(&t_pulseValveSeven, &QTimer::timeout, &sml_closeHighPressureInput_2);

        // Close valve 7
        sml_closeHighPressureInput_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureInput_2);
            // Valve closed successfully
            sml_validateCloseHighPressureInput_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureAfterValveSeven);
            // Valve failed to close
            sml_validateCloseHighPressureInput_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Compare pressure to step size
        sml_waitForPressureAfterValveSeven.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterValveSeven);
            // Too low go back to open valve 7
            sml_validatePressureAfterValveSeven.addTransition(this, &Pressurise::emit_pressureToLow, &valves()->sm_openHighPressureInput);
            // Too high go to pulse valve 2
            sml_validatePressureAfterValveSeven.addTransition(this, &Pressurise::emit_pressureToHigh, &valves()->sm_openSlowExhuastPath);
            // Within tolerence go to open valve 1
            sml_validatePressureAfterValveSeven.addTransition(this, &Pressurise::emit_pressureWithinTolerance, &valves()->sm_openOutput);




        // Open valve 2
        valves()->sm_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenSlowExhuastPath);
            // Valve closed successfully
            valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForValveTwoTimer);
            // Valve failed to close
            valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Wait for timer valve 2
        sml_waitForValveTwoTimer.addTransition(&t_pulseValveTwo, &QTimer::timeout, &sml_closeSlowExhuastPath_2);

        // Close valve 2
        sml_closeSlowExhuastPath_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseSlowExhuastPath_2);
            // Valve closed successfully
            sml_validateCloseSlowExhuastPath_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureAfterValveTwo);
            // Valve failed to close
            sml_validateCloseSlowExhuastPath_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Compare pressure to step size
        sml_waitForPressureAfterValveTwo.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterValveTwo);
            // Too low go back to open valve 7
            sml_validatePressureAfterValveTwo.addTransition(this, &Pressurise::emit_pressureToLow, &valves()->sm_openHighPressureInput);
            // Too high go to pulse valve 2
            sml_validatePressureAfterValveTwo.addTransition(this, &Pressurise::emit_pressureToHigh, &valves()->sm_openSlowExhuastPath);
            // Within tolerence go to open valve 1
            sml_validatePressureAfterValveTwo.addTransition(this, &Pressurise::emit_pressureWithinTolerance, &valves()->sm_openOutput);




        // Open valve 1
        valves()->sm_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenOutput);
            // Valve closed successfully
            valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForValveOneTimer);
            // Valve failed to close
            valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Wait for timer valve 1
        sml_waitForValveOneTimer.addTransition(&t_pulseValveOne, &QTimer::timeout, &sml_closeOutput_2);

        // Close valve 1
        sml_closeOutput_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseOutput_2);
            // Valve closed successfully
            sml_validateCloseOutput_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureAfterValveOne);
            // Valve failed to close
            sml_validateCloseOutput_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Compare pressure to step size
        sml_waitForPressureAfterValveOne.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterValveOne);
            // Calculated new step size is it more than tolerence the go to open valve 2
            sml_validatePressureAfterValveOne.addTransition(this, &Pressurise::emit_pressureToHigh, &valves()->sm_openSlowExhuastPath);
            // Calculated new step size is it less than zero then go to open valve 7
            sml_validatePressureAfterValveOne.addTransition(this, &Pressurise::emit_pressureToLow, &valves()->sm_openHighPressureInput);
            // Calculated new step size is it less than the tolerence then finish
            sml_validatePressureAfterValveOne.addTransition(this, &Pressurise::emit_pressureWithinTolerance, &sm_stop);

    }


    void Pressurise::validateInitialSystemVacuum()
    {
        // Get the validator state instance
        States::CommandValidatorState* state = (States::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

        // Is the pressure lower than required
        if(currentPressure < package.value("vac_down_to").toDouble())
        {
            // Move to next stage
            emit emit_initialSystemVacuumWithinTolerance();

            // No more action required here
            return;
        }

        // Vacuum too high carry on vaccing
        emit emit_initialSystemVacuumTooHigh();
    }


    void Pressurise::validatePressureAfterValveOne()
    {
        // Get the validator state instance
        States::CommandValidatorState* state = (States::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

        // Stop as pressure is correct
        double max = params.value("pressure").toDouble() + params.value("tolerance_valve_one").toDouble();
        double min = params.value("pressure").toDouble() - params.value("tolerance_valve_one").toDouble();

        qDebug() << "VALVE ONE - " << "max pressure: " << max << " Min pressure: " << min << " current pressure: " << currentPressure
                 << "set pressure: " << params.value("pressure").toDouble() << "tollerance: " <<  params.value("tolerance_valve_one").toDouble();

        // pressure within step tolerance
        if(currentPressure < max && currentPressure > min)
        {
            // Open the output value to top the vile up
            emit emit_pressureWithinTolerance();

            // No more action needed here
            return;
        }
        else if (currentPressure > max) // Pressure to high
        {
            // Reduce the pressure with value 2
            emit emit_pressureToHigh();

            // No more action needed here
            return;
        }
        else if (currentPressure < min) // Pressure too low
        {
            // Back to increase the pressure
            emit emit_pressureToLow();

            // No more action needed here
            return;
        }
    }

    void Pressurise::validatePressureAfterValveTwo()
    {
        // Get the validator state instance
        States::CommandValidatorState* state = (States::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

        // Calculate the boundary desired pressure
        double max, min;
        if(params.value("pressure").toDouble() > (pressureReading + params.value("step_size").toDouble()))
        {
            // Calculate the boundary desired pressure
            max = (pressureReading + params.value("step_size").toDouble()) + params.value("tolerance_valve_two").toDouble();
            min = (pressureReading + params.value("step_size").toDouble()) - params.value("tolerance_valve_two").toDouble();
        }
        else
        {
            // Calculate the boundary desired pressure
            max = params.value("pressure").toDouble() + params.value("tolerance_valve_two").toDouble();
            min = params.value("pressure").toDouble() - params.value("tolerance_valve_two").toDouble();
        }

        qDebug() << "VALVE TWO - " << "max pressure: " << max << " Min pressure: " << min << " current pressure: " << currentPressure
                 << "step size: " << params.value("step_size").toDouble() << "tollerance: " <<  params.value("tolerance_valve_two").toDouble();

        // Is the pressure at the correct level with a tolerance
        if(currentPressure < max && currentPressure > min)
        {
            // Save the pressure value
            pressureReading = currentPressure;

            // Open the output value to top the vile up
            emit emit_pressureWithinTolerance();

            // No more action needed here
            return;
        }
        else if (currentPressure > max)
        {
            // Reduce the pressure with value 2
            emit emit_pressureToHigh();

            // No more action needed here
            return;
        }
        else if (currentPressure < min)
        {
            // Back to increase the pressure
            emit emit_pressureToLow();

            // No more action needed here
            return;
        }
    }

    void Pressurise::validatePressureAfterValveSeven()
    {
        // Get the validator state instance
        States::CommandValidatorState* state = (States::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value and convert to mbar
        double currentPressure = package.value("pressure").toDouble() * 1000;

        // Calculate increase
        double max, min;
        if(params.value("pressure").toDouble() > (pressureReading + params.value("step_size").toDouble()))
        {
            // Calculate the boundary desired pressure
            max = (pressureReading + params.value("step_size").toDouble()) + params.value("tolerance_valve_seven").toDouble();
            min = (pressureReading + params.value("step_size").toDouble()) - params.value("tolerance_valve_seven").toDouble();
        }
        else
        {
            // Calculate the boundary desired pressure
            max = params.value("pressure").toDouble() + params.value("tolerance_valve_seven").toDouble();
            min = params.value("pressure").toDouble();
        }

        qDebug() << "VALVE SEVEN - " << "max pressure: " << max << " Min pressure: " << min << " current pressure: " << currentPressure
                 << "step size: " << params.value("step_size").toDouble() << "tollerance: " <<  params.value("tolerance_valve_seven").toDouble();


        // Is the pressure at the correct level with a tolerance
        if(currentPressure < max && currentPressure > min)
        {
            // Save the pressure value
            pressureReading = currentPressure;

            // Open the output value to top the vile up
            emit emit_pressureWithinTolerance();

            // No more action needed here
            return;
        }
        else if (currentPressure > max)
        {
            // Reduce the pressure with value 2
            emit emit_pressureToHigh();

            // No more action needed here
            return;
        }
        else if (currentPressure < min)
        {
            // Back to increase the pressure
            emit emit_pressureToLow();

            // No more action needed here
            return;
        }
    }


    /**
     * Start the timer for valve 1 is to pulse for
     *
     * @brief Pressurise::startValveOnePulseTimer
     */
    void Pressurise::startValveOnePulseTimer()
    {
        if(!t_pulseValveOne.isActive())
        {
            // Setup timer
            t_pulseValveOne.setSingleShot(false);
            t_pulseValveOne.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the timer for valve 1 is to pulse for
     *
     * @brief Pressurise::stopValveOnePulseTimer
     */
    void Pressurise::stopValveOnePulseTimer()
    {
        t_pulseValveOne.stop();
    }


    /**
     * Start the timer for valve 2 is to pulse for
     *
     * @brief Pressurise::startValveTwoPulseTimer
     */
    void Pressurise::startValveTwoPulseTimer()
    {
        if(!t_pulseValveTwo.isActive())
        {
            // Setup timer
            t_pulseValveTwo.setSingleShot(false);
            t_pulseValveTwo.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the timer for valve 2 is to pulse for
     *
     * @brief Pressurise::stopValveTwoPulseTimer
     */
    void Pressurise::stopValveTwoPulseTimer()
    {
        t_pulseValveTwo.stop();
    }



    /**
     * Start the timer for valve 7 is to pulse for
     *
     * @brief Pressurise::startValveSevenPulseTimer
     */
    void Pressurise::startValveSevenPulseTimer()
    {
        if(!t_pulseValveSeven.isActive())
        {
            // Setup timer
            t_pulseValveSeven.setSingleShot(false);
            t_pulseValveSeven.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the timer for valve 7 is to pulse for
     *
     * @brief Pressurise::startValveSevenPulseTimer
     */
    void Pressurise::stopValveSevenPulseTimer()
    {
        t_pulseValveSeven.stop();
    }


    /**
     * Start the timer for  inital vac down
     *
     * @brief Pressurise::startExhuastVoidVacDownTimer
     */
    void Pressurise::startExhuastVoidVacDownTimer()
    {
        if(!t_exhuastVoidVacDownTimer.isActive())
        {
            // Setup timer
            t_exhuastVoidVacDownTimer.setSingleShot(false);
            t_exhuastVoidVacDownTimer.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the timer for valve inital vac down
     *
     * @brief Pressurise::stopExhuastVoidVacDownTimer
     */
    void Pressurise::stopExhuastVoidVacDownTimer()
    {
        t_exhuastVoidVacDownTimer.stop();
    }


}}}







