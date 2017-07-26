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
#include "MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class ReadTurboSpeed    :   public MachineStates
    {
        Q_OBJECT

        public:
            ReadTurboSpeed(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~ReadTurboSpeed();

            void setParams(int interval);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_readTurboSpeedStopped(QVariantMap params);
            void emit_readTurboSpeedFailed(QVariantMap params);

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

    };
}}}


