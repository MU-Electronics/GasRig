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

            void stop();

            void stopAsFailed();

            void buildMachine();

            // Pressurise state machine
            Pressurise& m_pressurise;

            // Create the states for the machine
            QState
                sml_setLowPressure
            ,   sml_setHighPressure
            ,   sml_setAtmospheric
            ,   sml_checkCycles;


            // Create command validator states
            // Functions::CommandValidatorState ;

        signals:
            void emit_purgeFinished(QVariantMap params);
            void emit_purgeFailed(QVariantMap params);

            void emit_validationSuccess();
            void emit_validationFailed();

            void emit_continueCycling();
            void emit_stopCycling();

        public slots:
            void setHighPressure();
            void setLowPressure();
            void setAtmospheric();
            void checkCycles();

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


