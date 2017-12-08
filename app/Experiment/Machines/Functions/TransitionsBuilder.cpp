#include "TransitionsBuilder.h"

// Include external libs
#include <QObject>
#include <QDebug>
#include <QState>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"

// Include validator state type
#include "CommandValidatorState.h"

// Include functions
#include "Valves.h"
#include "Vacuum.h"
#include "Pressure.h"
#include "Flow.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    TransitionsBuilder::TransitionsBuilder(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Valves* valvesRef, Vacuum* vacuumRef, Pressure* pressureRef, Flow* flowRef)
        :   QObject(parent)

            // Access the global referances
        ,   m_settings(settings)
        ,   m_hardware(hardware)

            // Functions
        ,   m_valves(valvesRef)
        ,   m_vacuum(vacuumRef)
        ,   m_pressure(pressureRef)
        ,   m_flow(flowRef)
    {

    }

    /**
     * Enabled the turbo pump and validates
     *
     * @brief TransitionsBuilder::enableBackingPump
     * @param enable
     * @param enableValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::enableBackingPump(QState* enable,
                                               CommandValidatorState* enableValidate,
                                               QState* finished,
                                               QState* failed)
    {
        // Enable backing pump
        enable->addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, enableValidate);
            // Backing pump failed
            enableValidate->addTransition(m_vacuum, &Functions::Vacuum::emit_validationFailed, failed);
            // Validate backing pump on
            enableValidate->addTransition(m_vacuum, &Functions::Vacuum::emit_validationSuccess, finished);
    }

    /**
     * Disable the turbo pump and validates
     *
     * @brief TransitionsBuilder::disableBackingPump
     * @param enable
     * @param enableValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::disableBackingPump(QState* disable,
                                               CommandValidatorState* disableValidate,
                                               QState* finished,
                                               QState* failed)
    {
        // Disable backing pump
        disable->addTransition(&m_hardware, &Hardware::Access::emit_setPumpingState, disableValidate);
            // Backing pump failed
            disableValidate->addTransition(m_vacuum, &Functions::Vacuum::emit_validationFailed, failed);
            // Validate backing pump on
            disableValidate->addTransition(m_vacuum, &Functions::Vacuum::emit_validationSuccess, finished);
    }

    /**
     * Disable the turbo pump and validates
     *
     * @brief TransitionsBuilder::disableTurboPump
     * @param disable
     * @param disableValidate
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::disableTurboPump(QState* disable,
                                               CommandValidatorState* disableValidate,
                                               QState* finished,
                                               QState* failed)
    {
        // Disable turbo pump
        disable->addTransition(m_vacuum, &Functions::Vacuum::emit_turboPumpAlreadyDisabled, disableValidate);
        disable->addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, disableValidate);
            // Turbo pump was disabled
            disableValidate->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationSuccess, finished);
            // Turbo pump could not be disabled
            disableValidate->addTransition(this->vacuum(), &Functions::Vacuum::emit_validationFailed, failed);
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
        // Open valve 2
        open->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, openValidate);
            // Valve closed successfully
            openValidate->addTransition(m_valves, &Functions::Valves::emit_validationSuccess, finished);
            // Valve failed to close
            openValidate->addTransition(m_valves, &Functions::Valves::emit_validationFailed, failed);
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
        // Open valve 2
        close->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, closeValidate);
            // Valve closed successfully
            closeValidate->addTransition(m_valves, &Functions::Valves::emit_validationSuccess, finished);
            // Valve failed to close
            closeValidate->addTransition(m_valves, &Functions::Valves::emit_validationFailed, failed);
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
        // Close valve 1
        closeValve(closeOutput, closeOutputValidate, closeSlowExhuast, failed);

        // Close valve 2
        closeValve(closeSlowExhuast, closeSlowExhuastValidate, closeFastExhuast, failed);

        // Close valve 4
        closeValve(closeFastExhuast, closeFastExhuastValidate, closeVacuumIn, failed);

        // Close valve 5
        closeValve(closeVacuumIn, closeVacuumInValidate, closeVacuumOut, failed);

        // Close valve 6
        closeValve(closeVacuumOut, closeVacuumOutValidate, closeExhuast, failed);

        // Close valve 3
        closeValve(closeExhuast, closeExhuastValidate, closeHighPressureIn, failed);

        // Close valve 7
        closeValve(closeHighPressureIn, closeHighPressureInValidate, closeNitrogenIn, failed);

        // Close valve 9
        closeValve(closeNitrogenIn, closeNitrogenInValidate, closeFlowController, failed);

        // Close valve 8
        closeValve(closeFlowController, closeFlowControllerValidate, finished, failed);

    }

}}}}
