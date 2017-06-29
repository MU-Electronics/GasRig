#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../Hardware/CommandConstructor.h"

// Include command validator states
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines
{
    class MachineStates : public QObject
    {
        Q_OBJECT
        public:
            MachineStates(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the safety monitor
            Safety::Monitor &m_safety;

            // Hold the state machine
            QStateMachine machine;

            // Hold a timer instance
            QTimer t_vacMonitor;
            QTimer t_vacTime;

            // Create the states for the machine
            QState
                // Check pressure
                sm_systemPressure;

            CommandValidatorState
                // Validate pressure
                sm_validate_requestSystemPressure;

        signals:
            void hardwareRequest(QVariantMap command);

        public slots:
            // Functions for states
            void systemPressure();
            void validateSystemPressure();

        private:
            // Holds the application settings
            Settings::Container m_settings;

            // Hold instance of command constructor
            Hardware::CommandConstructor m_commandConstructor;





            // Setup state timers
            void setupTimers();

            // Connect states to their function
            void connectStatesToMethods();

    };
}}}


