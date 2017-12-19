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
#include "Functions/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    Pressurise::Pressurise(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers for state machine
        ,   t_pulseValveOne(parent)
        ,   t_pulseValveTwo(parent)
        ,   t_pulseValveSeven(parent)
        ,   t_exhuastVoidVacDownTimer(parent)
        ,   t_vacuumValveTimer(parent)
    {
        // We have stop state machines
        shutDownMachines = true;

        childClassName = QString::fromStdString(typeid(this).name());
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
        // States relating validating pressure
        connect(validator("sml_validatePressureAfterValveOne", true), &Functions::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveOne);
        connect(validator("sml_validatePressureAfterValveTwo", true), &Functions::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveTwo);
        connect(validator("sml_validatePressureAfterValveSeven", true), &Functions::CommandValidatorState::entered, this, &Pressurise::validatePressureAfterValveSeven);
        connect(validator("sml_validatePressureBeforeSelectValve", true), &Functions::CommandValidatorState::entered, this, &Pressurise::validatePressureBeforeSelectValve);

        connect(validator("sml_validatePressureForVacuum", true), &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validatePressureForVacuum);
        connect(validator("sml_validatePressureForVacuumAfterValveOne", true), &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validatePressureForVacuum);

        connect(validator("sml_validateInitialSystemVacuum", true), &Functions::CommandValidatorState::entered, this, &Pressurise::validateInitialSystemVacuum);




        // Misc states
        connect(validator("sml_shouldOpenValveFive", true), &Functions::CommandValidatorState::entered, this, &Pressurise::shouldOpenValveFive);
        connect(validator("sml_shouldCloseValveFive", true), &Functions::CommandValidatorState::entered, this, &Pressurise::shouldCloseValveFive);

        connect(state("sml_shouldEnableBackingPump", true), &QState::entered, this, &Pressurise::shouldEnableBackingPump);
        connect(state("sml_shouldDisablingBackingPump", true), &QState::entered, this, &Pressurise::shouldDisablingBackingPump);

        connect(validator("sml_shouldOpenValveOne", true), &Functions::CommandValidatorState::entered, this, &Pressurise::shouldOpenValveOne);




        // States relating to valve functions
        connect(state("sml_closeHighPressureInput_2", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("sml_closeOutput_2", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("sml_closeSlowExhuastPath_2", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("sml_openVacuumIn_2", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(state("sml_openOutput_2", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(state("sml_openExhuast_2", true), &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(state("sml_closeExhuast_2", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("sml_closeHighPressureNitrogen_2", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("sml_closeVacuumInForSlowExhuast", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("sml_closeVacuumInForSlowExhuast_2", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("sml_openVacuumInForSlowExhuast", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(state("sml_closeHighPressureInput_1", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("sml_closeHighPressureNitrogen_1", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("sml_closeVacuumOut_1", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);
        connect(state("sml_closeFlowController_1", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("sml_closeExhuast_1", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("sml_closeSlowExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("sml_closeFastExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeVacuumIn_1", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("sml_closeOutput_1", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("sml_openSlowExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(state("sml_openOutput_1", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(state("sml_openVacuumIn_1", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(state("sml_openFastExhuastPath_1", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openHighPressureInput_1", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(state("sml_openHighPressureNitrogen_1", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);

        // States relating to validation of valve functions
        connect(validator("sml_validateCloseHighPressureInput_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("sml_validateCloseOutput_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("sml_validateCloseSlowExhuastPath_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("sml_validateOpenVacuumIn_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("sml_validateOpenOutput_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("sml_validateCloseVacuumInForSlowExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("sml_validateCloseVacuumInForSlowExhuast_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("sml_validateOpenVacuumInForSlowExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("sml_validateOpenExhuast_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(validator("sml_validateCloseExhuast_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("sml_validateCloseHighPressureNitrogen_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("sml_validateCloseHighPressureInput_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("sml_validateCloseHighPressureNitrogen_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("sml_validateCloseVacuumOut_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);
        connect(validator("sml_validateCloseFlowController_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("sml_validateCloseExhuast_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("sml_validateCloseSlowExhuastPath_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("sml_validateOpenOutput_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("sml_validateOpenVacuumIn_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("sml_validateOpenFastExhuastPath_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(validator("sml_validateCloseFastExhuastPath_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("sml_validateCloseVacuumIn_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("sml_validateCloseOutput_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("sml_validateOpenSlowExhuastPath_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(validator("sml_validateOpenHighPressureInput_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);
        connect(validator("sml_validateOpenHighPressureNitrogen_1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);



        // States relating to vac pump
        connect(state("sml_enableBackingPump_2", true), &QState::entered, this->vacuum(), &Functions::Vacuum::enableBackingPump);
        connect(state("sml_disableBackingPump_2", true), &QState::entered, this->vacuum(), &Functions::Vacuum::disableBackingPump);
        connect(state("sml_disableTurboPump_1", true), &QState::entered, this->vacuum(), &Functions::Vacuum::disableTurboPump);
        connect(state("sml_enableBackingPump_1", true), &QState::entered, this->vacuum(), &Functions::Vacuum::enableBackingPump);
        connect(state("sml_disableBackingPump_1", true), &QState::entered, this->vacuum(), &Functions::Vacuum::disableBackingPump);


        // States relating to validating vac pump functions
        connect(validator("sml_validateEnableBackingPump_2", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableBackingPump);
        connect(validator("sml_validateDisableBackingPump_2", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableBackingPump);
        connect(validator("sml_validateDisableTurboPump_1", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableTurboPump);
        connect(validator("sml_validateEnableBackingPump_1", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateEnableBackingPump);
        connect(validator("sml_validateDisableBackingPump_1", true), &Functions::CommandValidatorState::entered, this->vacuum(), &Functions::Vacuum::validateDisableBackingPump);



        // Timers
        connect(state("sml_startValveOneTimer", true), &QState::entered, this, &Pressurise::startValveOnePulseTimer);
        connect(state("sml_startValveTwoTimer", true), &QState::entered, this, &Pressurise::startValveTwoPulseTimer);
        connect(state("sml_startValveSevenTimer", true), &QState::entered, this, &Pressurise::startValveSevenPulseTimer);
        connect(state("sml_startExhuastVoidVacDownTimer", true), &QState::entered, this, &Pressurise::startExhuastVoidVacDownTimer);
        connect(state("sml_waitForVacuumValveTimer", true), &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(state("sml_waitForVacuumValveTimer_2", true), &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(state("sml_waitForVacuumValveTimer_3", true), &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(state("sml_waitForVacuumValveTimer_4", true), &QState::entered, this, &Pressurise::startVacuumValveTimer);
        connect(state("sml_waitForVacuumValveTimer_5", true), &QState::entered, this, &Pressurise::startVacuumValveTimer);





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
    void Pressurise::setParams(double pressure, bool initVacDown, int stepSize = 2000, bool inputValve = true, bool outputValve = true)
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

        // Should open the output valve?
        params.insert("open_output_valve", outputValve);

        // What input valve should we use?
        if(inputValve)
        {
            // Open the multi purpose high pressure valve
            inputValveOpen = state("sml_openHighPressureInput_1", true);
            inputValveOpenValidation = validator("sml_validateOpenHighPressureInput_1", true);

            // Close the multi purpose high pressure valve
            inputValveClose = state("sml_closeHighPressureInput_2", true);
            inputValveCloseValidation = validator("sml_validateCloseHighPressureInput_2", true);
        }
        else
        {
            // Open the nitrogen high pressure valve
            inputValveOpen = state("sml_openHighPressureNitrogen_1", true);
            inputValveOpenValidation = validator("sml_validateOpenHighPressureNitrogen_1", true);

            // Close the nitrogen high pressure valve
            inputValveClose = state("sml_closeHighPressureNitrogen_2", true);
            inputValveCloseValidation = validator("sml_validateCloseHighPressureNitrogen_2", true);
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
        params.insert("tolerance_final", finalTol);

        // What is the tolerance of valve 2
        int valveTwoTol = finalTol + (pressure * 0.002);
        if(valveTwoTol > 250)
            valveTwoTol = 250;
        params.insert("tolerance_valve_two", 500);
        params.insert("tolerance_valve_two_step", 500);
        params.insert("tolerance_valve_two_final", valveTwoTol);

        // What is the tolerance of valve 7
        params.insert("tolerance_valve_seven", 500);
        params.insert("tolerance_valve_seven_step", 500);
        params.insert("tolerance_valve_seven_final", valveTwoTol);





        /*#######################################
         # Valve inital pulsing speeds
         ######################################*/

        // Starting speed to pulse valve 7
        params.insert("valve_7_pulse", 20);

        // Starting speed to pulse valve 2
        params.insert("valve_2_pulse", 20);

        // Speed to pulse valve 1
        params.insert("valve_1_pulse", 10000); // was 5000

        // Between valve 2 how long should vac down be when setting pressure below 1.5bar
        params.insert("exhuast_void_vac_down_time_pulse", 100);





        /*#######################################
         # Valve Pulse Speed Tunner
         ######################################*/

        // Valve 7: Desired pressure increase between pulses
        // Valve 2: Desired pressure increase between pulses for normal stage and final stage
        params.insert("valve_7_normal_step_size", 300);
        params.insert("valve_7_final_step_size", 50);
        params.insert("valve_7_step_size", params.value("valve_7_normal_step_size").toInt());


        // Valve 2: Desired pressure increase between pulses for normal stage and final stage
        params.insert("valve_2_normal_step_size", 80);
        params.insert("valve_2_final_step_size", 10);
        params.insert("valve_2_step_size", params.value("valve_2_normal_step_size").toInt());




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
    void Pressurise::beforeStart()
    {
        // Reset vars
        pressureReading = 1;
        backingPumpEnabled = false;
        pressureReading = 1;
        exhuastValvePressureChange.clear();
        inputValvePressureChange.clear();
    }


    /**
     * Start the state machine
     *
     * @brief Pressurise::stopped
     */
    void Pressurise::stopped()
    {
        // Stop timers
        t_pulseValveOne.stop();
        t_pulseValveTwo.stop();
        t_pulseValveSeven.stop();
        t_exhuastVoidVacDownTimer.stop();
        t_vacuumValveTimer.stop();
    }



    /**
     * Builds the shut down state machine
     *
     * @brief Pressurise::buildShutDownMachine
     */
    void Pressurise::buildShutDownMachine()
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
     * @brief Pressurise::buildMachine
     */
    void Pressurise::buildMachine()
    {
        // Where to start the machine
        (params.value("disable_init_vac_down").toBool()) ? machine.setInitialState(state("sml_closeHighPressureInput_1", true)) : machine.setInitialState(state("sml_waitForInitalPressure", true)) ;

        // Check the system pressure
        transitionsBuilder()->validatePressure(state("sml_waitForInitalPressure", true), validator("sml_validatePressureForVacuum", true), state("sml_closeHighPressureInput_1", true), &sm_stopAsFailed);

        // Close the high pressure valve
        transitionsBuilder()->closeValve(state("sml_closeHighPressureInput_1", true), validator("sml_validateCloseHighPressureInput_1", true), state("sml_closeHighPressureNitrogen_1", true), &sm_stopAsFailed);

        // Close the nitrogen valve
        transitionsBuilder()->closeValve(state("sml_closeHighPressureNitrogen_1", true), validator("sml_validateCloseHighPressureNitrogen_1", true), state("sml_closeVacuumOut_1", true), &sm_stopAsFailed);

        // Close Vacuum Output
        transitionsBuilder()->closeValve(state("sml_closeVacuumOut_1", true), validator("sml_validateCloseVacuumOut_1", true), state("sml_closeFlowController_1", true), &sm_stopAsFailed);

        // Close the flow controller valve
        transitionsBuilder()->closeValve(state("sml_closeFlowController_1", true), validator("sml_validateCloseFlowController_1", true), state("sml_closeExhuast_1", true), &sm_stopAsFailed);

        // Close the exhuast valve
        transitionsBuilder()->closeValve(state("sml_closeExhuast_1", true), validator("sml_validateCloseExhuast_1", true), state("sml_closeSlowExhuastPath_1", true), &sm_stopAsFailed);

        // Close Slow Exhuast
        state("sml_closeSlowExhuastPath_1", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("sml_validateCloseSlowExhuastPath_1", true));
            // Wrong signal was picked up
            validator("sml_validateCloseSlowExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationWrongId, state("sml_closeSlowExhuastPath_1", true));
            // Open the fast exhaust path valve
            if(!params.value("disable_init_vac_down").toBool()){
                validator("sml_validateCloseSlowExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openOutput_1", true));
            }else{
                validator("sml_validateCloseSlowExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeFastExhuastPath_1", true));
            }
            // Valve failed to close
            validator("sml_validateCloseSlowExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

        // Should we perform init vac down
        if(!params.value("disable_init_vac_down").toBool())
        {
            // Open output
            transitionsBuilder()->openValve(state("sml_openOutput_1", true), validator("sml_validateOpenOutput_1", true), state("sml_waitForPressureAfterInitValveOne", true), &sm_stopAsFailed);

            // Check the pressure now the output is open
            transitionsBuilder()->validatePressure(state("sml_waitForPressureAfterInitValveOne", true), validator("sml_validatePressureForVacuumAfterValveOne", true), state("sml_disableTurboPump_1", true), &sm_stopAsFailed);

            // Disable the vac station turbo
            transitionsBuilder()->disableTurboPump(state("sml_disableTurboPump_1", true), validator("sml_validateDisableTurboPump_1", true), state("sml_enableBackingPump_1", true), state("sml_enableBackingPump_1", true), &sm_stopAsFailed);

            // Enabled the backing pump
            transitionsBuilder()->enableBackingPump(state("sml_enableBackingPump_1", true), validator("sml_validateEnableBackingPump_1", true), state("sml_openVacuumIn_1", true), &sm_stopAsFailed);

            // Open the vacuum valve
            transitionsBuilder()->openValve(state("sml_openVacuumIn_1", true), validator("sml_validateOpenVacuumIn_1", true), state("sml_openFastExhuastPath_1", true), &sm_stopAsFailed);

            // Open fast exhuast path
            transitionsBuilder()->openValve(state("sml_openFastExhuastPath_1", true), validator("sml_validateOpenFastExhuastPath_1", true), state("sml_waitForInitialSystemVacDown", true), &sm_stopAsFailed);

            // Read the pressure
            state("sml_waitForInitialSystemVacDown", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validateInitialSystemVacuum", true));
                // Vacuum low enough to move on to the next stage?
                validator("sml_validateInitialSystemVacuum", true)->addTransition(this, &Pressurise::emit_initialSystemVacuumWithinTolerance, state("sml_closeFastExhuastPath_1", true));
                // Vacuum too high keep on checking pressure
                validator("sml_validateInitialSystemVacuum", true)->addTransition(this, &Pressurise::emit_initialSystemVacuumTooHigh, state("sml_waitForInitialSystemVacDown", true));
        }

        // Close Fast Exhuast
        state("sml_closeFastExhuastPath_1", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("sml_validateCloseFastExhuastPath_1", true));
            // Wrong signal was picked up
            validator("sml_validateCloseFastExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationWrongId, state("sml_closeFastExhuastPath_1", true));
            // Opened the fast exhaust path valve    @ see if statment below
            // Valve failed to close
            validator("sml_validateCloseFastExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Should we vac down the exhuast cavity or open exhuast valve 3?
        if(params.value("vacuum_backing").toDouble() > params.value("pressure").toDouble())
        {
            // Valve closed successfully
            validator("sml_validateCloseFastExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_startExhuastVoidVacDownTimer", true));

                // Spending some time vacing down the exhuast void as a buffer for next stage
                state("sml_startExhuastVoidVacDownTimer", true)->addTransition(this, &Pressurise::emit_timerActive, state("sml_waitForExhuastVoidVacDown", true));
                    state("sml_waitForExhuastVoidVacDown", true)->addTransition(&t_exhuastVoidVacDownTimer, &QTimer::timeout, state("sml_closeOutput_1", true));
        }
        else
        {
            // Valve closed successfully
            validator("sml_validateCloseFastExhuastPath_1", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeVacuumIn_1", true));

                // Close the vacuum input valve
                transitionsBuilder()->closeValve(state("sml_closeVacuumIn_1", true), validator("sml_validateCloseVacuumIn_1", true), state("sml_disableBackingPump_1", true), &sm_stopAsFailed);

                // Disable the backing pump
                transitionsBuilder()->disableBackingPump(state("sml_disableBackingPump_1", true), validator("sml_validateDisableBackingPump_1", true), state("sml_closeOutput_1", true), &sm_stopAsFailed);
        }

        // Set the output valve
        transitionsBuilder()->closeValve(state("sml_closeOutput_1", true), validator("sml_validateCloseOutput_1", true), state("sml_waitForPressureBeforeSelectValve", true), &sm_stopAsFailed);






        // Set inital valve to use
        state("sml_waitForPressureBeforeSelectValve", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validatePressureBeforeSelectValve", true));
            // Too low go to open valve 7
            validator("sml_validatePressureBeforeSelectValve", true)->addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Too high go to pulse valve 2
            validator("sml_validatePressureBeforeSelectValve", true)->addTransition(this, &Pressurise::emit_pressureToHigh, state("sml_waitForPressureBeforeValveFive", true));
            // Within tolerence go to open valve 1
            validator("sml_validatePressureBeforeSelectValve", true)->addTransition(this, &Pressurise::emit_pressureWithinTolerance, validator("sml_shouldOpenValveOne", true));








        // Open valve 7
        transitionsBuilder()->openValve(inputValveOpen, inputValveOpenValidation, state("sml_startValveSevenTimer", true), &sm_stopAsFailed);

        // Wait for timer valve 7
        state("sml_startValveSevenTimer", true)->addTransition(&t_pulseValveSeven, &QTimer::timeout, inputValveClose);

        // Close valve 7
        transitionsBuilder()->closeValve(inputValveClose, inputValveCloseValidation, state("sml_waitForPressureAfterValveSeven", true), &sm_stopAsFailed);

        // Compare pressure to step size
        state("sml_waitForPressureAfterValveSeven", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validatePressureAfterValveSeven", true));
            // Too low go back to open valve 7
            validator("sml_validatePressureAfterValveSeven", true)->addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Too high go to pulse valve 2
            validator("sml_validatePressureAfterValveSeven", true)->addTransition(this, &Pressurise::emit_pressureToHigh, state("sml_waitForPressureBeforeValveFive", true));
            // Within tolerence go to open valve 1
            validator("sml_validatePressureAfterValveSeven", true)->addTransition(this, &Pressurise::emit_pressureWithinTolerance, validator("sml_shouldOpenValveOne", true));





        // Request current pressure
        state("sml_waitForPressureBeforeValveFive", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_shouldCloseValveFive", true));

        // Should vacuum in valve be closed and exhuast opened?
        validator("sml_shouldCloseValveFive", true)->addTransition(this, &Pressurise::emit_shouldCloseValveFiveFalse, state("sml_closeVacuumInForSlowExhuast_2", true));
            // Close vacuum in
            transitionsBuilder()->openValve(state("sml_closeVacuumInForSlowExhuast_2", true), validator("sml_validateCloseVacuumInForSlowExhuast_2", true), state("sml_waitForVacuumValveTimer_5", true), &sm_stopAsFailed);
                // Wait for valve to close
                state("sml_waitForVacuumValveTimer_5", true)->addTransition(&t_vacuumValveTimer, &QTimer::timeout, state("sml_shouldDisablingBackingPump", true));
                    // Take note that we're disabling the backing pump
                    state("sml_shouldDisablingBackingPump", true)->addTransition(this, &Pressurise::emit_shouldDisableBackingPumpFalse, state("sml_openExhuast_2", true));
                    state("sml_shouldDisablingBackingPump", true)->addTransition(this, &Pressurise::emit_shouldDisableBackingPumpTrue, state("sml_disableBackingPump_2", true));
                        // Disable backing pump
                        transitionsBuilder()->disableBackingPump(state("sml_disableBackingPump_2", true), validator("sml_validateDisableBackingPump_2", true), state("sml_openExhuast_2", true), &sm_stopAsFailed);
                    // Open exhuast valve 3
                    transitionsBuilder()->openValve(state("sml_openExhuast_2", true), validator("sml_validateOpenExhuast_2", true), state("sml_openSlowExhuastPath_1", true), &sm_stopAsFailed);


        // Should exhuast be closed and vacuum enabled.
        validator("sml_shouldCloseValveFive", true)->addTransition(this, &Pressurise::emit_shouldCloseValveFiveTrue, state("sml_closeExhuast_2", true));
            // Close exhuast valve 3
            transitionsBuilder()->closeValve(state("sml_closeExhuast_2", true), validator("sml_validateCloseExhuast_2", true), state("sml_closeVacuumInForSlowExhuast", true), &sm_stopAsFailed);
                // Wait for valve to close
                //sml_waitForVacuumValveTimer_4.addTransition(&t_vacuumValveTimer, &QTimer::timeout, &sml_closeVacuumInForSlowExhuast);
                // Close valve 5
                transitionsBuilder()->closeValve(state("sml_closeVacuumInForSlowExhuast", true), validator("sml_validateCloseVacuumInForSlowExhuast", true), state("sml_waitForVacuumValveTimer_2", true), &sm_stopAsFailed);
                    // Wait for valve to close
                    state("sml_waitForVacuumValveTimer_2", true)->addTransition(&t_vacuumValveTimer, &QTimer::timeout, state("sml_shouldEnableBackingPump", true));
                        // Skip any action on the backing pump
                        state("sml_shouldEnableBackingPump", true)->addTransition(this, &Pressurise::emit_shouldEnableBackingPumpSkip, state("sml_openSlowExhuastPath_1", true));
                        // Backing pump needs enabling
                        state("sml_shouldEnableBackingPump", true)->addTransition(this, &Pressurise::emit_shouldEnableBackingPumpTrue, state("sml_enableBackingPump_2", true));
                            // Enable backing pump
                            transitionsBuilder()->enableBackingPump(state("sml_enableBackingPump_2", true), validator("sml_validateEnableBackingPump_2", true), state("sml_openSlowExhuastPath_1", true), &sm_stopAsFailed);

        // Open valve 2
        transitionsBuilder()->openValve(state("sml_openSlowExhuastPath_1", true), validator("sml_validateOpenSlowExhuastPath_1", true), state("sml_startValveTwoTimer", true), &sm_stopAsFailed);

        // Wait for timer valve 2
        state("sml_startValveTwoTimer", true)->addTransition(&t_pulseValveTwo, &QTimer::timeout, state("sml_closeSlowExhuastPath_2", true));

        // Close valve 2
        transitionsBuilder()->closeValve(state("sml_closeSlowExhuastPath_2", true), validator("sml_validateCloseSlowExhuastPath_2", true), state("sml_waitForPressureAfterValveFive", true), &sm_stopAsFailed);

        // Request current pressure
        state("sml_waitForPressureAfterValveFive", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_shouldOpenValveFive", true));

        // Should vacuum in valve be opened?
        validator("sml_shouldOpenValveFive", true)->addTransition(this, &Pressurise::emit_shouldOpenValveFiveFalse, state("sml_waitForPressureAfterValveTwo", true));
        // Open vacuum in valve and ensure backing pump on
        validator("sml_shouldOpenValveFive", true)->addTransition(this, &Pressurise::emit_shouldOpenValveFiveTrue, state("sml_waitForVacuumValveTimer_3", true));
            // Wait for valve to open
            state("sml_waitForVacuumValveTimer_3", true)->addTransition(&t_vacuumValveTimer, &QTimer::timeout, state("sml_openVacuumInForSlowExhuast", true));
                // Open the vacumm in valve
                transitionsBuilder()->openValve(state("sml_openVacuumInForSlowExhuast", true), validator("sml_validateOpenVacuumInForSlowExhuast", true), state("sml_waitForVacuumValveTimer", true), &sm_stopAsFailed);
                        // Allow vaccum in to vac down exhaust void for some time
                        state("sml_waitForVacuumValveTimer", true)->addTransition(&t_vacuumValveTimer, &QTimer::timeout, state("sml_waitForPressureAfterValveTwo", true));

        // Compare pressure to step size
        state("sml_waitForPressureAfterValveTwo", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validatePressureAfterValveTwo", true));
            // Too low go back to open valve 7
            validator("sml_validatePressureAfterValveTwo", true)->addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Too high go to pulse valve 2
            validator("sml_validatePressureAfterValveTwo", true)->addTransition(this, &Pressurise::emit_pressureToHigh, state("sml_waitForPressureBeforeValveFive", true));
            // Within tolerence go to open valve 1
            validator("sml_validatePressureAfterValveTwo", true)->addTransition(this, &Pressurise::emit_pressureWithinTolerance, validator("sml_shouldOpenValveOne", true));








        // Should the output be open?
        validator("sml_shouldOpenValveOne", true)->addTransition(this, &Pressurise::emit_shouldOpenValveOneFalse, state("sml_waitForPressureAfterValveOne", true));
        validator("sml_shouldOpenValveOne", true)->addTransition(this, &Pressurise::emit_shouldOpenValveOneTrue, state("sml_openOutput_2", true));

        // Open valve 1
        transitionsBuilder()->openValve(state("sml_openOutput_2", true), validator("sml_validateOpenOutput_2", true), state("sml_startValveOneTimer", true), &sm_stopAsFailed);

        // Wait for timer valve 1
        state("sml_startValveOneTimer", true)->addTransition(&t_pulseValveOne, &QTimer::timeout, state("sml_closeOutput_2", true));

        // Close valve 1
        transitionsBuilder()->openValve(state("sml_closeOutput_2", true), validator("sml_validateCloseOutput_2", true), state("sml_waitForPressureAfterValveOne", true), &sm_stopAsFailed);

        // Compare pressure to step size
        state("sml_waitForPressureAfterValveOne", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validatePressureAfterValveOne", true));
            // Calculated new step size is it more than tolerence the go to open valve 2
            validator("sml_validatePressureAfterValveOne", true)->addTransition(this, &Pressurise::emit_pressureToHigh, state("sml_waitForPressureBeforeValveFive", true));
            // Calculated new step size is it less than zero then go to open valve 7
            validator("sml_validatePressureAfterValveOne", true)->addTransition(this, &Pressurise::emit_pressureToLow, inputValveOpen);
            // Calculated new step size is it less than the tolerence then finish
            validator("sml_validatePressureAfterValveOne", true)->addTransition(this, &Pressurise::emit_pressureWithinTolerance, &sm_stop);

    }

    void Pressurise::shouldOpenValveOne()
    {
        if(params.value("open_output_valve").toBool() == true)
            emit emit_shouldOpenValveOneTrue();

        emit emit_shouldOpenValveOneFalse();
    }

    void Pressurise::validatePressureBeforeSelectValve()
    {
        // Get the validator state instance
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

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
        qDebug() << "should enable backing pump: " << backingPumpEnabled;
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
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

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
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

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
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;

        // Stop as pressure is correct
        double max = params.value("pressure").toDouble() + params.value("tolerance_final").toDouble();
        double min = params.value("pressure").toDouble() - params.value("tolerance_final").toDouble();

        qDebug() << "VALVE ONE - " << "max pressure: " << max << " Min pressure: " << min << " current pressure: " << currentPressure
                 << "set pressure: " << params.value("pressure").toDouble() << "tollerance: " <<  params.value("tolerance_final").toDouble();

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
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value
        double currentPressure = package.value("pressure").toDouble() * 1000;


        // Set pressure decrease wanted
        if(abs(params.value("pressure").toDouble() - currentPressure) < 500 )
        {
            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_2_step_size", params.value("valve_2_final_step_size").toInt());
        }
        else
        {
            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_2_step_size", params.value("valve_2_normal_step_size").toInt());
        }


        // Calculate the boundary desired pressure
        double max, min;
        if(abs(params.value("pressure").toDouble() - currentPressure - 1000) < params.value("step_size").toDouble())
        {
            // Set the new valve tolerance
            params.insert("tolerance_valve_two", params.value("tolerance_valve_two_final").toInt());

            // Increment smaller than step size
            // Calculate the boundary desired pressure
            max = params.value("pressure").toDouble() + params.value("tolerance_valve_two").toDouble();
            min = params.value("pressure").toDouble() - params.value("tolerance_valve_two").toDouble();
        }
        else if(params.value("pressure").toDouble() > (pressureReading + params.value("step_size").toDouble() /*- 1000*/))
        {
            // Set the new valve tolerance
            params.insert("tolerance_valve_two", params.value("tolerance_valve_two_step").toInt());

            // Calculate the boundary desired pressure
            max = (pressureReading + params.value("step_size").toDouble()) + params.value("tolerance_valve_two").toDouble();
            min = (pressureReading + params.value("step_size").toDouble()) - params.value("tolerance_valve_two").toDouble();

        }
        else if(params.value("pressure").toDouble() < (pressureReading - params.value("step_size").toDouble() /*- 1000*/))
        {
            // Set the new valve tolerance
            params.insert("tolerance_valve_two", params.value("tolerance_valve_two_step").toInt());

            // Calculate the boundary desired pressure
            max = (pressureReading - params.value("step_size").toDouble()) + params.value("tolerance_valve_two").toDouble();
            min = (pressureReading - params.value("step_size").toDouble()) - params.value("tolerance_valve_two").toDouble();
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
            t_pulseValveTwo.setInterval(tuneValveSpeed(currentPressure, exhuastValvePressureChange, params.value("valve_2_step_size").toDouble(), t_pulseValveTwo.interval()));

            // Reduce the pressure with value 2
            emit emit_pressureToHigh();

            // No more action needed here
            return;
        }
        else if (currentPressure < min)
        {
            // Reset valve time
            //t_pulseValveTwo.setInterval(params.value("valve_2_pulse").toInt());
            t_pulseValveTwo.setInterval(tuneValveSpeed(currentPressure, exhuastValvePressureChange, params.value("valve_2_step_size").toDouble(), t_pulseValveTwo.interval()));

            // Back to increase the pressure
            emit emit_pressureToLow();

            // No more action needed here
            return;
        }
    }


    void Pressurise::validatePressureAfterValveSeven()
    {
        // Get the validator state instance
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Current pressure value and convert to mbar
        double currentPressure = package.value("pressure").toDouble() * 1000;


        // Set pressure decrease wanted
        if(abs(params.value("pressure").toDouble() - currentPressure) < 500 )
        {
            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_7_step_size", params.value("valve_7_final_step_size").toInt());
        }
        else
        {
            // If on the pressure tunning stage decrease vavle tunning params
            params.insert("valve_7_step_size", params.value("valve_7_normal_step_size").toInt());
        }


        // Calculate increase
        double max, min;
        if(abs(params.value("pressure").toDouble() - currentPressure) < params.value("step_size").toDouble())
        {
            // Set the new valve tolerance
            params.insert("tolerance_valve_seven", params.value("tolerance_valve_seven_final").toInt());

            // Increment is smaller than steps size
            // Calculate the boundary desired pressure
            max = params.value("pressure").toDouble() + params.value("tolerance_valve_seven").toDouble();
            min = params.value("pressure").toDouble();
        }
        else if(params.value("pressure").toDouble() > (pressureReading + params.value("step_size").toDouble() - 1000))
        {
            // Set the new valve tolerance
            params.insert("tolerance_valve_seven", params.value("tolerance_valve_seven_step").toInt());

            // Calculate the boundary desired pressure
            max = (pressureReading + params.value("step_size").toDouble()) + params.value("tolerance_valve_seven").toDouble();
            min = (pressureReading + params.value("step_size").toDouble()) - params.value("tolerance_valve_seven").toDouble();
        }
        else if(params.value("pressure").toDouble() < (pressureReading + params.value("step_size").toDouble() - 1000))
        {
            // Set the new valve tolerance
            params.insert("tolerance_valve_seven", params.value("tolerance_valve_seven_step").toInt());

            // Calculate the boundary desired pressure
            max = (pressureReading - params.value("step_size").toDouble()) + params.value("tolerance_valve_seven").toDouble();
            min = (pressureReading - params.value("step_size").toDouble()) - params.value("tolerance_valve_seven").toDouble();
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
            // Tune valve time
            //t_pulseValveSeven.setInterval(params.value("valve_7_pulse").toInt());
            t_pulseValveSeven.setInterval(tuneValveSpeed(currentPressure, inputValvePressureChange, params.value("valve_7_step_size").toDouble(), t_pulseValveSeven.interval()));


            // Reduce the pressure with value 2
            emit emit_pressureToHigh();

            // No more action needed here
            return;
        }
        else if (currentPressure < min)
        {
            // Tune valve time
            t_pulseValveSeven.setInterval(tuneValveSpeed(currentPressure, inputValvePressureChange, params.value("valve_7_step_size").toDouble(), t_pulseValveSeven.interval()));

            // Back to increase the pressure
            emit emit_pressureToLow();

            // No more action needed here
            return;
        }
    }



    int Pressurise::tuneValveSpeed(double currentPressure, QList<double>& previousPressures, double desiredStepSize, int currentSpeed)
    {
        // Add the new value
        previousPressures.append(currentPressure);

        // We need a sample of three to make decisions
        if(previousPressures.size() <= 3)
        {
            return currentSpeed;
        }
        else if(previousPressures.size() == 4)
        {
            // Remove the first value
            previousPressures.pop_front();
        }

        // Calculate the current average pressure change
        double pressureAverage = 0;
        for (int i = 0; i < previousPressures.size(); ++i) {
            pressureAverage = pressureAverage + previousPressures.at(i);
        }
        pressureAverage = pressureAverage / previousPressures.size();
        double pressureChangeAverage = abs(pressureAverage - currentPressure);


        // Find pressure difference for most recent pressure change
        double lastPressureChange = abs(previousPressures.at(previousPressures.size() - 1) - currentPressure);

        // If its 30% of the desired step size then do not tune valve speed
        if( lastPressureChange < (desiredStepSize + (desiredStepSize * 0.3)) && lastPressureChange > (desiredStepSize - (desiredStepSize * 0.3)) )
        {
            // Return current speed as timing is perfect
            return currentSpeed;
        }



        // Should the valve speed be incremented by a corse value?
        if(pressureChangeAverage < (desiredStepSize * 0.2))
        {
            return minMaxValveSpeed(currentSpeed + 10);
        }

        // Should the valve speed be incremented by a fine valve?
        if(pressureChangeAverage < (desiredStepSize * 0.8))
        {
            return minMaxValveSpeed(currentSpeed + 1);
        }

        // Should the valve speed be decremented?
        if(pressureChangeAverage > (desiredStepSize + pow(log(desiredStepSize), 2.5) + 20))
        {
            return minMaxValveSpeed(currentSpeed - 5);
        }
        else if(pressureChangeAverage > desiredStepSize)
        {
            return minMaxValveSpeed(currentSpeed - 2);
        }

        // If not decision then return current valve speed;
        return currentSpeed;
    }


    int Pressurise::minMaxValveSpeed(int timing)
    {
        if(timing < 4)
            return 4;

        if(timing > 2000)
            return 2000;

        return timing;
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

}}}







