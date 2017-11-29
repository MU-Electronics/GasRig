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
    class ReadTurboSpeed    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            ReadTurboSpeed(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~ReadTurboSpeed();

            void setParams(int interval);

            void start();

            void stopped();

            void buildMachine();

        signals:

        public slots:
            void startTurboTimer();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Timers
            QTimer t_turboSpeed;

            // States
            QState
                sml_startTurboSpeedTimer
            ,   sml_readTurboSpeed;

    };
}}}


