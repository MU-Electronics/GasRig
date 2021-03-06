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

    TransitionsBuilder::TransitionsBuilder(QObject *parent, Settings::Container *settings, Hardware::Access &hardware, Valves* valvesRef, Vacuum* vacuumRef, Pressure* pressureRef, Flow* flowRef)
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
     * Transitions to react to com bus errors
     *
     * @brief TransitionsBuilder::stateComErrors
     * @param state
     * @param failed
     */
    void TransitionsBuilder::stateComErrors(QState* state, QState* failed)
    {
        // Check for timeout errors on bus
        state->addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, failed);

        // Check for critical errors on bus
        state->addTransition(&m_hardware, &Hardware::Access::emit_critialSerialError, failed);
    }

    void TransitionsBuilder::stateComErrors(CommandValidatorState* state, QState* failed)
    {
        // Check for timeout errors on bus
        state->addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, failed);

        // Check for critical errors on bus
        state->addTransition(&m_hardware, &Hardware::Access::emit_critialSerialError, failed);
    }






    /**
     * Validate a pressure with success or fail signals
     *
     * @brief validatePressure
     * @param waitForPressure
     * @param validatePressureReading
     * @param finished
     * @param failed
     */
    void TransitionsBuilder::validatePressure(QState* waitForPressure,
                          CommandValidatorState* validatePressureReading,
                          QState* finished,
                          QState* failed)
    {
        // Check the system pressure
        waitForPressure->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validatePressureReading);
            // Pressure is low enough
            validatePressureReading->addTransition(m_pressure, &Functions::Pressure::emit_validationSuccess, finished);
            // Pressure is too high
            validatePressureReading->addTransition(m_pressure, &Functions::Pressure::emit_validationFailed, failed);

        // Account for bus errors
        stateComErrors(waitForPressure, failed);
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

        // Account for bus errors
        stateComErrors(enable, failed);
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

        // Account for bus errors
        stateComErrors(disable, failed);
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
                                               QState* readySet,
                                               QState* finished,
                                               QState* failed)
    {
        // Disable turbo pump
        disable->addTransition(m_vacuum, &Functions::Vacuum::emit_turboPumpAlreadyDisabled, readySet);
        disable->addTransition(&m_hardware, &Hardware::Access::emit_setTurboPumpState, disableValidate);
            // Turbo pump was disabled
            disableValidate->addTransition(m_vacuum, &Functions::Vacuum::emit_validationSuccess, finished);
            // Turbo pump could not be disabled
            disableValidate->addTransition(m_vacuum, &Functions::Vacuum::emit_validationFailed, failed);

        // Account for bus errors
        stateComErrors(disable, failed);
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
        open->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, openValidate);
            // Wrong signal was picked up
            openValidate->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, open);
            // Valve closed successfully
            openValidate->addTransition(m_valves, &Functions::Valves::emit_validationSuccess, finished);
            // Valve failed to close
            openValidate->addTransition(m_valves, &Functions::Valves::emit_validationFailed, failed);

        // Account for bus errors
        stateComErrors(open, failed);
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
        close->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, closeValidate);
            // Wrong signal was picked up
            closeValidate->addTransition(m_valves, &Functions::Valves::emit_validationWrongId, close);
            // Valve closed successfully
            closeValidate->addTransition(m_valves, &Functions::Valves::emit_validationSuccess, finished);
            // Valve failed to close
            closeValidate->addTransition(m_valves, &Functions::Valves::emit_validationFailed, failed);

        // Account for bus errors
        stateComErrors(close, failed);
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
