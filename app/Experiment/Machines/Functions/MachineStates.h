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

// Include functions
#include "Valves.h"
#include "Vacuum.h"
#include "Pressure.h"
#include "Flow.h"

// Include transisiton builder
#include "TransitionsBuilder.h"

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

            // Are there any sub state machines that requirer stopping
            bool shutDownMachines = false;

            // Hold the sub state machine shut down
            QStateMachine shutDownMachine;


            // Holds the dynamically created states
            QMap<QString, QState*> m_states;
            QMap<QString, CommandValidatorState*> m_validators;

            // Creates (if not exists) and returns the states
            QState *state(QString id, bool type);
            CommandValidatorState *validator(QString id, bool type);


            // External functions
            Valves* m_valves;
            Vacuum* m_vacuum;
            Pressure* m_pressure;
            Flow* m_flow;


            // State getters
            Valves* valves();
            Vacuum* vacuum();
            Pressure* pressure();
            Flow* flow();


            // Transition builder for common tasks
            TransitionsBuilder* m_transitionsBuilder;

            // Transnistion builder getter
            TransitionsBuilder* transitionsBuilder();


            // Create the stop and stopped as failed states
            QState
                sm_stop
            ,   sm_stopAsFailed

            ,   ssm_stop
            ,   ssm_stopAsFailed;


            // Helper methods
            void removeAllTransitions(QStateMachine &stateMachine);
            void paramsOverride(QVariantMap override);


            // Contract methods that must be implimented
            virtual void start() = 0;
            virtual void buildMachine() = 0;

            // Not all machines will have a stop procedure
            virtual void buildShutDownMachine(){}


            // Testing
            QString childClassName = "";
            // Connection helpers
            // void connectValve(QString name, bool machine, const QMetaMethod &method);

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

            void emit_machineStopping(QVariantMap details);
            void emit_machineFinished(QVariantMap details);
            void emit_machineFailed(QVariantMap errorDetails);

            void emit_machineAlreadyStopped();

        public slots:
            // Finishing state machine slots
            void stopMachineWithoutError();
            void stopMachineWithError();

            // Re-implimention of stopped for each machine
            virtual void stopped() = 0;

            // Allow re-implentation of machine finished signal emitter
            virtual void emitStopped();

            // Allow re-implentation of sub machine finished signal emitter
            virtual void afterSubMachinesStopped();

            // Stops sub state machine shutdown machine
            void stopShutDownSubMachineWithoutError();
            void stopShutDownSubMachineWithError();

        private:
            // Connect states to their function
            void connectStatesToMethods();

            // Turbo pump state
            // bool turboState = false;

            // Vacuum pressure
            // double vacuumPressure = 0;

            // Pressure
            // double pressureSensor = 0;

    };
}}}}


