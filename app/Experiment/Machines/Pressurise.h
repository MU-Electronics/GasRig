#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "States/MachineStates.h"

// Include valiator
#include "States/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines
{
    class Pressurise    :   public States::MachineStates
    {
        Q_OBJECT

        public:
            Pressurise(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~Pressurise();

            void setParams(double pressure, bool initVacDown, int stepSize, bool inputValve);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

            void connectStatesToMethods();


            // Holds the application settings
            Settings::Container m_settings;

            // Hold the pressure for the previous stage
            double pressureReading = 1;

            // Hold the state of the backing pump
            bool backingPumpEnabled = false;

            // Exhuast mode
            bool exhuastMode = false;

            // Timers for state machine
            QTimer  t_pulseValveOne
            ,       t_pulseValveTwo
            ,       t_pulseValveSeven
            ,       t_exhuastVoidVacDownTimer
            ,       t_vacuumValveTimer;


            // Valve to open and close
            QState* inputValveOpen;
            States::CommandValidatorState* inputValveOpenValidation;
            QState* inputValveClose;
            States::CommandValidatorState* inputValveCloseValidation;


            QState
                    // Timer related states
                    sml_startValveOneTimer
            ,       sml_startValveTwoTimer
            ,       sml_startValveSevenTimer
            ,       sml_startExhuastVoidVacDownTimer
            ,       sml_waitForExhuastVoidVacDown
            ,       sml_waitForVacuumValveTimer
            ,       sml_waitForVacuumValveTimer_2
            ,       sml_waitForVacuumValveTimer_3
            ,       sml_waitForVacuumValveTimer_4
            ,       sml_waitForVacuumValveTimer_5
            ,       sml_shouldEnableBackingPump
            ,       sml_shouldDisablingBackingPump
          //,       sml_waitForInitalVacDown



                    // States relating to read pressure
            ,       sml_waitForPressureAfterValveOne
            ,       sml_waitForPressureAfterValveTwo
            ,       sml_waitForPressureAfterValveSeven
            ,       sml_waitForInitalPressure
            ,       sml_waitForPressureAfterInitValveOne
            ,       sml_waitForPressureBeforeValveFive
            ,       sml_waitForPressureAfterValveFive
            ,       sml_waitForPressureBeforeSelectValve
            ,       sml_waitForInitialSystemVacDown



                    // States relating to valve functions
            ,       sml_openHighPressureInput_1
            ,       sml_openHighPressureNitrogen_1
            ,       sml_closeHighPressureInput_1
            ,       sml_closeHighPressureNitrogen_1
            ,       sml_closeVacuumOut_1
            ,       sml_closeFlowController_1
            ,       sml_closeExhuast_1
            ,       sml_closeSlowExhuastPath_1
            ,       sml_closeFastExhuastPath_1
            ,       sml_closeVacuumIn_1
            ,       sml_closeOutput_1
            ,       sml_openSlowExhuastPath_1
            ,       sml_openOutput_1
            ,       sml_openVacuumIn_1
            ,       sml_openFastExhuastPath_1
            ,       sml_openExhuast_2
            ,       sml_closeExhuast_2
            ,       sml_closeSlowExhuastPath_2
            ,       sml_closeOutput_2
            ,       sml_closeHighPressureInput_2
            ,       sml_openVacuumIn_2
            ,       sml_openOutput_2
            ,       sml_closeHighPressureNitrogen_2
            ,       sml_closeVacuumInForSlowExhuast
            ,       sml_openVacuumInForSlowExhuast
            ,       sml_closeVacuumInForSlowExhuast_2



                    // States relating to vac pump
            ,       sml_enableBackingPump_2
            ,       sml_disableBackingPump_2
            ,       sml_disableTurboPump_1
            ,       sml_enableBackingPump_1
            ,       sml_disableBackingPump_1;


            States::CommandValidatorState
                    // Validator Misc
                    sml_validatePressureAfterValveSeven
            ,       sml_validatePressureAfterValveTwo
            ,       sml_validatePressureAfterValveOne
            ,       sml_validatePressureBeforeSelectValve
            ,       sml_shouldOpenValveFive
            ,       sml_shouldCloseValveFive
            ,       sml_validateInitialSystemVacuum



                    // States relating to pressure
            ,       sml_validatePressureForVacuum
            ,       sml_validatePressureForVacuumAfterValveOne



                    // States realting to vacuum station validation
            ,       sml_validateDisableTurboPump_1
            ,       sml_validateEnableBackingPump_1
            ,       sml_validateDisableBackingPump_1

                    // States relating to validation of valve functions
            ,       sml_validateOpenHighPressureInput_1
            ,       sml_validateOpenHighPressureNitrogen_1
            ,       sml_validateCloseHighPressureInput_1
            ,       sml_validateCloseHighPressureNitrogen_1
            ,       sml_validateCloseVacuumOut_1
            ,       sml_validateCloseFlowController_1
            ,       sml_validateCloseExhuast_1
            ,       sml_validateCloseSlowExhuastPath_1
            ,       sml_validateOpenOutput_1
            ,       sml_validateOpenVacuumIn_1
            ,       sml_validateOpenFastExhuastPath_1
            ,       sml_validateCloseFastExhuastPath_1
            ,       sml_validateCloseVacuumIn_1
            ,       sml_validateCloseOutput_1
            ,       sml_validateOpenSlowExhuastPath_1
            ,       sml_validateCloseVacuumInForSlowExhuast
            ,       sml_validateOpenVacuumInForSlowExhuast
            ,       sml_validateCloseVacuumInForSlowExhuast_2
            ,       sml_validateCloseHighPressureInput_2
            ,       sml_validateCloseSlowExhuastPath_2
            ,       sml_validateCloseOutput_2
            ,       sml_validateOpenVacuumIn_2
            ,       sml_validateOpenOutput_2
            ,       sml_validateOpenExhuast_2
            ,       sml_validateCloseExhuast_2
            ,       sml_validateCloseHighPressureNitrogen_2



                    // States relating to validating vac pump functions
            ,       sml_validateEnableBackingPump_2
            ,       sml_validateDisableBackingPump_2;

        signals:
            void emit_pressuriseFinished(QVariantMap params);
            void emit_pressuriseFailed(QVariantMap params);

            // State signals
            void emit_pressureToLow();
            void emit_pressureToHigh();
            void emit_pressureWithinTolerance();

            void emit_initialSystemVacuumTooHigh();
            void emit_initialSystemVacuumWithinTolerance();

            void emit_shouldOpenValveFiveTrue();
            void emit_shouldOpenValveFiveFalse();

            void emit_shouldCloseValveFiveTrue();
            void emit_shouldCloseValveFiveFalse();

            void emit_shouldEnableBackingPumpTrue();
            void emit_shouldEnableBackingPumpSkip();

            void emit_shouldDisableBackingPumpTrue();
            void emit_shouldDisableBackingPumpFalse();

            // Timer signals
            void emit_timerActive();

        public slots:
            // Validator states
            void validatePressureAfterValveOne();
            void validatePressureAfterValveTwo();
            void validatePressureAfterValveSeven();
            void validateInitialSystemVacuum();
            void validatePressureBeforeSelectValve();
            void shouldOpenValveFive();
            void shouldCloseValveFive();
            void shouldEnableBackingPump();
            void shouldDisablingBackingPump();

            // Timers
            void startValveOnePulseTimer();
            void startValveTwoPulseTimer();
            void startValveSevenPulseTimer();
            void startExhuastVoidVacDownTimer();
            void startVacuumValveTimer();

            void stopValveOnePulseTimer();
            void stopValveTwoPulseTimer();
            void stopValveSevenPulseTimer();
            void stopExhuastVoidVacDownTimer();
            void stopVacuumValveTimer();

    };
}}}



