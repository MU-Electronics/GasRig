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
#include "Pressurise.h"

namespace App { namespace Experiment { namespace Machines
{
    class ContinuousPressure    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            ContinuousPressure(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~ContinuousPressure();

            void setParams(int maxTime, int monitorTime, double topUp, double leak, double pressure, int stepSize = 2000, bool inputValve = true, bool outputValve = true, bool exhuastValveOnly = false);

            void beforeStart();

            void stopped();

            void buildMachine();

            void buildShutDownMachine();

            Pressurise& m_pressurise;

        signals:
            void emit_maxTimeStillRunning();
            void emit_maxTimeElapsed();
            void emit_maxTimerStarted();

            void emit_leakDetected();
            void emit_topUpRequired();
            void emit_nothingRequired();

        public slots:
            void startMaxTime();
            void startMonitorTime();
            void startDelayTime();

            void checkPressure();

            void stopPressurise();
            void setHighPressure();

        private:
            // Referance to QObject
            //QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Timers
            QTimer t_maxTime;
            QTimer t_monitorTime;
            QTimer t_pressureDelayTime;

            // Past time and pressure
            long long  m_time = -1;
            double m_pastPressure = -1;

    };
}}}


