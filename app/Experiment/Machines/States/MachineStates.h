#pragma once

#include <memory.h>

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

// Include states
#include "Valves.h"

namespace App { namespace Experiment { namespace Machines { namespace States
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

            // Valves
            Valves* m_valves;

            // State getters
            Valves* valves();

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
            // Connect states to their function
            void connectStatesToMethods();

            // Turbo pump state
            bool turboState = false;

            // Vacuum pressure
            double vacuumPressure = 0;

            // Pressure
            double pressure = 0;

    };
}}}}


