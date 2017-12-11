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

namespace App { namespace Experiment { namespace Machines { namespace Functions
{
    class Valves : public QObject
    {
        Q_OBJECT

        public:
            Valves(QObject* parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~Valves();


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

            void connectStatesToMethods();

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);
            void emit_validationWrongId(QVariantMap data);

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
