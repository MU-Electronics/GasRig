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
#include "Functions/MachineStates.h"

// Include valiator
#include "Functions/CommandValidatorState.h"

#include <QList>

namespace App { namespace Experiment { namespace Machines
{
    class Pressurise    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            Pressurise(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~Pressurise();

            void setParams(double pressure, bool initVacDown, int stepSize, bool inputValve, bool outputValve);

            void start();

            void stopped();

            void buildMachine();

            void buildShutDownMachine();

            void connectStatesToMethods();


            // Holds the application settings
            Settings::Container m_settings;

            // Hold the pressure for the previous stage
            double pressureReading = 1;

            // Hold the state of the backing pump
            bool backingPumpEnabled = false;

            // Exhuast mode
            bool exhuastMode = false;

            // Pressure changes cache
            QList<double> exhuastValvePressureChange;
            QList<double> inputValvePressureChange;

            // Timers for state machine
            QTimer  t_pulseValveOne
            ,       t_pulseValveTwo
            ,       t_pulseValveSeven
            ,       t_exhuastVoidVacDownTimer
            ,       t_vacuumValveTimer;


            // Valve to open and close
            QState* inputValveOpen;
            Functions::CommandValidatorState* inputValveOpenValidation;
            QState* inputValveClose;
            Functions::CommandValidatorState* inputValveCloseValidation;

            // Valve tuning function
            int tuneValveSpeed(double currentPressure, QList<double>& previousPressures, double desiredStepSize, int currentSpeed);

        signals:
            // State signals
            void emit_pressureToLow();
            void emit_pressureToHigh();
            void emit_pressureWithinTolerance();

            void emit_initialSystemVacuumTooHigh();
            void emit_initialSystemVacuumWithinTolerance();

            void emit_shouldOpenValveOneTrue();
            void emit_shouldOpenValveOneFalse();

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
            void shouldOpenValveOne();

            // Timers
            void startValveOnePulseTimer();
            void startValveTwoPulseTimer();
            void startValveSevenPulseTimer();
            void startExhuastVoidVacDownTimer();
            void startVacuumValveTimer();
    };
}}}



