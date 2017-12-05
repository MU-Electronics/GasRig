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

// Include valiator
#include "Functions/CommandValidatorState.h"

// Include possable machine states
#include "Functions/MachineStates.h"

// Required state machines
#include "Pressurise.h"
#include "Vent.h"
#include "VacDown.h"

namespace App { namespace Experiment { namespace Machines
{
    class Purge    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            Purge(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~Purge();

            void setParams(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo);

            void start();

            void stopped();

            void buildMachine();

            // Override build sub state machine
            void buildShutDownMachine();

            // Pressurise state machine
            Pressurise& m_pressurise;

            // Pressurise state machine
            Vent& m_vent;

            // Pressurise state machine
            VacDown& m_vacDown;

            // Create the states for the machine
            QState
                sml_setLowPressure
            ,   sml_setHighPressure
            ,   sml_setAtmospheric
            ,   sml_checkCycles
            ,   sml_vent
            ,   sml_finishingVent
            ,   sml_vacDown

            ,   ssml_vent
            ,   ssml_pressurise
            ,   ssml_vacDown;


            // Create command validator states
            // Functions::CommandValidatorState ;

        signals:
            void emit_validationSuccess();
            void emit_validationFailed();

            void emit_continueCycling();
            void emit_stopCycling();

        public slots:
            void setHighPressure();
            void ventOutput();
            void setVacuum();
            void checkCycles();

            void stopVacuum();
            void stopVent();
            void stopPressurise();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Holds the current stage
            int cycles = 0;

            // Timers
            // QTimer

    };
}}}


