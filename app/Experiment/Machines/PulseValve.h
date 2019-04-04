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

// Include valiator
#include "Functions/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines
{
    class PulseValve    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            PulseValve(QObject *parent, Settings::Container* settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~PulseValve();

            void setParams(int valve, int cycles, int timeOpen, int timeClosed);

            void beforeStart();

            void stopped();

            void buildMachine();

            void buildShutDownMachine();

            // How long to open pulse valve
            QTimer t_pulseValveOpen;

            // How long to close pulse valve
            QTimer t_pulseValveClose;

        signals:
            void emit_continueCycling();
            void emit_stopCycling();

        public slots:
            void cycle();

            void startPulseValveOpenTimer();
            void stopPulseValveOpenTimer();
            void startPulseValveCloseTimer();
            void stopPulseValveCloseTimer();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container* m_settings;

            // Valve to open and close
            QState* valvePointerOpen;
            QState* valvePointerClose;
            Functions::CommandValidatorState* valvePointerOpenValidator;
            Functions::CommandValidatorState* valvePointerCloseValidator;

            // Count number of pulses
            int cyclesCount;

            void setValvePointer(int valve);
    };
}}}



