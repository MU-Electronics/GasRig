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
    class ReadVacuum    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            ReadVacuum(QObject *parent, Settings::Container *settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~ReadVacuum();

            void setParams(int vacSensorTimeInter);

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:

        public slots:
            void startVacuumPressureMonitor();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer t_vacPressureMonitor;

    };
}}}


