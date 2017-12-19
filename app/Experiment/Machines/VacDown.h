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
    class VacDown    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            VacDown(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~VacDown();

            void setParams(double value, int valueType, bool turbo, int gasMode, int mode);

            void beforeStart();

            void stopped();

            void buildMachine();

            void buildShutDownMachine();

        signals:
            void emit_timerActive();
            void emit_pressureReached();
            void emit_pressureToLow();

        public slots:
            // States
            void startVacuumTimer();
            void validatePressureForStop();

        private:
            // Referance to QObject
            QObject *parent;


            // Holds the application settings
            Settings::Container m_settings;


            // Timers
            QTimer t_vacDown;
    };
}}}


