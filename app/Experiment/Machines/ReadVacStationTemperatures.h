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
    class ReadVacStationTemperatures    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            ReadVacStationTemperatures(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~ReadVacStationTemperatures();

            void setParams(int interval);

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:

        public slots:
            void startTemperatureTimer();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer t_temperatureMonitor;

    };
}}}


