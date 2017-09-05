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

            void setParams(double pressure, int input, int frequency);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

            void connectStatesToMethods();


            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold the pressure for the previous stage
            double pressure = 1;

            // Timers for state machine
            QTimer t_pulseValveOne;
            QTimer t_pulseValveTwo;
            QTimer t_pulseValveSeven;

            QState  sml_startValveOneTimer
            ,       sml_startValveTwoTimer
            ,       sml_startValveSevenTimer

            ,       sml_waitForPressureAfterValveOne
            ,       sml_waitForPressureAfterValveTwo
            ,       sml_waitForPressureAfterValveSeven

            ,       sml_waitForValveOneTimer
            ,       sml_waitForValveTwoTimer
            ,       sml_waitForValveSevenTimer

            ,       sml_closeSlowExhuastPath_2
            ,       sml_closeOutput_2
            ,       sml_closeHighPressureInput_2;


            States::CommandValidatorState
                    sml_validatePressureAfterValveSeven
            ,       sml_validatePressureAfterValveTwo
            ,       sml_validatePressureAfterValveOne

            ,       sml_validateCloseHighPressureInput_2
            ,       sml_validateCloseSlowExhuastPath_2
            ,       sml_validateCloseOutput_2;

        signals:
            void emit_pressuriseFinished(QVariantMap params);
            void emit_pressuriseFailed(QVariantMap params);

            // State signals
            void emit_pressureToLow();
            void emit_pressureToHigh();
            void emit_pressureWithinTolerance();

            // Timer signals
            void emit_timerActive();

        public slots:
            // States
            void testMethod();

            // Validator states
            void validatePressureAfterValveOne();
            void validatePressureAfterValveTwo();
            void validatePressureAfterValveSeven();

            // Timers
            void startValveOnePulseTimer();
            void startValveTwoPulseTimer();
            void startValveSevenPulseTimer();

            void stopValveOnePulseTimer();
            void stopValveTwoPulseTimer();
            void stopValveSevenPulseTimer();

    };
}}}



