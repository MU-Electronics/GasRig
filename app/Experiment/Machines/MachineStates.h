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

            // Hold params for states
            QVariantMap params;

            // Hold a timer instance
            QTimer t_vacPressureMonitor;
            QTimer t_vacTime;

            // Create the states for the machine
            QState
                // Check pressure
                sm_systemPressure
                // Close valves
            ,   sm_closeHighPressureInput
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
            ,   sm_openVacuumOut
                // States relating to controlling the vac station
            ,   sm_disableTurboPump
            ,   sm_enableTurboPump
            ,   sm_disableBackingPump
            ,   sm_enableBackingPump
            ,   sm_setGasModeHeavy
            ,   sm_setGasModeMedium
            ,   sm_setGasModeHelium
            ,   sm_vacPressure
                // Timers
            ,   sm_startVacuumPressureMonitor
            ,   sm_startVacuumTimer
            ,   sm_timerWait
                // Finishing sequence
            ,   sm_finishVacSession;




            // Create command validator states
            CommandValidatorState
                // Validate pressure
                sm_validatePressureForVacuum
                // Validate close valves
            ,   sm_validateCloseHighPressureInput
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
            ,   sm_validateOpenVacuumOut
                // States relating to controlling the vac station
            ,   sm_validateDisableTurboPump
            ,   sm_validateEnableTurboPump
            ,   sm_validateDisableBackingPump
            ,   sm_validateEnableBackingPump
            ,   sm_validateSetGasModeHeavy
            ,   sm_validateSetGasModeMedium
            ,   sm_validateSetGasModeHelium
            ,   sm_validateStartVacuumPressureMonitor
            ,   sm_validateVacPressureForTurbo;


        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

            void emit_stateAlreadySet();

            void emit_timerActive();

        public slots:
            // Pressure related states
            void systemPressure();
            void validatePressureForVacuum();
            void vacPressure();
            void validateVacPressureForTurbo();

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

            // States relating to controlling the vac station
            void disableTurboPump();
            void enableTurboPump();
            void disableBackingPump();
            void enableBackingPump();
            void setGasModeHeavy();
            void setGasModeMedium();
            void setGasModeHelium();

            // States relating to validating the vac station commands
            void validateDisableTurboPump();
            void validateEnableTurboPump();
            void validateDisableBackingPump();
            void validateEnableBackingPump();
            void validateSetGasModeHeavy();
            void validateSetGasModeMedium();
            void validateSetGasModeHelium();



            // Finishing sequence
            void finishVacSession();



            // Timer functions
            void timerWait();
            void startVacuumPressureMonitor();
            void stopVacuumPressureMonitor();
            void startVacuumTimer();
            void stopVacuumTimer();

        private:
            // Holds the application settings
            Settings::Container m_settings;

            // Hold instance of command constructor
            Hardware::CommandConstructor m_commandConstructor;

            // Setup state timers
            void setupTimers();

            // Connect states to their function
            void connectStatesToMethods();

            // Open valve helper
            void valveHelper(QString number, bool state);
            void validateValveHelper(QString number, bool state);

            // Turbo pump state
            bool turboState = false;

            // Vacuum pressure
            double vacuumPressure = 0;

            // Pressure
            double pressure = 0;

    };
}}}


