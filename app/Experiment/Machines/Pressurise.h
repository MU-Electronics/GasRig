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

            void setParams(double pressure, bool initVacDown);

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





            QState  sml_startValveOneTimer
            ,       sml_startValveTwoTimer
            ,       sml_startValveSevenTimer
            ,       sml_startExhuastVoidVacDownTimer
            ,       sml_waitForVacuumValveTimer
            ,       sml_waitForVacuumValveTimer_2
            ,       sml_waitForVacuumValveTimer_3
            ,       sml_waitForVacuumValveTimer_4
            ,       sml_waitForVacuumValveTimer_5

            ,       sml_waitForPressureAfterValveOne
            ,       sml_waitForPressureAfterValveTwo
            ,       sml_waitForPressureAfterValveSeven
            ,       sml_waitForInitalPressure
            ,       sml_waitForExhuastVoidVacDown
            ,       sml_waitForPressureAfterInitValveOne
            ,       sml_waitForPressureBeforeValveFive
            ,       sml_waitForPressureAfterValveFive

            ,       sml_waitForValveOneTimer
            ,       sml_waitForValveTwoTimer
            ,       sml_waitForValveSevenTimer
            ,       sml_waitForInitalVacDown
            ,       sml_waitForInitialSystemVacDown

            ,       sml_openExhuast_2
            ,       sml_closeExhuast_2
            ,       sml_closeSlowExhuastPath_2
            ,       sml_closeOutput_2
            ,       sml_closeHighPressureInput_2
            ,       sml_openVacuumIn_2
            ,       sml_openOutput_2

            ,       sml_enableBackingPump_2

            ,       sml_closeVacuumInForSlowExhuast
            ,       sml_openVacuumInForSlowExhuast
            ,       sml_closeVacuumInForSlowExhuast_2

            ,       sml_shouldEnableBackingPump
            ,       sml_disableBackingPump_2
            ,       sml_shouldDisablingBackingPump;


            States::CommandValidatorState
                    sml_validatePressureAfterValveSeven
            ,       sml_validatePressureAfterValveTwo
            ,       sml_validatePressureAfterValveOne

            ,       sml_shouldOpenValveFive
            ,       sml_shouldCloseValveFive

            ,       sml_validatePressureForVacuumAfterValveOne

            ,       sml_validateInitialSystemVacuum

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



