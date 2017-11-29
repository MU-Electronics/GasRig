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

// Include valiator
#include "Functions/CommandValidatorState.h"

// Include possable machine states
#include "Functions/MachineStates.h"


namespace App { namespace Experiment { namespace Machines
{
    class Vent    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            Vent(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~Vent();

            void setParams(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);

            void start();

            void stopped();

            void buildMachine();

            // Create the states for the machine
            QState
                // Stage finder
                sml_stageFinder

                // Pressure waiting
            ,   sml_waitForPressureAfterSlowExhuast
            ,   sml_waitForPressureAfterOutput
            ,   sml_waitForPressureAfterVacOutput
            ,   sml_waitForPressureAfterFlowCavity
            ,   sml_waitForPressureAfterNitrogenPipe
            ,   sml_waitForPressureAfterMultiPipe
            ,   sml_waitForPressureAfterFlowOnePipe
            ,   sml_waitForPressureAfterFlowTwoPipe

                // Control flow controller override valves
            ,   sml_flowControllerOneValveOverrideOpen
            ,   sml_flowControllerTwoValveOverrideOpen
            ,   sml_flowControllerOneValveOverrideOff
            ,   sml_flowControllerTwoValveOverrideOff
            ,   sml_flowControllerOneValveOverrideClose
            ,   sml_flowControllerTwoValveOverrideClose

                // Close valves
            ,   sml_closeHighPressureInput
            ,   sml_closeHighPressureNitrogen
            ,   sml_closeFlowController
            ,   sml_closeFlowController_2
            ,   sml_closeFlowController_3
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
            ,   sml_openFlowController_2
            ,   sml_openFlowController_3
            ,   sml_openExhuast
            ,   sml_openOutput
            ,   sml_openSlowExhuastPath
            ,   sml_openFastExhuastPath
            ,   sml_openVacuumIn
            ,   sml_openVacuumOut;


            // Create command validator states
            Functions::CommandValidatorState
                // Wait for pressure
                sml_validatePressureAfterSlowExhuast
            ,   sml_validatePressureAfterOutput
            ,   sml_validatePressureAfterVacOutput
            ,   sml_validatePressureAfterFlowCavity
            ,   sml_validatePressureAfterNitrogenPipe
            ,   sml_validatePressureAfterMultiPipe
            ,   sml_validatePressureAfterFlowOnePipe
            ,   sml_validatePressureAfterFlowTwoPipe

                // Control flow controller override valves
            ,   sml_validateFlowControllerOneValveOverrideOpen
            ,   sml_validateFlowControllerTwoValveOverrideOpen
            ,   sml_validateFlowControllerOneValveOverrideClose
            ,   sml_validateFlowControllerTwoValveOverrideClose
            ,   sml_validateFlowControllerOneValveOverrideOff
            ,   sml_validateFlowControllerTwoValveOverrideOff

                // Validate close valves
            ,   sml_validateCloseHighPressureInput
            ,   sml_validateCloseHighPressureNitrogen
            ,   sml_validateCloseFlowController
            ,   sml_validateCloseFlowController_2
            ,   sml_validateCloseFlowController_3
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
            ,   sml_validateOpenFlowController_2
            ,   sml_validateOpenFlowController_3
            ,   sml_validateOpenExhuast
            ,   sml_validateOpenOutput
            ,   sml_validateOpenSlowExhuastPath
            ,   sml_validateOpenFastExhuastPath
            ,   sml_validateOpenVacuumIn
            ,   sml_validateOpenVacuumOut;

        signals:
            void emit_validationSuccess();
            void emit_validationFailed();

            void emit_openExhuast();
            void emit_openSlowExhuast();
            void emit_ventOuput();
            void emit_ventVacuumOutput();
            void emit_ventFlowCavity();
            void emit_ventNitrogenPipes();
            void emit_ventMultiPipes();
            void emit_ventFlowOnePipes();
            void emit_ventFlowTwoPipes();
            void emit_finished();

        public slots:
            void stageFinder();
            void validatePressureForAtmospheric();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Holds the current stage
            int stage = 0;

            // Timers
            // QTimer

            // Helpers
            void openPressureClose( QState& open,
                                    Functions::CommandValidatorState& openValidate,
                                    QState& close,
                                    Functions::CommandValidatorState& closeValidate,
                                    QState& pressureWait,
                                    Functions::CommandValidatorState& pressureValidate,
                                    QState& finished,
                                    QState& failed);

    };
}}}


