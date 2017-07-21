#include "MachineStates.h"

#include <cmath>

// Include extenral deps
#include <QObject>
#include <QDebug>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines
{

    MachineStates::MachineStates(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety)
        :   QObject(parent)
        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   machine(parent)
        ,   m_commandConstructor(*new Hardware::CommandConstructor)

            // Timers for states
        ,   t_vacPressureMonitor(parent)
        ,   t_pressureMonitor(parent)
        ,   t_flowControllerFlowMonitor(parent)
        ,   t_vacTime(parent)

            // Pressure sensor ralted states
        ,   sm_systemPressure(&machine)
        ,   sm_validatePressureForVacuum(&machine)
        ,   sm_vacPressure(&machine)
        ,   sm_validateVacPressureForTurbo(&machine)

            // Close valve related states
        ,   sm_closeHighPressureInput(&machine)       
        ,   sm_closeHighPressureNitrogen(&machine)      
        ,   sm_closeFlowController(&machine)        
        ,   sm_closeExhuast(&machine)
        ,   sm_closeOutput(&machine)
        ,   sm_closeSlowExhuastPath(&machine)
        ,   sm_closeFastExhuastPath(&machine)
        ,   sm_closeVacuumIn(&machine)
        ,   sm_closeVacuumOut(&machine)

            // Validate closed valve
        ,   sm_validateCloseHighPressureInput(&machine)
        ,   sm_validateCloseHighPressureNitrogen(&machine)
        ,   sm_validateCloseFlowController(&machine)
        ,   sm_validateCloseExhuast(&machine)
        ,   sm_validateCloseOutput(&machine)
        ,   sm_validateCloseSlowExhuastPath(&machine)
        ,   sm_validateCloseFastExhuastPath(&machine)
        ,   sm_validateCloseVacuumIn(&machine)
        ,   sm_validateCloseVacuumOut(&machine)

            // Open valve related states
        ,   sm_openHighPressureInput(&machine)
        ,   sm_openHighPressureNitrogen(&machine)
        ,   sm_openFlowController(&machine)
        ,   sm_openExhuast(&machine)
        ,   sm_openOutput(&machine)
        ,   sm_openSlowExhuastPath(&machine)
        ,   sm_openFastExhuastPath(&machine)
        ,   sm_openVacuumIn(&machine)
        ,   sm_openVacuumOut(&machine)

            // Validate open valve
        ,   sm_validateOpenHighPressureInput(&machine)
        ,   sm_validateOpenHighPressureNitrogen(&machine)
        ,   sm_validateOpenFlowController(&machine)
        ,   sm_validateOpenExhuast(&machine)
        ,   sm_validateOpenOutput(&machine)
        ,   sm_validateOpenSlowExhuastPath(&machine)
        ,   sm_validateOpenFastExhuastPath(&machine)
        ,   sm_validateOpenVacuumIn(&machine)
        ,   sm_validateOpenVacuumOut(&machine)

            // States relating to controlling the vac station
        ,   sm_disableTurboPump(&machine)
        ,   sm_enableTurboPump(&machine)
        ,   sm_disableBackingPump(&machine)
        ,   sm_enableBackingPump(&machine)
        ,   sm_setGasModeHeavy(&machine)
        ,   sm_setGasModeMedium(&machine)
        ,   sm_setGasModeHelium(&machine)


            // States relating to validating the vac station
        ,   sm_validateDisableTurboPump(&machine)
        ,   sm_validateEnableTurboPump(&machine)
        ,   sm_validateDisableBackingPump(&machine)
        ,   sm_validateEnableBackingPump(&machine)
        ,   sm_validateSetGasModeHeavy(&machine)
        ,   sm_validateSetGasModeMedium(&machine)
        ,   sm_validateSetGasModeHelium(&machine)
        ,   sm_validateStartVacuumPressureMonitor(&machine)

            // States relating to controlling the flow controller
        ,   sm_flowControllerOneFlow(&machine)
        ,   sm_flowControllerTwoFlow(&machine)

            // States relating to validating the vac station
        ,   sm_validateFlowControllerOneFlow(&machine)
        ,   sm_validateFlowControllerTwoFlow(&machine)

            // Timers
        ,   sm_timerWait(&machine)
        ,   sm_startVacuumPressureMonitor(&machine)
        ,   sm_startPressureMonitor(&machine)
        ,   sm_startFlowControllerFlowMonitor(&machine)
        ,   sm_startVacuumTimer(&machine)

            // Finishing sequence
        ,   sm_finishVacSession(&machine)

            // Re-implimention of stop for each machine
        ,   sm_stop(&machine)
        ,   sm_stopAsFailed(&machine)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &MachineStates::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }


    /**
     * Connect states to their matching methods
     *      NOTE: Methods are virtual so can be overloaded in machine classes
     *
     * @brief MachineStates::connectStatesToMethods
     */
    void MachineStates::connectStatesToMethods()
    {
        // Pressure related states
        connect(&sm_systemPressure, &QState::entered, this, &MachineStates::systemPressure);
        connect(&sm_validatePressureForVacuum, &CommandValidatorState::entered, this, &MachineStates::validatePressureForVacuum);
        connect(&sm_vacPressure, &QState::entered, this, &MachineStates::vacPressure);
        connect(&sm_validateVacPressureForTurbo, &CommandValidatorState::entered, this, &MachineStates::validateVacPressureForTurbo);


        // Link close valve states
        connect(&sm_closeHighPressureInput, &QState::entered, this, &MachineStates::closeHighPressureInput);
        connect(&sm_closeHighPressureNitrogen, &QState::entered, this, &MachineStates::closeHighPressureNitrogen);
        connect(&sm_closeFlowController, &QState::entered, this, &MachineStates::closeFlowController);
        connect(&sm_closeExhuast, &QState::entered, this, &MachineStates::closeExhuast);
        connect(&sm_closeOutput, &QState::entered, this, &MachineStates::closeOutput);
        connect(&sm_closeSlowExhuastPath, &QState::entered, this, &MachineStates::closeSlowExhuastPath);
        connect(&sm_closeFastExhuastPath, &QState::entered, this, &MachineStates::closeFastExhuastPath);
        connect(&sm_closeVacuumIn, &QState::entered, this, &MachineStates::closeVacuumIn);
        connect(&sm_closeVacuumOut, &QState::entered, this, &MachineStates::closeVacuumOut);

        // Link close valve validator states
        connect(&sm_validateCloseHighPressureInput, &CommandValidatorState::entered, this, &MachineStates::validateCloseHighPressureInput);
        connect(&sm_validateCloseHighPressureNitrogen, &CommandValidatorState::entered, this, &MachineStates::validateCloseHighPressureNitrogen);
        connect(&sm_validateCloseFlowController, &CommandValidatorState::entered, this, &MachineStates::validateCloseFlowController);
        connect(&sm_validateCloseExhuast, &CommandValidatorState::entered, this, &MachineStates::validateCloseExhuast);
        connect(&sm_validateCloseOutput, &CommandValidatorState::entered, this, &MachineStates::validateCloseOutput);
        connect(&sm_validateCloseSlowExhuastPath, &CommandValidatorState::entered, this, &MachineStates::validateCloseSlowExhuastPath);
        connect(&sm_validateCloseFastExhuastPath, &CommandValidatorState::entered, this, &MachineStates::validateCloseFastExhuastPath);
        connect(&sm_validateCloseVacuumIn, &CommandValidatorState::entered, this, &MachineStates::validateCloseVacuumIn);
        connect(&sm_validateCloseVacuumOut, &CommandValidatorState::entered, this, &MachineStates::validateCloseVacuumOut);

        // Link open valve states
        connect(&sm_openHighPressureInput, &QState::entered, this, &MachineStates::openHighPressureInput);
        connect(&sm_openHighPressureNitrogen, &QState::entered, this, &MachineStates::openHighPressureNitrogen);
        connect(&sm_openFlowController, &QState::entered, this, &MachineStates::openFlowController);
        connect(&sm_openExhuast, &QState::entered, this, &MachineStates::openExhuast);
        connect(&sm_openOutput, &QState::entered, this, &MachineStates::openOutput);
        connect(&sm_openSlowExhuastPath, &QState::entered, this, &MachineStates::openSlowExhuastPath);
        connect(&sm_openFastExhuastPath, &QState::entered, this, &MachineStates::openFastExhuastPath);
        connect(&sm_openVacuumIn, &QState::entered, this, &MachineStates::openVacuumIn);
        connect(&sm_openVacuumOut, &QState::entered, this, &MachineStates::openVacuumOut);

        // Link open valve validator states
        connect(&sm_validateOpenHighPressureInput, &CommandValidatorState::entered, this, &MachineStates::validateOpenHighPressureInput);
        connect(&sm_validateOpenHighPressureNitrogen, &CommandValidatorState::entered, this, &MachineStates::validateOpenHighPressureNitrogen);
        connect(&sm_validateOpenFlowController, &CommandValidatorState::entered, this, &MachineStates::validateOpenFlowController);
        connect(&sm_validateOpenExhuast, &CommandValidatorState::entered, this, &MachineStates::validateOpenExhuast);
        connect(&sm_validateOpenOutput, &CommandValidatorState::entered, this, &MachineStates::validateOpenOutput);
        connect(&sm_validateOpenSlowExhuastPath, &CommandValidatorState::entered, this, &MachineStates::validateOpenSlowExhuastPath);
        connect(&sm_validateOpenFastExhuastPath, &CommandValidatorState::entered, this, &MachineStates::validateOpenFastExhuastPath);
        connect(&sm_validateOpenVacuumIn, &CommandValidatorState::entered, this, &MachineStates::validateOpenVacuumIn);
        connect(&sm_validateOpenVacuumOut, &CommandValidatorState::entered, this, &MachineStates::validateOpenVacuumOut);

        // Link vac station states
        connect(&sm_disableTurboPump, &QState::entered, this, &MachineStates::disableTurboPump);
        connect(&sm_enableTurboPump, &QState::entered, this, &MachineStates::enableTurboPump);
        connect(&sm_disableBackingPump, &QState::entered, this, &MachineStates::disableBackingPump);
        connect(&sm_enableBackingPump, &QState::entered, this, &MachineStates::enableBackingPump);
        connect(&sm_setGasModeHeavy, &QState::entered, this, &MachineStates::setGasModeHeavy);
        connect(&sm_setGasModeMedium, &QState::entered, this, &MachineStates::setGasModeMedium);
        connect(&sm_setGasModeHelium, &QState::entered, this, &MachineStates::setGasModeHelium);

        // Link vac station validation states
        connect(&sm_validateDisableTurboPump, &CommandValidatorState::entered, this, &MachineStates::validateDisableTurboPump);
        connect(&sm_validateEnableTurboPump, &CommandValidatorState::entered, this, &MachineStates::validateEnableTurboPump);
        connect(&sm_validateDisableBackingPump, &CommandValidatorState::entered, this, &MachineStates::validateDisableBackingPump);
        connect(&sm_validateEnableBackingPump, &CommandValidatorState::entered, this, &MachineStates::validateEnableBackingPump);
        connect(&sm_validateSetGasModeHeavy, &CommandValidatorState::entered, this, &MachineStates::validateSetGasModeHeavy);
        connect(&sm_validateSetGasModeMedium, &CommandValidatorState::entered, this, &MachineStates::validateSetGasModeMedium);
        connect(&sm_validateSetGasModeHelium, &CommandValidatorState::entered, this, &MachineStates::validateSetGasModeHelium);

        // Link flow controller states
        connect(&sm_flowControllerOneFlow, &QState::entered, this, &MachineStates::flowControllerOneFlow);
        connect(&sm_flowControllerTwoFlow, &QState::entered, this, &MachineStates::flowControllerTwoFlow);

        // Link flow controller validation states
        connect(&sm_validateFlowControllerOneFlow, &CommandValidatorState::entered, this, &MachineStates::validateFlowControllerOneFlow);
        connect(&sm_validateFlowControllerTwoFlow, &CommandValidatorState::entered, this, &MachineStates::validateFlowControllerTwoFlow);

        // Link the timer states
        connect(&sm_timerWait, &QState::entered, this, &MachineStates::timerWait);
        connect(&sm_startVacuumPressureMonitor, &QState::entered, this, &MachineStates::startVacuumPressureMonitor);
        connect(&sm_startPressureMonitor, &QState::entered, this, &MachineStates::startPressureMonitor);
        connect(&sm_startFlowControllerFlowMonitor, &QState::entered, this, &MachineStates::startFlowControllerFlowMonitor);
        connect(&sm_startVacuumTimer, &QState::entered, this, &MachineStates::startVacuumTimer);


        // Finishing sequence
        connect(&sm_finishVacSession, &QState::entered, this, &MachineStates::finishVacSession);

        // Re-implimention of stop for each machine
        connect(&sm_stop, &QState::entered, this, &MachineStates::stop);
        connect(&sm_stopAsFailed, &QState::entered, this, &MachineStates::stopAsFailed);
    }



    /**
     * This helper method removes all transistions from all the states
     *
     * @brief MachineStates::removeAllTransitions
     */
    void MachineStates::removeAllTransitions()
    {
        // Get all states from machine and loop through them
        QList<QState *> allStates = machine.findChildren<QState *>();
        while(!allStates.isEmpty())
        {
            // Get the current state
            QState *state = allStates.takeFirst();

            // Get all the transistions for this state
            QList<QAbstractTransition *> transitions = state->transitions();

            // Remove all the transisition from the states
            while (!transitions.isEmpty())
                state->removeTransition(transitions.takeFirst());
        }
    }



    /**
     * Empty state to wait for timer events
     *
     * @brief MachineStates::timerWait
     */
    void MachineStates::timerWait()
    {
    }



    /**
     * The timer for how long to vac down for
     *
     * @brief MachineStates::startVacuumTimer
     */
    void MachineStates::startVacuumTimer()
    {
        if(!t_vacTime.isActive())
        {
            // Setup timer
            t_vacTime.setSingleShot(true);
            t_vacTime.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the vac down timer
     *
     * @brief MachineStates::stopVacuumTimer
     */
    void MachineStates::stopVacuumTimer()
    {
        t_vacTime.stop();
    }




    /**
     * Timer to use to trigger reading the vacuum sensor
     *
     * @brief MachineStates::startVacuumPressureMonitor
     */
    void MachineStates::startVacuumPressureMonitor()
    {
        if(!t_vacPressureMonitor.isActive())
        {
            // Setup timer
            t_vacPressureMonitor.setSingleShot(false);
            t_vacPressureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vacuum sensor
     *
     * @brief MachineStates::stopVacuumPressureMonitor
     */
    void MachineStates::stopVacuumPressureMonitor()
    {
        t_vacPressureMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the pressure sensor
     *
     * @brief MachineStates::startPressureMonitor
     */
    void MachineStates::startPressureMonitor()
    {
        if(!t_pressureMonitor.isActive())
        {
            // Setup timer
            t_pressureMonitor.setSingleShot(false);
            t_pressureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the pressure sensor
     *
     * @brief MachineStates::stopPressureMonitor
     */
    void MachineStates::stopPressureMonitor()
    {
        t_pressureMonitor.stop();
    }



    /**
     * Timer to use to trigger reading the flow controller flow sensor
     *
     * @brief MachineStates::startPressureMonitor
     */
    void MachineStates::startFlowControllerFlowMonitor()
    {
        if(!t_flowControllerFlowMonitor.isActive())
        {
            // Setup timer
            t_flowControllerFlowMonitor.setSingleShot(false);
            t_flowControllerFlowMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the flow controller flow sensor
     *
     * @brief MachineStates::stopPressureMonitor
     */
    void MachineStates::stopFlowControllerFlowMonitor()
    {
        t_flowControllerFlowMonitor.stop();
    }







    /**
     * Open valve helper
     *
     * @brief MachineStates::openValveHelper
     * @param number
     */
    void MachineStates::valveHelper(QString number, bool state)
    {
        // Find the correct valve name
        QString valveName = m_settings.hardware.valve_connections.value(number).toString();

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setValveState(valveName, state));
    }

    /**
     * Vavlidate valve helper
     *
     * @brief MachineStates::openValveHelper
     * @param number
     */
    void MachineStates::validateValveHelper(QString number, bool state)
    {
        // Get the validator state instance
        CommandValidatorState* command = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = command->package;

        // Find the correct valve name
        QString valveName = m_settings.hardware.valve_connections.value(number).toString();

        // Check valve is the same
        if(package.value("port").toString() == valveName && state == package.value("value").toBool())
        {
            // Data to pass on
            QVariantMap success;
            success.insert("requested_valve", valveName);
            success.insert("requested_valve_id", number);
            success.insert("valve_changed", package.value("port").toString());
            success.insert("requested_state", state);
            success.insert("state", package.value("value").toBool());

            // Emit safe to proceed
            emit emit_validationSuccess(success);

            return;
        }

        // Failed data to passon
        QVariantMap error;
        error.insert("message", "The valve failed to update correctly");
        error.insert("requested_valve", valveName);
        error.insert("requested_valve_id", number);
        error.insert("valve_changed", package.value("port").toString());
        error.insert("requested_state", state);
        error.insert("state", package.value("value").toBool());

        // Emit not safe to proceed
        emit emit_validationFailed(error);
    }


    /**
     * Request closing of the high pressure input valve
     *
     * @brief MachineStates::systemPressure
     */
    void MachineStates::closeHighPressureInput()
    {
        valveHelper("7", false);
    }

    void MachineStates::closeHighPressureNitrogen()
    {
        valveHelper("9", false);
    }

    void MachineStates::closeFlowController()
    {
        valveHelper("8", false);
    }

    void MachineStates::closeExhuast()
    {
        valveHelper("3", false);
    }

    void MachineStates::closeOutput()
    {
        valveHelper("1", false);
    }

    void MachineStates::closeSlowExhuastPath()
    {
        valveHelper("4", false);
    }

    void MachineStates::closeFastExhuastPath()
    {
        valveHelper("2", false);
    }

    void MachineStates::closeVacuumIn()
    {
        valveHelper("5", false);
    }

    void MachineStates::closeVacuumOut()
    {
        valveHelper("6", false);
    }


    /**
     * Request closing of the high pressure input valve
     *
     * @brief MachineStates::systemPressure
     */
    void MachineStates::validateCloseHighPressureInput()
    {
        validateValveHelper("7", false);
    }

    void MachineStates::validateCloseHighPressureNitrogen()
    {
        validateValveHelper("9", false);
    }

    void MachineStates::validateCloseFlowController()
    {
        validateValveHelper("8", false);
    }

    void MachineStates::validateCloseExhuast()
    {
        validateValveHelper("3", false);
    }

    void MachineStates::validateCloseOutput()
    {
        validateValveHelper("1", false);
    }

    void MachineStates::validateCloseSlowExhuastPath()
    {
        validateValveHelper("4", false);
    }

    void MachineStates::validateCloseFastExhuastPath()
    {
        validateValveHelper("2", false);
    }

    void MachineStates::validateCloseVacuumIn()
    {
        validateValveHelper("5", false);
    }

    void MachineStates::validateCloseVacuumOut()
    {
        validateValveHelper("6", false);
    }






    void MachineStates::openHighPressureInput()
    {
        valveHelper("7", true);
    }

    void MachineStates::openHighPressureNitrogen()
    {
        valveHelper("9", true);
    }

    void MachineStates::openFlowController()
    {
        valveHelper("8", true);
    }

    void MachineStates::openExhuast()
    {
        valveHelper("3", true);
    }

    void MachineStates::openOutput()
    {
        valveHelper("1", true);
    }

    void MachineStates::openSlowExhuastPath()
    {
        valveHelper("4", true);
    }

    void MachineStates::openFastExhuastPath()
    {
        valveHelper("2", true);
    }

    void MachineStates::openVacuumIn()
    {
        valveHelper("5", true);
    }

    void MachineStates::openVacuumOut()
    {
        valveHelper("6", true);
    }





    void MachineStates::validateOpenHighPressureInput()
    {
        validateValveHelper("7", true);
    }

    void MachineStates::validateOpenHighPressureNitrogen()
    {
        validateValveHelper("9", true);
    }

    void MachineStates::validateOpenFlowController()
    {
        validateValveHelper("8", true);
    }

    void MachineStates::validateOpenExhuast()
    {
        validateValveHelper("3", true);
    }

    void MachineStates::validateOpenOutput()
    {
        validateValveHelper("1", true);
    }

    void MachineStates::validateOpenSlowExhuastPath()
    {
        validateValveHelper("4", true);
    }

    void MachineStates::validateOpenFastExhuastPath()
    {
        validateValveHelper("2", true);
    }

    void MachineStates::validateOpenVacuumIn()
    {
        validateValveHelper("5", true);
    }

    void MachineStates::validateOpenVacuumOut()
    {
        validateValveHelper("6", true);
    }













    void MachineStates::disableTurboPump()
    {
        // Check current state
        if(!turboState)
        {
            emit emit_stateAlreadySet();
            return;
        }

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setTurboPump(false));
    }

    void MachineStates::enableTurboPump()
    {
        // Get the param trubo over ride if exists
        if(turboState || (!params.value("turbo").isNull() && !params.value("turbo").toBool()) )
        {
            emit emit_stateAlreadySet();
            return;
        }

        // Emit siganl to HAL
       emit hardwareRequest(m_commandConstructor.setTurboPump(true));
    }

    void MachineStates::disableBackingPump()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPump(false));
    }

    void MachineStates::enableBackingPump()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPump(true));
    }

    void MachineStates::setGasModeHeavy()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(0));
    }

    void MachineStates::setGasModeMedium()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(1));
    }

    void MachineStates::setGasModeHelium()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(2));
    }


    void MachineStates::validateDisableTurboPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        if(package.value("state").toBool() == false)
        {
            // Store turbo off
            turboState = false;

            // Store the success
            QVariantMap success;
            success.insert("message", "the turbo pump could not be disabled");
            success.insert("current_stated", package.value("state").toBool());
            success.insert("requested_state", false);

            // Emit safe to proceed
            emit emit_validationSuccess(success);

            return;
        }

        // Store the error
        QVariantMap error;
        error.insert("message", "the turbo pump could not be disabled");
        error.insert("current_stated", package.value("state").toBool());
        error.insert("requested_state", false);

        // Emit not safe to proceed
        emit emit_validationFailed(error);
    }

    void MachineStates::validateEnableTurboPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Store enable turbo
        turboState = true;

        qDebug() << "@todo Validating enabling turbo pump";

        QVariantMap success;
        success.insert("requested_state", true);
        emit emit_validationSuccess(success);

    }

    void MachineStates::validateDisableBackingPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        qDebug() << package;
    }

    void MachineStates::validateEnableBackingPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        //qDebug() << package;
    }

    void MachineStates::validateSetGasModeHeavy()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        qDebug() << package;
    }

    void MachineStates::validateSetGasModeMedium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        qDebug() << package;
    }

    void MachineStates::validateSetGasModeHelium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        qDebug() << package;
    }








    void MachineStates::flowControllerOneFlow()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerFlowRate("FlowControllerOne"));
    }


    void MachineStates::validateFlowControllerOneFlow()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void MachineStates::flowControllerTwoFlow()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerFlowRate("FlowControllerTwo"));
    }


    void MachineStates::validateFlowControllerTwoFlow()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }










    void MachineStates::vacPressure()
    {
        emit hardwareRequest(m_commandConstructor.getVacuumPressure( m_settings.hardware.vacuum_guage.value("connection").toString(),
                                                                     m_settings.hardware.vacuum_guage.value("slope").toDouble(),
                                                                     m_settings.hardware.vacuum_guage.value("offset").toDouble()));
    }


    void MachineStates::validateVacPressureForTurbo()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Get value form settings
        double setPressure = (m_settings.safety.turbo_pump.value("turbo_from").toDouble()) * 1000;

        // If port is the same as the vacuum guage port
        if(package.value("port").toString() == m_settings.hardware.vacuum_guage.value("connection").toString())
        {
            // Calculate current vacuum
            pressure = (std::pow(10, (1.667*package.value("calibrated").toDouble()-9.333)))/100;

            // Apply some hysteresis
            if( (turboState == false && pressure < setPressure) || (turboState == true && pressure < setPressure + 1) )
            {
                QVariantMap success;
                success.insert("pressure", pressure);
                emit emit_validationSuccess(success);
                return;
            }
        }

        // Store the error
        QVariantMap error;
        error.insert("message", "the vacuum pressure is too high for the turbo pump to be turned on");
        emit emit_validationFailed(error);

    }


    /**
     * Request a reading of the system pressure
     *
     * @brief MachineStates::systemPressure
     */
    void MachineStates::systemPressure()
    {
        emit hardwareRequest(m_commandConstructor.getPressureReading(1));
    }


    /**
     * Validate a reading of the system pressure
     *
     * @brief MachineStates::validateSystemPressure
     */
    void MachineStates::validatePressureForVacuum()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Get the pressure
        float pressure = package.value("pressure").toFloat();

        // Get the max pressure allowed
        float maxPressure = m_settings.safety.vacuum.value("vacuum_from").toFloat();

        // Check the pressure is safe to vac down
        if(pressure < maxPressure)
        {
            // Store the stage info
            QVariantMap data;
            data.insert("pressure", pressure);

            // Emit safe to proceed
            emit emit_validationSuccess(data);

            return;
        }

        // Store the error
        QVariantMap error;
        error.insert("message", "pressure in system is too high for the vac station; Exhuast the system first.");
        error.insert("system_pressure", pressure);
        error.insert("system_pressure_max", maxPressure);

        // Emit not safe to proceed
        emit emit_validationFailed(error);
    }



    void MachineStates::finishVacSession()
    {
        // Turn off vacuum
       disableTurboPump();
       disableBackingPump();

       // Close valves
       closeOutput();
       closeVacuumOut();
       closeFastExhuastPath();
       closeSlowExhuastPath();
       closeVacuumIn();

       // Stop timers
       stopVacuumPressureMonitor();
       stopVacuumTimer();
    }

}}}
