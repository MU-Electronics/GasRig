#pragma once

// Include external libs
#include <QDebug>
#include <QThread>
#include <QState>

// Include validator state
#include "CommandValidatorState.h"

// Include functions
#include "Valves.h"
#include "Vacuum.h"
#include "Pressure.h"
#include "Flow.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    class TransitionsBuilder
    {
        public:
            TransitionsBuilder(Settings::Container settings, Valves* valvesRef, Vacuum* vacuumRef, Pressure* pressureRef, Flow* flowRef);

            // Pressure related transistions
            void validatePressure(QState* waitForPressure,
                                  CommandValidatorState* validatePressureReading,
                                  QState* finished,
                                  QState* failed);

            // Valve related transistions
            void openValve(QState* open,
                           CommandValidatorState* openValidate,
                           QState* finished,
                           QState* failed);
            void closeValve(QState* close,
                           CommandValidatorState* closeValidate,
                           QState* finished,
                           QState* failed);
            void closeAllValves(QState* closeOutput,
                                CommandValidatorState* closeOutputValidate,
                                QState* closeSlowExhuast,
                                CommandValidatorState* closeSlowExhuastValidate,
                                QState* closeFastExhuast,
                                CommandValidatorState* closeFastExhuastValidate,
                                QState* closeVacuumIn,
                                CommandValidatorState* closeVacuumInValidate,
                                QState* closeVacuumOut,
                                CommandValidatorState* closeVacuumOutValidate,
                                QState* closeExhuast,
                                CommandValidatorState* closeExhuastValidate,
                                QState* closeHighPressureIn,
                                CommandValidatorState* closeHighPressureInValidate,
                                QState* closeNitrogenIn,
                                CommandValidatorState* closeNitrogenInValidate,
                                QState* closeFlowController,
                                CommandValidatorState* closeFlowControllerValidate,
                                QState* finished,
                                QState* failed);

        signals:


        public slots:


        private:
            // External states
            Valves* m_valves;
            Vacuum* m_vacuum;
            Pressure* m_pressure;
            Flow* m_flow;

    };

}}}}

