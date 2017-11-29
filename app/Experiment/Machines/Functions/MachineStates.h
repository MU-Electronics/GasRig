#pragma once

#include <memory.h>

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>
#include <QVariantMap>

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

namespace App { namespace Experiment { namespace Machines { namespace Functions
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

            // Did an error occure which caused the machine to stop
            bool error = false;

            // Error cache for details on any error
            QVariantMap errorDetails;

            // External states
            Valves* m_valves;
            Vacuum* m_vacuum;
            Pressure* m_pressure;
            Flow* m_flow;


            // State getters
            Valves* valves();
            Vacuum* vacuum();
            Pressure* pressure();
            Flow* flow();

            // Create the states for the machine
            QState
               // Re-implimention of stop for each machine
                sm_stop
            ,   sm_stopAsFailed;

            // Helper methods
            void stopMachineWithoutError();
            void stopMachineWithError();
            void removeAllTransitions();
            void paramsOverride(QVariantMap override);

            // Contract methods that must be implimented
            virtual void start() = 0;
            virtual void buildMachine() = 0;

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

            void emit_machineFinished(QVariantMap details);
            void emit_machineFailed(QVariantMap errorDetails);

        public slots:
            // Re-implimention of stopped for each machine
            virtual void stopped() = 0;

            // Allow re-implentation of machine finished signal emitter
            virtual void emitStopped();

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


