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

            // Hold a timer instance for sensors
            QTimer t_vacPressureMonitor;
            QTimer t_pressureMonitor;
            QTimer t_flowControllerFlowMonitor;
            QTimer t_flowControllerTemperatureMonitor;
            QTimer t_vacStationTemperatureMonitor;
            QTimer t_turboSpeedMonitor;

            // Hold a timer instance for events
            QTimer t_vacTime;   // Vac down timer

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
            ,   sm_getBearingTemperature
            ,   sm_getTC110ElectronicsTemperature
            ,   sm_getPumpBottomTemperature
            ,   sm_getMotorTemperature
                // States relating to controlling the flow controller
            ,   sm_flowControllerOneFlow
            ,   sm_flowControllerTwoFlow
                // Timers
            ,   sm_startVacuumPressureMonitor
            ,   sm_startPressureMonitor
            ,   sm_startFlowControllerFlowMonitor
            ,   sm_startVacuumTimer
            ,   sm_timerWait
            ,   sm_initalWait
            ,   sm_startFlowControllerTemperatureMonitor
            ,   sm_startVacStationTemperatureMonitor
            ,   sm_startTurboSpeedMonitor
                // Finishing sequence
            ,   sm_finishVacSession
            ,   // Re-implimention of stop for each machine
                sm_stop
            ,   sm_stopAsFailed;



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
            ,   sm_validateVacPressureForTurbo
            ,   sm_validateGetBearingTemperature
            ,   sm_validateGetTC110ElectronicsTemperature
            ,   sm_validateGetPumpBottomTemperature
            ,   sm_validateGetMotorTemperature
                // States relating to validating the flow controller
            ,   sm_validateFlowControllerOneFlow
            ,   sm_validateFlowControllerTwoFlow;



            // Helper methods
            void removeAllTransitions();

            // Contract methods that must be implimented
            virtual void start() = 0;
            virtual void buildMachine() = 0;

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);

            void emit_stateAlreadySet();

            void emit_turboPumpAlreadyEnabled();
            void emit_turboPumpAlreadyDisabled();

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

            // States relating to reading the vac station params
            void getTurboSpeed();
            void getBearingTemperature();
            void getTC110ElectronicsTemperature();
            void getPumpBottomTemperature();
            void getMotorTemperature();
            void validateGetBearingTemperature();
            void validateGetTC110ElectronicsTemperature();
            void validateGetPumpBottomTemperature();
            void validateGetMotorTemperature();

            // States relating to validating the vac station commands
            void validateDisableTurboPump();
            void validateEnableTurboPump();
            void validateDisableBackingPump();
            void validateEnableBackingPump();
            void validateSetGasModeHeavy();
            void validateSetGasModeMedium();
            void validateSetGasModeHelium();

            // States relating to controlling the flow controller
            void flowControllerOneFlow();
            void flowControllerTwoFlow();

            // States relating to validating the flow controller
            void validateFlowControllerOneFlow();
            void validateFlowControllerTwoFlow();


            // Finishing sequence
            void finishVacSession();



            // Re-implimention of stop for each machine
            virtual void stop() = 0;
            virtual void stopAsFailed() = 0;



            // Timer functions
            void timerWait();

            void startVacuumPressureMonitor();
            void stopVacuumPressureMonitor();

            void startPressureMonitor();
            void stopPressureMonitor();

            void startFlowControllerFlowMonitor();
            void stopFlowControllerFlowMonitor();

            void startFlowControllerTemperatureMonitor();
            void stopFlowControllerTemperatureMonitor();

            void startVacStationTemperatureMonitor();
            void stopVacStationTemperatureMonitor();

            void startTurboSpeedMonitor();
            void stopTurboSpeedMonitor();

            void startVacuumTimer();
            void stopVacuumTimer();

        private:
            // Holds the application settings
            Settings::Container m_settings;

            // Hold instance of command constructor
            Hardware::CommandConstructor m_commandConstructor;

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


