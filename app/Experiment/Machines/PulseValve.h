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

// Include valiator
#include "States/CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines
{
    class PulseValve    :   public States::MachineStates
    {
        Q_OBJECT

        public:
            PulseValve(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~PulseValve();

            void setParams(int valve, int cycles, int timeOpen, int timeClosed);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_pulseValveFinished(QVariantMap params);
            void emit_pulseValveFailed(QVariantMap params);

            void emit_continueCycling();
            void emit_stopCycling();

        public slots:
            void cycle();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Valve to open and close
            QState* valvePointerOpen;
            QState* valvePointerClose;

            // Should be continue cycling
            QState sml_cycle;

            // Count number of pulses
            int cyclesCount;

            void setValvePointer(int valve);
            void closeValve(int valve);

    };
}}}



