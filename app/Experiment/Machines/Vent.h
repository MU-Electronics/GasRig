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

            void buildShutDownMachine();

            bool fastExhuastOpen = false;

        signals:
            void emit_validationSuccess();
            void emit_validationFailed();

            void emit_openValveFour();

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
            void openPressureClose(QState *open,
                                    Functions::CommandValidatorState *openValidate,
                                    QState *close,
                                    Functions::CommandValidatorState *closeValidate,
                                    QState *openFastExhuast,
                                    QState *closeFastExhuast,
                                    QState *pressureWait,
                                    Functions::CommandValidatorState *pressureValidate,
                                    QState *finished,
                                    QState *failed);

    };
}}}


