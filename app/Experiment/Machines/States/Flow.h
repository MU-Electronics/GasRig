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
    class Flow : public QObject
    {
        Q_OBJECT

        public:
            Flow(QObject* parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~Flow();


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

            // Create the states for the machine
            QState
                // States relating to controlling the flow controller
                sm_flowControllerOneFlow
            ,   sm_flowControllerTwoFlow;


            // Create command validator states
             CommandValidatorState
                // States relating to validating the flow controller
                sm_validateFlowControllerOneFlow
            ,   sm_validateFlowControllerTwoFlow;


            void connectStatesToMethods();

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

        public slots:
            // States relating to controlling the flow controller
            void flowControllerOneFlow();
            void flowControllerTwoFlow();

            // States relating to validating the flow controller
            void validateFlowControllerOneFlow();
            void validateFlowControllerTwoFlow();

        private:


    };

}}}}