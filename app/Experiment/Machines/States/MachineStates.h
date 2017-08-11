#pragma once

#include <memory.h>

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

// Include states
#include "Valves.h"
#include "Vacuum.h"
#include "Pressure.h"
#include "Flow.h"
#include "Timers.h"

namespace App { namespace Experiment { namespace Machines { namespace States
{
    class MachineStates : public QObject
    {
        Q_OBJECT
        public:
            MachineStates(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);
            ~MachineStates();

            // Holds the application settings
            Settings::Container m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the safety monitor
            Safety::Monitor &m_safety;

            // Hold the state machine
            QStateMachine machine;

            // Hold params for states
            QVariantMap params;

            // Hold instance of command constructor
            Hardware::CommandConstructor& m_commandConstructor;

            // External states
            Valves* m_valves;
            Vacuum* m_vacuum;
            Pressure* m_pressure;
            Flow* m_flow;
            Timers* m_timers;


            // State getters
            Valves* valves();
            Vacuum* vacuum();
            Pressure* pressure();
            Flow* flow();
            Timers* timers();

            // Create the states for the machine
            QState
                // Finishing sequence
                sm_finishVacSession
            ,   // Re-implimention of stop for each machine
                sm_stop
            ,   sm_stopAsFailed;



            // Create command validator states
            //CommandValidatorState


            // Helper methods
            void removeAllTransitions();

            // Contract methods that must be implimented
            virtual void start() = 0;
            virtual void buildMachine() = 0;

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

        public slots:
            // Re-implimention of stop for each machine
            virtual void stop() = 0;
            virtual void stopAsFailed() = 0;

        private:
            // Connect states to their function
            void connectStatesToMethods();

            // Turbo pump state
            bool turboState = false;

            // Vacuum pressure
            double vacuumPressure = 0;

            // Pressure
            double pressureSensor = 0;

    };
}}}}


