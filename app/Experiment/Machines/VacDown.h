#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "Functions/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class VacDown    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            VacDown(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~VacDown();

            void setParams(double value, int valueType, bool turbo, int gasMode, int mode);

            void start();

            void stopped();

            void buildMachine();

            void buildShutDownMachine();

        signals:
            void emit_timerActive();
            void emit_pressureReached();
            void emit_pressureToLow();

        public slots:
            // States
            void startVacuumTimer();
            void validatePressureForStop();

        private:
            // Referance to QObject
            QObject *parent;


            // Holds the application settings
            Settings::Container m_settings;


            // Timers
            QTimer t_vacDown;


            // States
            QState
                // Close valves
                sml_closeHighPressureInput
            ,   sml_closeHighPressureNitrogen
            ,   sml_closeFlowController
            ,   sml_closeExhuast
            ,   sml_closeOutput
            ,   sml_closeSlowExhuastPath
            ,   sml_closeFastExhuastPath
            ,   sml_closeVacuumIn
            ,   sml_closeVacuumOut

                // Open valve related states
            ,   sml_openHighPressureInput
            ,   sml_openHighPressureNitrogen
            ,   sml_openFlowController
            ,   sml_openExhuast
            ,   sml_openOutput
            ,   sml_openSlowExhuastPath
            ,   sml_openFastExhuastPath
            ,   sml_openVacuumIn
            ,   sml_openVacuumOut

                // States vacuum
            ,   sml_enableBackingPump
            ,   sml_enableTurboPump
            ,   sml_disableTurboPump
            ,   sml_waitForTurboSpeed

                // Timer states
            ,   sml_startVacuumTimer
            ,   sml_checkPressureForVacuum
            ,   sml_timerWait;


            // Validator states
            Functions::CommandValidatorState
                // States valves
                sml_validateCloseHighPressureInput
            ,   sml_validateCloseHighPressureNitrogen
            ,   sml_validateCloseFlowController
            ,   sml_validateCloseExhuast
            ,   sml_validateCloseOutput
            ,   sml_validateCloseSlowExhuastPath
            ,   sml_validateCloseFastExhuastPath
            ,   sml_validateCloseVacuumIn
            ,   sml_validateCloseVacuumOut

                // Validate open valve
            ,   sml_validateOpenHighPressureInput
            ,   sml_validateOpenHighPressureNitrogen
            ,   sml_validateOpenFlowController
            ,   sml_validateOpenExhuast
            ,   sml_validateOpenOutput
            ,   sml_validateOpenSlowExhuastPath
            ,   sml_validateOpenFastExhuastPath
            ,   sml_validateOpenVacuumIn
            ,   sml_validateOpenVacuumOut

                // States pressure
            ,   sml_validateVacPressureForTurbo
            ,   sml_validatePressureForVacuum
            ,   sml_validatePressureForStop

                // States vacuum
            ,   sml_validateTurboSpeedZero
            ,   sml_validateEnableBackingPump
            ,   sml_validateEnableTurboPump
            ,   sml_validateDisableTurboPump;
    };
}}}


