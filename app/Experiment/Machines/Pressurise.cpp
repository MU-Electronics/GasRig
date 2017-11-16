#include "Pressurise.h"

#include <cmath>

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
        ,   sml_waitForPressureAfterInitValveOne(&machine)
        ,   sml_waitForPressureBeforeValveFive(&machine)
        ,   sml_waitForPressureAfterValveFive(&machine)
        ,   sml_waitForPressureBeforeSelectValve(&machine)

        ,   sml_waitForValveOneTimer(&machine)
        ,   sml_waitForValveTwoTimer(&machine)
        ,   sml_waitForValveSevenTimer(&machine)
        ,   sml_waitForExhuastVoidVacDown(&machine)

        ,   sml_shouldOpenValveFive(&machine)
        ,   sml_shouldCloseValveFive(&machine)

        ,   sml_shouldEnableBackingPump(&machine)
        ,   sml_shouldDisablingBackingPump(&machine)

            // Copy states that are used more than once to make then unique
        ,   sml_closeHighPressureInput_2(&machine)
        ,   sml_closeSlowExhuastPath_2(&machine)
        ,   sml_closeOutput_2(&machine)
        ,   sml_openVacuumIn_2(&machine)
        ,   sml_openOutput_2(&machine)
        ,   sml_closeVacuumInForSlowExhuast(&machine)
        ,   sml_openVacuumInForSlowExhuast(&machine)
        ,   sml_closeVacuumInForSlowExhuast_2(&machine)
        ,   sml_closeHighPressureNitrogen_2(&machine)

        ,   sml_openExhuast_2(&machine)
        ,   sml_closeExhuast_2(&machine)

        ,   sml_validateCloseSlowExhuastPath_2(&machine)
        ,   sml_validateCloseHighPressureInput_2(&machine)
        ,   sml_validateCloseOutput_2(&machine)
        ,   sml_validateOpenVacuumIn_2(&machine)
        ,   sml_validateOpenOutput_2(&machine)
        ,   sml_validatePressureForVacuumAfterValveOne(&machine)
        ,   sml_validateCloseVacuumInForSlowExhuast(&machine)
        ,   sml_validateOpenVacuumInForSlowExhuast(&machine)
        ,   sml_validateOpenExhuast_2(&machine)
        ,   sml_validateCloseExhuast_2(&machine)
        ,   sml_validateCloseVacuumInForSlowExhuast_2(&machine)
        ,   sml_validateCloseHighPressureNitrogen_2(&machine)


        ,   sml_enableBackingPump_2(&machine)
        ,   sml_disableBackingPump_2(&machine)


        ,   sml_validateEnableBackingPump_2(&machine)
        ,   sml_validateDisableBackingPump_2(&machine)


            // Addtional validator states for state machine
        ,   sml_validatePressureAfterValveSeven(&machine)
        ,   sml_validatePressureAfterValveTwo(&machine)
        ,   sml_validatePressureAfterValveOne(&machine)
        ,   sml_validateInitialSystemVacuum(&machine)
        ,   sml_validatePressureBeforeSelectValve(&machine)

            // Timer states
        ,   sml_startValveOneTimer(&machine)
        ,   sml_startValveTwoTimer(&machine)
        ,   sml_startValveSevenTimer(&machine)
        ,   sml_startExhuastVoidVacDownTimer(&machine)
        ,   sml_waitForVacuumValveTimer(&machine)
        ,   sml_waitForVacuumValveTimer_2(&machine)
        ,   sml_waitForVacuumValveTimer_3(&machine)
        ,   sml_waitForVacuumValveTimer_4(&machine)
        ,   sml_waitForVacuumValveTimer_5(&machine)

            // Timers for state machine
        ,   t_pulseValveOne(parent)
        ,   t_pulseValveTwo(parent)
        ,   t_pulseValveSeven(parent)
        ,   t_exhuastVoidVacDownTimer(parent)
        ,   t_vacuumValveTimer(parent)
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
        connect(&sml_waitForVacuumValveTimer, &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(&sml_waitForVacuumValveTimer_2, &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(&sml_waitForVacuumValveTimer_3, &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(&sml_waitForVacuumValveTimer_4, &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(&sml_waitForVacuumValveTimer_5, &QState::entered, this, &Pressurise::startVacuumValveTimer);


        // Copy states that are used more than once to make then unique
        connect(&sml_closeHighPressureInput_2, &QState::entered, this->valves(), &States::Valves::closeHighPressureInput);
        connect(&sml_closeOutput_2, &QState::entered, this->valves(), &States::Valves::closeOutput);
        connect(&sml_closeSlowExhuastPath_2, &QState::entered, this->valves(), &States::Valves::closeSlowExhuastPath);
        connect(&sml_openVacuumIn_2, &QState::entered, this->valves(), &States::Valves::openVacuumIn);
        connect(&sml_openOutput_2, &QState::entered, this->valves(), &States::Valves::openOutput);
        connect(&sml_openExhuast_2, &QState::entered, this->valves(), &States::Valves::openExhuast);
        connect(&sml_closeExhuast_2, &QState::entered, this->valves(), &States::Valves::closeExhuast);
        connect(&sml_closeHighPressureNitrogen_2, &QState::entered, this->valves(), &States::Valves::closeHighPressureNitrogen);

        connect(&sml_validateCloseHighPressureInput_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseHighPressureInput);
        connect(&sml_validateCloseOutput_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseOutput);
        connect(&sml_validateCloseSlowExhuastPath_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseSlowExhuastPath);
        connect(&sml_validateOpenVacuumIn_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateOpenVacuumIn);
        connect(&sml_validateOpenOutput_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateOpenOutput);
        connect(&sml_validatePressureForVacuumAfterValveOne, &States::CommandValidatorState::entered, this->pressure(), &States::Pressure::validatePressureForVacuum);

        connect(&sml_shouldOpenValveFive, &States::CommandValidatorState::entered, this, &Pressurise::shouldOpenValveFive);
        connect(&sml_shouldCloseValveFive, &States::CommandValidatorState::entered, this, &Pressurise::shouldCloseValveFive);

        connect(&sml_closeVacuumInForSlowExhuast, &QState::entered, this->valves(), &States::Valves::closeVacuumIn);
        connect(&sml_closeVacuumInForSlowExhuast_2, &QState::entered, this->valves(), &States::Valves::closeVacuumIn);
        connect(&sml_openVacuumInForSlowExhuast, &QState::entered, this->valves(), &States::Valves::openVacuumIn);

        connect(&sml_shouldEnableBackingPump, &QState::entered, this, &Pressurise::shouldEnableBackingPump);
        connect(&sml_shouldDisablingBackingPump, &QState::entered, this, &Pressurise::shouldDisablingBackingPump);

        connect(&sml_enableBackingPump_2, &QState::entered, this->vacuum(), &States::Vacuum::enableBackingPump);
        connect(&sml_disableBackingPump_2, &QState::entered, this->vacuum(), &States::Vacuum::disableBackingPump);

        // Addtional validator states
        connect(&sml_validatePressureAfterValveOne, &States::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveOne);
        connect(&sml_validatePressureAfterValveTwo, &States::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveTwo);
        connect(&sml_validatePressureAfterValveSeven, &States::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveSeven);
        connect(&sml_validatePressureBeforeSelectValve, &States::CommandValidatorState::entered, this, &Pressurise::validatePressureBeforeSelectValve);
        connect(&sml_validateInitialSystemVacuum, &States::CommandValidatorState::entered, this, &Pressurise::validateInitialSystemVacuum);
        connect(&sml_validateCloseVacuumInForSlowExhuast, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseVacuumIn);
        connect(&sml_validateCloseVacuumInForSlowExhuast_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseVacuumIn);
        connect(&sml_validateOpenVacuumInForSlowExhuast, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateOpenVacuumIn);
        connect(&sml_validateOpenExhuast_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateOpenExhuast);
        connect(&sml_validateCloseExhuast_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseExhuast);
        connect(&sml_validateCloseHighPressureNitrogen_2, &States::CommandValidatorState::entered, this->valves(), &States::Valves::validateCloseHighPressureNitrogen);


        connect(&sml_validateEnableBackingPump_2, &States::CommandValidatorState::entered, this->vacuum(), &States::Vacuum::validateEnableBackingPump);
        connect(&sml_validateDisableBackingPump_2, &States::CommandValidatorState::entered, this->vacuum(), &States::Vacuum::validateDisableBackingPump);

    }



    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param pressure, set the end pressure
     * @param initVacDown, should we vac down the output first?
     * @param stepSize, what pressure increases for the output
     * @param inputValve, which input valve to use?   MHG = true, Nitrogen in = false
     */
    // Below show the to be implimented function vars
    // Valve 7 init step size, Valve 7 final step size, Valve 2 init step size, Valve 2 final step size
    // Valve 1 tolerance, Valve 2 tolerance, Valve 7 tolerance
    void Pressurise::setParams(double pressure, bool initVacDown, int stepSize = 2000, bool inputValve = true)
    {
        /*#######################################
         # Configuration Settings
         ######################################*/

        // What is the target pressure
        params.insert("pressure", pressure);

        // Disable init pressure check and vac down
        params.insert("disable_init_vac_down", initVacDown);

        // What is the step size in pressure
        params.insert("step_size", stepSize);

        // What input valve should we use?
        if(inputValve)
        {
            // Open the multi purpose high pressure valve
            inputValveOpen = &valves()->sm_openHighPressureInput;
            inputValveOpenValidation = &valves()->sm_validateOpenHighPressureInput;

            // Close the multi purpose high pressure valve
            inputValveClose = &sml_closeHighPressureInput_2;
            inputValveCloseValidation = &sml_validateCloseHighPressureInput_2;
        }
        else
        {
            // Open the nitrogen high pressure valve
            inputValveOpen = &valves()->sm_openHighPressureNitrogen;
            inputValveOpenValidation = &valves()->sm_validateOpenHighPressureNitrogen;

            // Close the nitrogen high pressure valve
            inputValveClose = &sml_closeHighPressureNitrogen_2;
            inputValveCloseValidation = &sml_validateCloseHighPressureNitrogen_2;
        }





        /*#######################################
         # Vacuum Backing Settings
         ######################################*/

        // When do we need a vacuum backing for the exhaust
        params.insert("vacuum_backing", 4000);

        // Vac down the exhuast void to provide a initial buffer
        params.insert("exhuast_void_vac_down_time", 5000);

        // Initall vac down system to in mbar
        params.insert("vac_down_to", 20);





        /*#######################################
         # Tolerance Settings
         ######################################*/

        // What is the tolerance of valve 1 (end tolerance, 0.2% or 30 mBar)
        int finalTol = pressure * 0.002;
        if(finalTol < 30)
            finalTol = 30;
        if(pressure < 2000)
            finalTol = 10;
        params.insert("tolerance_valve_one", finalTol);

        // What is the tolerance of valve 2
        int valveTwoTol = finalTol + (pressure * 0.002);
        if(valveTwoTol > 250)
            valveTwoTol = 250;
        params.insert("tolerance_valve_two", valveTwoTol);

        // What is the tolerance of valve 7
        params.insert("tolerance_valve_seven", 500);






        /*#######################################
         # Valve inital pulsing speeds
         ######################################*/

        // Starting speed to pulse valve 7
        params.insert("valve_7_pulse", 20);

        // Starting speed to pulse valve 2
        params.insert("valve_2_pulse", 20);

        // Speed to pulse valve 1
        params.insert("valve_1_pulse", 5000);


        // Between valve 2 how long should vac down be when setting pressure below 1.5bar
        params.insert("exhuast_void_vac_down_time_pulse", 300);





        /*#######################################
         # Valve Pulse Speed Tunner
         ######################################*/

        // Valve 7: Desired pressure increase between pulses
        // Valve 2: Desired pressure increase between pulses for normal stage and final stage
        params.insert("valve_7_normal_step_size", 400);
        params.insert("valve_7_final_step_size", 100);
        params.insert("valve_7_step_size", params.value("valve_7_normal_step_size").toInt());

        // Valve 7: Tolerence for desired pressure increase between pulses
        params.insert("valve_7_step_size_tolerance", 30);

        // Valve 7: Increment pulse width when pressure increase was too small / no change
        params.insert("valve_7_increment", 5);

        // Valve 7: Decrement pulse width when pressure increase was too large
        params.insert("valve_7_decrement", 5);

        // Valve 7: Previous tunning pressure
        params.insert("valve_7_previous_pressure", -1);



        // Valve 2: Desired pressure increase between pulses for normal stage and final stage
        params.insert("valve_2_normal_step_size", 80);
        params.insert("valve_2_final_step_size", 10);
        params.insert("valve_2_step_size", params.value("valve_2_normal_step_size").toInt());

        // Valve 2: Tolerence for desired pressure increase between pulses
        params.insert("valve_2_step_size_tolerance", 10);

        // Valve 2: Increment pulse width when pressure increase was too small / no change
        params.insert("valve_2_increment", 5);

        // Valve 2: Corse increment pulse width when pressure increase was too small / no change
        params.insert("valve_2_increment_corse", 20);

        // Valve 2: Fine increment pulse width when pressure increase needs to be small
        params.insert("valve_2_increment_fine", 2);

        // Valve 2: Decrement pulse width when pressure increase was too large
        params.insert("valve_2_decrement", 5);

        // Valve 2: Previous tunning pressure
        params.insert("valve_2_previous_pressure", -1);



        /*#######################################
         # Timer Settings
         ######################################*/

        // Set the timer for the pulse width for valve 1
        t_pulseValveOne.setInterval(params.value("valve_1_pulse").toInt());

        // Set the timer for the pulse width for valve 2
        t_pulseValveTwo.setInterval(params.value("valve_2_pulse").toInt());

        // Set the timer for the pulse width for valve 7
        t_pulseValveSeven.setInterval(params.value("valve_7_pulse").toInt());


        // Set the timer for the inital vac down time
        t_exhuastVoidVacDownTimer.setInterval(params.value("exhuast_void_vac_down_time").toInt());


        // Set timer for valve 5 (vac in)
        t_vacuumValveTimer.setInterval(params.value("exhuast_void_vac_down_time_pulse").toInt());


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
        stopExhuastVoidVacDownTimer();
        stopVacuumValveTimer();

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
        stopExhuastVoidVacDownTimer();
        stopVacuumValveTimer();

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
        (params.value("disable_init_vac_down").toBool()) ? machine.setInitialState(&valves()->sm_closeHighPressureInput) : machine.setInitialState(&sml_waitForInitalPressure) ;

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
            if(!params.value("disable_init_vac_down").toBool()){
                valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openOutput);
            }else{
                valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeFastExhuastPath);
            }
            // Valve failed to close
            valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Should we perform init vac down
        if(!params.value("disable_init_vac_down").toBool())
        {
            // Open output
            valves()->sm_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenOutput);
                // Valve closed successfully
                valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureAfterInitValveOne);
                // Valve failed to close
                valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Check the pressure now the output is open
            sml_waitForPressureAfterInitValveOne.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureForVacuumAfterValveOne);
                // Pressure is low enough
                sml_validatePressureForVacuumAfterValveOne.addTransition(this->pressure(), &States::Pressure::emit_validationSuccess, &vacuum()->sm_disableTurboPump);
                // Pressure is too high
                sml_validatePressureForVacuumAfterValveOne.addTransition(this->pressure(), &States::Pressure::emit_validationFailed, &sm_stopAsFailed);

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
        }

        // Close Fast Exhuast
        valves()->sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFastExhuastPath);
            // Opened the fast exhaust path valve    @ see if statment below
            // Valve failed to close
            valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Should we vac down the exhuast cavity or open exhuast valve 3?
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
                    vacuum()->sm_validateDisableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &valves()->sm_closeOutput);
                    // Backing pump failed
                    vacuum()->sm_validateDisableBackingPump.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);
        }

        // Set the output valve
        valves()->sm_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseOutput);
            // Close the fast exhuast valve
            valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureBeforeSelectValve);
            // Valve failed to close
            valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);






        // Set inital valve to use
        sml_waitForPressureBeforeSelectValve.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureBeforeSelectValve);
            // Too low go to open valve 7
            sml_validatePressureBeforeSelectValve.addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Too high go to pulse valve 2
            sml_validatePressureBeforeSelectValve.addTransition(this, &Pressurise::emit_pressureToHigh, &sml_waitForPressureBeforeValveFive);
            // Within tolerence go to open valve 1
            sml_validatePressureBeforeSelectValve.addTransition(this, &Pressurise::emit_pressureWithinTolerance, &sml_openOutput_2);








        // Open valve 7
        inputValveOpen->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, inputValveOpenValidation);
            // Valve closed successfully
            inputValveOpenValidation->addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_startValveSevenTimer);
            // Valve failed to close
            inputValveOpenValidation->addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Wait for timer valve 7
        sml_startValveSevenTimer.addTransition(&t_pulseValveSeven, &QTimer::timeout, inputValveClose);

        // Close valve 7
        inputValveClose->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, inputValveCloseValidation);
            // Valve closed successfully
            inputValveCloseValidation->addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureAfterValveSeven);
            // Valve failed to close
            inputValveCloseValidation->addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Compare pressure to step size
        sml_waitForPressureAfterValveSeven.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterValveSeven);
            // Too low go back to open valve 7
            sml_validatePressureAfterValveSeven.addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Too high go to pulse valve 2
            sml_validatePressureAfterValveSeven.addTransition(this, &Pressurise::emit_pressureToHigh, &sml_waitForPressureBeforeValveFive);
            // Within tolerence go to open valve 1
            sml_validatePressureAfterValveSeven.addTransition(this, &Pressurise::emit_pressureWithinTolerance, &sml_openOutput_2);





        // Request current pressure
        sml_waitForPressureBeforeValveFive.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_shouldCloseValveFive);

        // Should vacuum in valve be closed and exhuast opened?
        sml_shouldCloseValveFive.addTransition(this, &Pressurise::emit_shouldCloseValveFiveFalse, &sml_closeVacuumInForSlowExhuast_2);
            // Close vacuum in
            sml_closeVacuumInForSlowExhuast_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumInForSlowExhuast_2);
                // Validate valive 5 closed
                sml_validateCloseVacuumInForSlowExhuast_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
                sml_validateCloseVacuumInForSlowExhuast_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForVacuumValveTimer_5);
                    // Wait for valve to close
                    sml_waitForVacuumValveTimer_5.addTransition(&t_vacuumValveTimer, &QTimer::timeout, &sml_shouldDisablingBackingPump);
                        // Take note that we're disabling the backing pump
                        sml_shouldDisablingBackingPump.addTransition(this, &Pressurise::emit_shouldDisableBackingPumpFalse, &sml_openExhuast_2);
                        sml_shouldDisablingBackingPump.addTransition(this, &Pressurise::emit_shouldDisableBackingPumpTrue, &sml_disableBackingPump_2);
                            // Disable backing pump
                            sml_disableBackingPump_2.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &sml_validateDisableBackingPump_2);
                                // Backing pump failed
                                sml_validateDisableBackingPump_2.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);
                                // Validate backing pump on
                                sml_validateDisableBackingPump_2.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &sml_openExhuast_2);
                        // Open exhuast valve 3
                        sml_openExhuast_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenExhuast_2);
                            // Valve failed to close
                            sml_validateOpenExhuast_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
                            // Close the output
                            sml_validateOpenExhuast_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openSlowExhuastPath);

        // Should exhuast be closed and vacuum enabled.
        sml_shouldCloseValveFive.addTransition(this, &Pressurise::emit_shouldCloseValveFiveTrue, &sml_closeExhuast_2);
            // Close exhuast valve 3
            sml_closeExhuast_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseExhuast_2);
                // Valve failed to close
                sml_validateCloseExhuast_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
                // Close the output
                sml_validateCloseExhuast_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_closeVacuumInForSlowExhuast);
                    // Wait for valve to close
                    //sml_waitForVacuumValveTimer_4.addTransition(&t_vacuumValveTimer, &QTimer::timeout, &sml_closeVacuumInForSlowExhuast);
                    // Close valve 5
                    sml_closeVacuumInForSlowExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumInForSlowExhuast);
                        // Validate valive 5 closed
                        sml_validateCloseVacuumInForSlowExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
                        sml_validateCloseVacuumInForSlowExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForVacuumValveTimer_2);
                            // Wait for valve to close
                            sml_waitForVacuumValveTimer_2.addTransition(&t_vacuumValveTimer, &QTimer::timeout, &sml_shouldEnableBackingPump);
                                // Skip any action on the backing pump
                                sml_shouldEnableBackingPump.addTransition(this, &Pressurise::emit_shouldEnableBackingPumpSkip, &valves()->sm_openSlowExhuastPath);
                                // Backing pump needs enabling
                                sml_shouldEnableBackingPump.addTransition(this, &Pressurise::emit_shouldEnableBackingPumpTrue, &sml_enableBackingPump_2);
                                    // Enable backing pump
                                    sml_enableBackingPump_2.addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, &sml_validateEnableBackingPump_2);
                                        // Backing pump failed
                                        sml_validateEnableBackingPump_2.addTransition(this->vacuum(), &States::Vacuum::emit_validationFailed, &sm_stopAsFailed);
                                        // Validate backing pump on
                                        sml_validateEnableBackingPump_2.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &valves()->sm_openSlowExhuastPath);

        // Open valve 2
        valves()->sm_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenSlowExhuastPath);
            // Valve closed successfully
            valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_startValveTwoTimer);
            // Valve failed to close
            valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Wait for timer valve 2
        sml_startValveTwoTimer.addTransition(&t_pulseValveTwo, &QTimer::timeout, &sml_closeSlowExhuastPath_2);

        // Close valve 2
        sml_closeSlowExhuastPath_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseSlowExhuastPath_2);
            // Valve closed successfully
            sml_validateCloseSlowExhuastPath_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureAfterValveFive);
            // Valve failed to close
            sml_validateCloseSlowExhuastPath_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Request current pressure
        sml_waitForPressureAfterValveFive.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_shouldOpenValveFive);

        // Should vacuum in valve be opened?
        sml_shouldOpenValveFive.addTransition(this, &Pressurise::emit_shouldOpenValveFiveFalse, &sml_waitForPressureAfterValveTwo);
        // Open vacuum in valve and ensure backing pump on
        sml_shouldOpenValveFive.addTransition(this, &Pressurise::emit_shouldOpenValveFiveTrue, &sml_waitForVacuumValveTimer_3);
            // Wait for valve to open
            sml_waitForVacuumValveTimer_3.addTransition(&t_vacuumValveTimer, &QTimer::timeout, &sml_openVacuumInForSlowExhuast);
                // Open the vacumm in valve
                sml_openVacuumInForSlowExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenVacuumInForSlowExhuast);
                    // Validate the vacuum in valve
                    sml_validateOpenVacuumInForSlowExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForVacuumValveTimer);
                    sml_validateOpenVacuumInForSlowExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
                        // Allow vaccum in to vac down exhaust void for some time
                        sml_waitForVacuumValveTimer.addTransition(&t_vacuumValveTimer, &QTimer::timeout, &sml_waitForPressureAfterValveTwo);

        // Compare pressure to step size
        sml_waitForPressureAfterValveTwo.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterValveTwo);
            // Too low go back to open valve 7
            sml_validatePressureAfterValveTwo.addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Too high go to pulse valve 2
            sml_validatePressureAfterValveTwo.addTransition(this, &Pressurise::emit_pressureToHigh, &sml_waitForPressureBeforeValveFive);
            // Within tolerence go to open valve 1
            sml_validatePressureAfterValveTwo.addTransition(this, &Pressurise::emit_pressureWithinTolerance, &sml_openOutput_2);








        // Open valve 1
        sml_openOutput_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenOutput_2);
            // Valve closed successfully
            sml_validateOpenOutput_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_startValveOneTimer);
            // Valve failed to close
            sml_validateOpenOutput_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Wait for timer valve 1
        sml_startValveOneTimer.addTransition(&t_pulseValveOne, &QTimer::timeout, &sml_closeOutput_2);

        // Close valve 1
        sml_closeOutput_2.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseOutput_2);
            // Valve closed successfully
            sml_validateCloseOutput_2.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sml_waitForPressureAfterValveOne);
            // Valve failed to close
            sml_validateCloseOutput_2.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Compare pressure to step size
        sml_waitForPressureAfterValveOne.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterValveOne);
            // Calculated new step size is it more than tolerence the go to open valve 2
            sml_validatePressureAfterValveOne.addTransition(this, &Pressurise::emit_pressureToHigh, &sml_waitForPressureBeforeValveFive);
            // Calculated new step size is it less than zero then go to open valve 7
            sml_validatePressureAfterValveOne.addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Calculated new step size is it less than the tolerence then finish
            sml_validatePressureAfterValveOne.addTransition(this, &Pressurise::emit_pressureWithinTolerance, &sm_stop);

    }

    void Pressurise::validatePressureBeforeSelectValve()
    {
        // Get the validator state instance
        States::CommandValidatorState* state = (States::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

        // Save the pressure value
        pressureReading = currentPressure;

        if(currentPressure < params.value("pressure").toDouble())
        {
            emit emit_pressureToLow();
            return;
        }
        else if(currentPressure > params.value("pressure").toDouble())
        {
            emit emit_pressureToHigh();
            return;
        }
        else if(currentPressure == params.value("pressure").toDouble())
        {
            emit emit_pressureWithinTolerance();
            return;
        }
    }

    void Pressurise::shouldDisablingBackingPump()
    {
        if(backingPumpEnabled)
        {
            backingPumpEnabled = false;
            emit emit_shouldDisableBackingPumpTrue();
        }

        emit emit_shouldDisableBackingPumpFalse();
    }

    void Pressurise::shouldEnableBackingPump()
    {
        if(!backingPumpEnabled)
        {
            // Save that we're about to enable the backing pump
            backingPumpEnabled = true;

            emit emit_shouldEnableBackingPumpTrue();

            return;
        }

        emit emit_shouldEnableBackingPumpSkip();
    }

    void Pressurise::shouldOpenValveFive()
    {
        if(exhuastMode)
        {
            emit emit_shouldOpenValveFiveTrue();
            return;
        }

        emit emit_shouldOpenValveFiveFalse();
    }

    void Pressurise::shouldCloseValveFive()
    {
        // Get the validator state instance
        States::CommandValidatorState* state = (States::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

        if(params.value("vacuum_backing").toDouble() > currentPressure)
        {
            emit emit_shouldCloseValveFiveTrue();
            exhuastMode = true;
            return;
        }
        exhuastMode = false;
        emit emit_shouldCloseValveFiveFalse();
    }

    void Pressurise::validateInitialSystemVacuum()
    {
        // Get the validator state instance
        States::CommandValidatorState* state = (States::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

        qDebug() << currentPressure << params.value("vac_down_to").toDouble();

        // Is the pressure lower than required
        if(currentPressure < params.value("vac_down_to").toDouble())
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

            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_2_step_size", params.value("valve_2_normal_step_size").toInt());
        }
        else if(params.value("pressure").toDouble() < (pressureReading - params.value("step_size").toDouble()))
        {
            // Calculate the boundary desired pressure
            max = (pressureReading - params.value("step_size").toDouble()) + params.value("tolerance_valve_two").toDouble();
            min = (pressureReading - params.value("step_size").toDouble()) - params.value("tolerance_valve_two").toDouble();

            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_2_step_size", params.value("valve_2_normal_step_size").toInt());
        }
        else
        {
            // Calculate the boundary desired pressure
            max = params.value("pressure").toDouble() + params.value("tolerance_valve_two").toDouble();
            min = params.value("pressure").toDouble() - params.value("tolerance_valve_two").toDouble();

            // If on the pressure tunning stage decrease vavle tunning params
            qDebug() << "Value two step size switches to small: " << params.value("valve_2_final_step_size").toInt();
            params.insert("valve_2_step_size", params.value("valve_2_final_step_size").toInt());

        }

        qDebug() << "VALVE TWO - " << "max pressure: " << max << " Min pressure: " << min << " current pressure: " << currentPressure
                 << "step size: " << params.value("step_size").toDouble() << "tollerance: " <<  params.value("tolerance_valve_two").toDouble()
                 << "valve speed: " << t_pulseValveTwo.interval();

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
            // Previous tunning pressure
            int previousPressure = params.value("valve_2_previous_pressure").toInt();

            qDebug() << "Tunning valve two, pressure change: " << abs(previousPressure - currentPressure) << " target step size: " << params.value("valve_2_step_size").toInt();

            // Find pressure difference
            double pressureDiff = abs(previousPressure - currentPressure);

            // Desired step size
            double stepSize = params.value("valve_2_step_size").toDouble();

            // Change the valve timing in the correct direction
            if( pressureDiff < (stepSize + (stepSize * 0.3)) && pressureDiff > (stepSize - (stepSize * 0.3)))
            {
                // Do nothing the step size is perfect
                qDebug() << "Tune doing nothing";
            }
            else if(pressureDiff < stepSize) // Step size to small
            {
                // Increase by default amount
                int inc = params.value("valve_2_increment").toInt();

                // Increase by corse or fine?
                if(abs(previousPressure - currentPressure) < (params.value("valve_2_step_size").toInt() * 0.05))
                {
                    inc = params.value("valve_2_increment_corse").toInt();
                }
                else if(abs(previousPressure - currentPressure) < (params.value("valve_2_step_size").toInt() * 0.2))
                {
                    inc = params.value("valve_2_increment_fine").toInt();
                }

                // Save new speed
                t_pulseValveTwo.setInterval(t_pulseValveTwo.interval() + inc);
            }
            else if(pressureDiff > stepSize) // Step size too large reduce speed
            {
                // Decrease the timing and save the new speed
                t_pulseValveTwo.setInterval(t_pulseValveTwo.interval() - params.value("valve_2_decrement").toInt());
            }


            // Update the previous pressure with current pressure
            params.insert("valve_2_previous_pressure", currentPressure);

            // Reduce the pressure with value 2
            emit emit_pressureToHigh();

            // No more action needed here
            return;
        }
        else if (currentPressure < min)
        {
            // Reset valve time
            t_pulseValveTwo.setInterval(params.value("valve_2_pulse").toInt());

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

            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_7_step_size", params.value("valve_7_normal_step_size").toInt());
        }
        else
        {
            // Calculate the boundary desired pressure
            max = params.value("pressure").toDouble() + params.value("tolerance_valve_seven").toDouble();
            min = params.value("pressure").toDouble();

            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_7_step_size", params.value("valve_7_final_step_size").toInt());
        }

        qDebug() << "VALVE SEVEN - " << "max pressure: " << max << " Min pressure: " << min << " current pressure: " << currentPressure
                 << "step size: " << params.value("step_size").toDouble() << "tollerance: " <<  params.value("tolerance_valve_seven").toDouble()
                 << "valve speed" << t_pulseValveSeven.interval();


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
            // Reset valve time
            t_pulseValveSeven.setInterval(params.value("valve_7_pulse").toInt());

            // Reduce the pressure with value 2
            emit emit_pressureToHigh();

            // No more action needed here
            return;
        }
        else if (currentPressure < min)
        {
            // Previous tunning pressure
            int previousPressure = params.value("valve_7_previous_pressure").toInt();

            // Does the valve timing need tunning?
            int currentMinPressure = currentPressure - params.value("valve_7_step_size_tolerance").toInt();
            int currentMaxPressure = currentPressure + params.value("valve_7_step_size_tolerance").toInt();
            if( previousPressure != -1 &&
                !(
                    (abs(previousPressure - currentMinPressure) <= params.value("valve_7_step_size").toInt()) &&
                    (abs(previousPressure - currentMaxPressure) >= params.value("valve_7_step_size").toInt())
                )
            )
            {
                // Change the valve timing in the correct direction
                if(abs(previousPressure - currentPressure) < params.value("valve_7_step_size").toInt())
                { // Step size too small
                    t_pulseValveSeven.setInterval(t_pulseValveSeven.interval() + params.value("valve_7_increment").toInt());
                }
                else if(abs(previousPressure - currentPressure) > params.value("valve_7_step_size").toInt())
                { // Step size too large
                    t_pulseValveSeven.setInterval(t_pulseValveSeven.interval() - params.value("valve_7_decrement").toInt());
                }
            }

            // Update the previous pressure with current pressure
            params.insert("valve_7_previous_pressure", currentPressure);

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
            t_pulseValveOne.setSingleShot(true);
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
            t_pulseValveTwo.setSingleShot(true);
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
            t_pulseValveSeven.setSingleShot(true);
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



    /**
     * Start the timer for vacuum in valve
     *
     * @brief Pressurise::startVacuumValveTimer
     */
    void Pressurise::startVacuumValveTimer()
    {
        if(!t_vacuumValveTimer.isActive())
        {
            // Setup timer
            t_vacuumValveTimer.setSingleShot(true);
            t_vacuumValveTimer.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the timer for vacuum in valve
     *
     * @brief Pressurise::stopVacuumValveTimer
     */
    void Pressurise::stopVacuumValveTimer()
    {
        t_vacuumValveTimer.stop();
    }
}}}







