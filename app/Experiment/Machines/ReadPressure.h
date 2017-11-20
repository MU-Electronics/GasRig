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

namespace App { namespace Experiment { namespace Machines
{
    class ReadPressure    :   public States::MachineStates
    {
        Q_OBJECT

        public:
            ReadPressure(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~ReadPressure();

            void setParams(int pressureSensorTimeInter);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_readPressureStopped(QVariantMap params);
            void emit_readPressureFailed(QVariantMap params);

        public slots:
            void startPressureMonitor();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Timers
            QTimer t_pressureMonitor;

            // States
            QState
                sml_startPressureMonitor
            ,   sml_systemPressure;

    };
}}}


