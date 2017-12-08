#include "TransitionsBuilder.h"

// Include external libs
#include <QDebug>
#include <QState>

// Include settings container
#include "../../../Settings/Container.h"

// Include validator state type
#include "CommandValidatorState.h"

// Include functions
#include "Valves.h"
#include "Vacuum.h"
#include "Pressure.h"
#include "Flow.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    TransitionsBuilder::TransitionsBuilder(Settings::Container settings, Valves* valvesRef, Vacuum* vacuumRef, Pressure* pressureRef, Flow* flowRef)
            // Functions
        :   m_valves(valvesRef)
        ,   m_vacuum(vacuumRef)
        ,   m_pressure(pressureRef)
        ,   m_flow(flowRef)

    {

    }


    /**
     * Open a valve and validate the repsonce
     *
     * @brief TransitionsBuilder::openValve
     * @param open
     * @param openValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::openValve(QState* open,
                                       CommandValidatorState* openValidate,
                                       QState* finished,
                                       QState* failed)
    {

    }

    /**
     * Close a valve and validate the repsonce
     *
     * @brief TransitionsBuilder::openValve
     * @param close
     * @param closeValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::closeValve(QState* close,
                                       CommandValidatorState* closeValidate,
                                       QState* finished,
                                       QState* failed)
    {

    }



    /**
     * Close all valves in the system
     *
     * @brief closeAllValves
     * @param closeOutput
     * @param closeOutputValidate
     * @param closeSlowExhuast
     * @param closeSlowExhuastValidate
     * @param closeFastExhuast
     * @param closeFastExhuastValidate
     * @param closeVacuumIn
     * @param closeVacuumInValidate
     * @param closeVacuumOut
     * @param closeVacuumOutValidate
     * @param closeExhuast
     * @param closeExhuastValidate
     * @param closeHighPressureIn
     * @param closeHighPressureInValidate
     * @param closeNitrogenIn
     * @param closeNitrogenInValidate
     * @param closeFlowController
     * @param closeFlowControllerValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::closeAllValves(QState* closeOutput,
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
                        QState* failed)
    {

    }

}}}}
