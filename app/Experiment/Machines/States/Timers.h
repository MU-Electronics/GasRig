#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include command validator states
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace States
{
    class Timers : public QObject
    {
        Q_OBJECT

        public:
            Timers(QObject* parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~Timers();


            // Holds the application settings
            Settings::Container m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the safety monitor
            Safety::Monitor &m_safety;

            // Hold the state machine
            QStateMachine &m_machine;

            // Hold params for states
            QVariantMap &m_params;

            // Hold instance of command constructor
            Hardware::CommandConstructor& m_commandConstructor;

            // Hold a timer instance for sensors
            QTimer t_vacPressureMonitor;
            QTimer t_pressureMonitor;
            QTimer t_flowControllerFlowMonitor;
            QTimer t_flowControllerTemperatureMonitor;
            QTimer t_vacStationTemperatureMonitor;
            QTimer t_turboSpeedMonitor;

            // Hold a timer instance for events
            QTimer t_vacTime;   // Vac down timer

            // Create the states for the machine
            QState
                // Timers
                sm_startVacuumPressureMonitor
            ,   sm_startPressureMonitor
            ,   sm_startFlowControllerFlowMonitor
            ,   sm_startVacuumTimer
            ,   sm_timerWait
            ,   sm_initalWait
            ,   sm_startFlowControllerTemperatureMonitor
            ,   sm_startVacStationTemperatureMonitor
            ,   sm_startTurboSpeedMonitor;


            // Create command validator states
            // CommandValidatorState


            void connectStatesToMethods();

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

            void emit_stateAlreadySet();

            void emit_timerActive();

        public slots:
            // Timer functions
            void timerWait();

            void startVacuumPressureMonitor();
            void stopVacuumPressureMonitor();

            void startPressureMonitor();
            void stopPressureMonitor();

            void startFlowControllerFlowMonitor();
            void stopFlowControllerFlowMonitor();

            void startFlowControllerTemperatureMonitor();
            void stopFlowControllerTemperatureMonitor();

            void startVacStationTemperatureMonitor();
            void stopVacStationTemperatureMonitor();

            void startTurboSpeedMonitor();
            void stopTurboSpeedMonitor();

            void startVacuumTimer();
            void stopVacuumTimer();

        private:


    };

}}}}
