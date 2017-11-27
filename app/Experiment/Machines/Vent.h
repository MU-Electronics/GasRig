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

namespace App { namespace Experiment { namespace Machines
{
    class Vent    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            Vent(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~Vent();

            void setParams(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_ventFinished(QVariantMap params);
            void emit_ventFailed(QVariantMap params);

        public slots:

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Timers
            // QTimer

            // States
            // QState

    };
}}}


