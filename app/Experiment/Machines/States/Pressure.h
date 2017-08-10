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
    class Pressure : public QObject
    {
        Q_OBJECT

        public:
            Pressure(QObject* parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~Pressure();


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
                sm_vacPressure
            ,   sm_systemPressure;


            // Create command validator states
            CommandValidatorState
                // Validate pressure
                sm_validateVacPressureForTurbo
            ,   sm_validatePressureForVacuum;

            // Connect state to methods
            void connectStatesToMethods();

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

        public slots:
            // Monitor the turbo state
            void monitorTurboState(QVariantMap package);

            // Pressure
            void systemPressure();
            void vacPressure();

            // Conditiona validation
            void validatePressureForVacuum();
            void validateVacPressureForTurbo();


        private:

            // Turbo pump state
            bool m_turboState = false;

            // Vacuum pressure
            double m_vacuumPressure = 0;

            // Pressure
            double m_pressureSensor = 0;


    };

}}}}
