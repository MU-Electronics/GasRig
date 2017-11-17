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

            // How long to open pulse valve
            QTimer t_pulseValveOpen;

            // How long to close pulse valve
            QTimer t_pulseValveClose;

        signals:
            void emit_pulseValveFinished(QVariantMap params);
            void emit_pulseValveFailed(QVariantMap params);

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
            Settings::Container m_settings;

            // Valve to open and close
            QState* valvePointerOpen;
            QState* valvePointerClose;

            // Should be continue cycling
            QState sml_cycle
            ,   sml_startPulseValveOpenTimer
            ,   sml_startPulseValveCloseTimer

                // Valve states
            ,   sml_closeOutput_1
            ,   sml_openOutput_1
            ,   sml_closeSlowExhuastPath_1
            ,   sml_openSlowExhuastPath_1
            ,   sml_closeExhuast_1
            ,   sml_openExhuast_1
            ,   sml_closeFastExhuastPath_1
            ,   sml_openFastExhuastPath_1
            ,   sml_closeVacuumIn_1
            ,   sml_openVacuumIn_1
            ,   sml_closeVacuumOut_1
            ,   sml_openVacuumOut_1
            ,   sml_closeHighPressureInput_1
            ,   sml_openHighPressureInput_1
            ,   sml_closeFlowController_1
            ,   sml_openFlowController_1
            ,   sml_closeHighPressureNitrogen_1
            ,   sml_openHighPressureNitrogen_1;

            // Count number of pulses
            int cyclesCount;

            void setValvePointer(int valve);
            void closeValve(int valve);

    };
}}}



