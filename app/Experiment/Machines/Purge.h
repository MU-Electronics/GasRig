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

            // Create the states for the machine
            // QState ;


            // Create command validator states
            // Functions::CommandValidatorState ;

        signals:
            void emit_purgeFinished(QVariantMap params);
            void emit_purgeFailed(QVariantMap params);

            void emit_validationSuccess();
            void emit_validationFailed();

        public slots:


        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Holds the current stage
            int cycle = 0;

            // Timers
            // QTimer

    };
}}}


