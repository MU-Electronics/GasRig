#pragma once

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

#include <QStateMachine>
#include <QState>

#include <QTimer>

namespace App { namespace Experiment
{
    class VacDown    :   public QObject
    {
        Q_OBJECT

        public:
            VacDown(QObject *parent, Settings::Container settings);

            void buildMachine(Hardware::Access &hardware, Safety::Monitor &safety);

        private:
            // Holds the application settings
            Settings::Container m_settings;

            // Hold the state machine
            QStateMachine machine;

            // Create the states for the machine
            QState
                // Check pressure
                sm_systemPressure,
                sm_validate_requestSystemPressure,

                // Close valves that could damage the system and validate response
                sm_closeHighPressureInput,
                sm_validate_closeHighPressureInput,
                sm_closeHighPressureNitrogen,
                sm_validate_closeHighPressureNitrogen,
                sm_closeFlowController,
                sm_validate_closeFlowController,
                sm_closeExhuast,
                sm_validate_closeExhuast,

                // Ensure turbo is disabled
                sm_disableTurbo,
                sm_validate_disableTurbo,

                // Start vacuum monitoring
                sm_startVacMonior,
                sm_validate_startVacMonior,

                // Start backing pump
                sm_startBackingPump,
                sm_validate_startBackingPump,

                // Start the turbo when vacuum low enough and has been selected
                sm_enableTurbo,
                sm_validate_enableTurbo,

                // When the timer has finished run the finished sequence
                sm_finished,

                // System error
                sm_error;

            // Hold a timer instance
            QTimer t_vacMonitor;
            QTimer t_vacTime;


    };
}}


