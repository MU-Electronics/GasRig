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
#include "States/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class SafeValve    :   public States::MachineStates
    {
        Q_OBJECT

        public:
            SafeValve(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~SafeValve();

            void setParams(int id, bool state);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_safeValveFinished(QVariantMap params);
            void emit_safeValveFailed(QVariantMap params);

        private:
            // Referance to QObject
            QObject *parent;


            // Holds the application settings
            Settings::Container m_settings;


            // Valve state machines
            void valveOne(bool state);
            void valveTwo(bool state);
            void valveThree(bool state);
            void valveFour(bool state);
            void valveFive(bool state);
            void valveSix(bool state);
            void valveSeven(bool state);
            void valveEight(bool state);
            void valveNine(bool state);


            // Create the states for the machine
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
            ,   sml_openVacuumOut;


            // Create command validator states
            States::CommandValidatorState
                // Validate close valves
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
            ,   sml_validateOpenVacuumOut;

    };
}}}


