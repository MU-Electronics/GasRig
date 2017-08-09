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
    class Valves : public QObject
    {
        Q_OBJECT

        public:
            Valves(QObject* parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the safety monitor
            Safety::Monitor &m_safety;

            // Hold the state machine
            QStateMachine &m_machine;

            // Hold params for states
            QVariantMap &m_params;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold instance of command constructor
            Hardware::CommandConstructor& m_commandConstructor;

            // Create the states for the machine
            QState
                // Close valves
                sm_closeHighPressureInput
            ,   sm_closeHighPressureNitrogen
            ,   sm_closeFlowController
            ,   sm_closeExhuast
            ,   sm_closeOutput
            ,   sm_closeSlowExhuastPath
            ,   sm_closeFastExhuastPath
            ,   sm_closeVacuumIn
            ,   sm_closeVacuumOut
                // Open valve related states
            ,   sm_openHighPressureInput
            ,   sm_openHighPressureNitrogen
            ,   sm_openFlowController
            ,   sm_openExhuast
            ,   sm_openOutput
            ,   sm_openSlowExhuastPath
            ,   sm_openFastExhuastPath
            ,   sm_openVacuumIn
            ,   sm_openVacuumOut;

            // Create command validator states
            CommandValidatorState
                // Validate close valves
                sm_validateCloseHighPressureInput
            ,   sm_validateCloseHighPressureNitrogen
            ,   sm_validateCloseFlowController
            ,   sm_validateCloseExhuast
            ,   sm_validateCloseOutput
            ,   sm_validateCloseSlowExhuastPath
            ,   sm_validateCloseFastExhuastPath
            ,   sm_validateCloseVacuumIn
            ,   sm_validateCloseVacuumOut
                // Validate open valve
            ,   sm_validateOpenHighPressureInput
            ,   sm_validateOpenHighPressureNitrogen
            ,   sm_validateOpenFlowController
            ,   sm_validateOpenExhuast
            ,   sm_validateOpenOutput
            ,   sm_validateOpenSlowExhuastPath
            ,   sm_validateOpenFastExhuastPath
            ,   sm_validateOpenVacuumIn
            ,   sm_validateOpenVacuumOut;


            void connectStatesToMethods();

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

        public slots:
            // Close valve states
            void closeHighPressureInput();
            void closeHighPressureNitrogen();
            void closeFlowController();
            void closeExhuast();
            void closeOutput();
            void closeSlowExhuastPath();
            void closeFastExhuastPath();
            void closeVacuumIn();
            void closeVacuumOut();

            // Validate close valve states
            void validateCloseHighPressureInput();
            void validateCloseHighPressureNitrogen();
            void validateCloseFlowController();
            void validateCloseExhuast();
            void validateCloseOutput();
            void validateCloseSlowExhuastPath();
            void validateCloseFastExhuastPath();
            void validateCloseVacuumIn();
            void validateCloseVacuumOut();

            // Open valve states
            void openHighPressureInput();
            void openHighPressureNitrogen();
            void openFlowController();
            void openExhuast();
            void openOutput();
            void openSlowExhuastPath();
            void openFastExhuastPath();
            void openVacuumIn();
            void openVacuumOut();

            // Validate open valve states
            void validateOpenHighPressureInput();
            void validateOpenHighPressureNitrogen();
            void validateOpenFlowController();
            void validateOpenExhuast();
            void validateOpenOutput();
            void validateOpenSlowExhuastPath();
            void validateOpenFastExhuastPath();
            void validateOpenVacuumIn();
            void validateOpenVacuumOut();

            private:
                // Open valve helper
                void valveHelper(QString number, bool state);
                void validateValveHelper(QString number, bool state);

    };

}}}}
