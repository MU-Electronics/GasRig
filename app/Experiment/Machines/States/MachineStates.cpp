#include "MachineStates.h"

#include <cmath>

// Include extenral deps
#include <QObject>
#include <QDebug>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace States
{

    MachineStates::MachineStates(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety)
        :   QObject(parent)
        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   machine(parent)
        ,   m_commandConstructor(*new Hardware::CommandConstructor)

            // Valve states
        ,   m_valves(*new Valves(parent, settings, hardware, safety, machine, params, m_commandConstructor))

            // Timers for states
        ,   t_vacPressureMonitor(parent)
        ,   t_pressureMonitor(parent)
        ,   t_flowControllerFlowMonitor(parent)
        ,   t_vacTime(parent)
        ,   t_flowControllerTemperatureMonitor(parent)
        ,   t_vacStationTemperatureMonitor(parent)
        ,   t_turboSpeedMonitor(parent)

            // Pressure sensor related states
        ,   sm_systemPressure(&machine)
        ,   sm_validatePressureForVacuum(&machine)
        ,   sm_vacPressure(&machine)
        ,   sm_validateVacPressureForTurbo(&machine)

            // States relating to controlling the vac station
        ,   sm_disableTurboPump(&machine)
        ,   sm_enableTurboPump(&machine)
        ,   sm_disableBackingPump(&machine)
        ,   sm_enableBackingPump(&machine)
        ,   sm_setGasModeHeavy(&machine)
        ,   sm_setGasModeMedium(&machine)
        ,   sm_setGasModeHelium(&machine)
        ,   sm_getBearingTemperature(&machine)
        ,   sm_getTC110ElectronicsTemperature(&machine)
        ,   sm_getPumpBottomTemperature(&machine)
        ,   sm_getMotorTemperature(&machine)

            // States relating to validating the vac station
        ,   sm_validateDisableTurboPump(&machine)
        ,   sm_validateEnableTurboPump(&machine)
        ,   sm_validateDisableBackingPump(&machine)
        ,   sm_validateEnableBackingPump(&machine)
        ,   sm_validateSetGasModeHeavy(&machine)
        ,   sm_validateSetGasModeMedium(&machine)
        ,   sm_validateSetGasModeHelium(&machine)
        ,   sm_validateStartVacuumPressureMonitor(&machine)
        ,   sm_validateGetBearingTemperature(&machine)
        ,   sm_validateGetTC110ElectronicsTemperature(&machine)
        ,   sm_validateGetPumpBottomTemperature(&machine)
        ,   sm_validateGetMotorTemperature(&machine)

            // States relating to controlling the flow controller
        ,   sm_flowControllerOneFlow(&machine)
        ,   sm_flowControllerTwoFlow(&machine)

            // States relating to validating the vac station
        ,   sm_validateFlowControllerOneFlow(&machine)
        ,   sm_validateFlowControllerTwoFlow(&machine)

            // Timers
        ,   sm_initalWait(&machine)
        ,   sm_timerWait(&machine)
        ,   sm_startVacuumPressureMonitor(&machine)
        ,   sm_startPressureMonitor(&machine)
        ,   sm_startFlowControllerFlowMonitor(&machine)
        ,   sm_startFlowControllerTemperatureMonitor(&machine)
        ,   sm_startVacStationTemperatureMonitor(&machine)
        ,   sm_startTurboSpeedMonitor(&machine)
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

        // Link vac station states
        connect(&sm_disableTurboPump, &QState::entered, this, &MachineStates::disableTurboPump);
        connect(&sm_enableTurboPump, &QState::entered, this, &MachineStates::enableTurboPump);
        connect(&sm_disableBackingPump, &QState::entered, this, &MachineStates::disableBackingPump);
        connect(&sm_enableBackingPump, &QState::entered, this, &MachineStates::enableBackingPump);
        connect(&sm_setGasModeHeavy, &QState::entered, this, &MachineStates::setGasModeHeavy);
        connect(&sm_setGasModeMedium, &QState::entered, this, &MachineStates::setGasModeMedium);
        connect(&sm_setGasModeHelium, &QState::entered, this, &MachineStates::setGasModeHelium);

        connect(&sm_getBearingTemperature, &QState::entered, this, &MachineStates::getBearingTemperature);
        connect(&sm_getTC110ElectronicsTemperature, &QState::entered, this, &MachineStates::getTC110ElectronicsTemperature);
        connect(&sm_getPumpBottomTemperature, &QState::entered, this, &MachineStates::getPumpBottomTemperature);
        connect(&sm_getMotorTemperature, &QState::entered, this, &MachineStates::getMotorTemperature);

        // Link vac station validation states
        connect(&sm_validateDisableTurboPump, &CommandValidatorState::entered, this, &MachineStates::validateDisableTurboPump);
        connect(&sm_validateEnableTurboPump, &CommandValidatorState::entered, this, &MachineStates::validateEnableTurboPump);
        connect(&sm_validateDisableBackingPump, &CommandValidatorState::entered, this, &MachineStates::validateDisableBackingPump);
        connect(&sm_validateEnableBackingPump, &CommandValidatorState::entered, this, &MachineStates::validateEnableBackingPump);
        connect(&sm_validateSetGasModeHeavy, &CommandValidatorState::entered, this, &MachineStates::validateSetGasModeHeavy);
        connect(&sm_validateSetGasModeMedium, &CommandValidatorState::entered, this, &MachineStates::validateSetGasModeMedium);
        connect(&sm_validateSetGasModeHelium, &CommandValidatorState::entered, this, &MachineStates::validateSetGasModeHelium);
        connect(&sm_validateGetBearingTemperature, &CommandValidatorState::entered, this, &MachineStates::validateGetBearingTemperature);
        connect(&sm_validateGetTC110ElectronicsTemperature, &CommandValidatorState::entered, this, &MachineStates::validateGetTC110ElectronicsTemperature);
        connect(&sm_validateGetPumpBottomTemperature, &CommandValidatorState::entered, this, &MachineStates::validateGetPumpBottomTemperature);
        connect(&sm_validateGetMotorTemperature, &CommandValidatorState::entered, this, &MachineStates::validateGetMotorTemperature);

        // Link flow controller states
        connect(&sm_flowControllerOneFlow, &QState::entered, this, &MachineStates::flowControllerOneFlow);
        connect(&sm_flowControllerTwoFlow, &QState::entered, this, &MachineStates::flowControllerTwoFlow);

        // Link flow controller validation states
        connect(&sm_validateFlowControllerOneFlow, &CommandValidatorState::entered, this, &MachineStates::validateFlowControllerOneFlow);
        connect(&sm_validateFlowControllerTwoFlow, &CommandValidatorState::entered, this, &MachineStates::validateFlowControllerTwoFlow);

        // Link the timer states
        connect(&sm_initalWait, &QState::entered, this, &MachineStates::timerWait);
        connect(&sm_timerWait, &QState::entered, this, &MachineStates::timerWait);
        connect(&sm_startVacuumPressureMonitor, &QState::entered, this, &MachineStates::startVacuumPressureMonitor);
        connect(&sm_startPressureMonitor, &QState::entered, this, &MachineStates::startPressureMonitor);
        connect(&sm_startFlowControllerFlowMonitor, &QState::entered, this, &MachineStates::startFlowControllerFlowMonitor);
        connect(&sm_startVacuumTimer, &QState::entered, this, &MachineStates::startVacuumTimer);

        connect(&sm_startFlowControllerTemperatureMonitor, &QState::entered, this, &MachineStates::startFlowControllerTemperatureMonitor);
        connect(&sm_startVacStationTemperatureMonitor, &QState::entered, this, &MachineStates::startVacStationTemperatureMonitor);
        connect(&sm_startTurboSpeedMonitor, &QState::entered, this, &MachineStates::startTurboSpeedMonitor);

        // Finishing sequence
        connect(&sm_finishVacSession, &QState::entered, this, &MachineStates::finishVacSession);

        // Re-implimention of stop for each machine
        connect(&sm_stop, &QState::entered, this, &MachineStates::stop);
        connect(&sm_stopAsFailed, &QState::entered, this, &MachineStates::stopAsFailed);
    }



    /**
     * Returns the valve states container
     *
     * @brief MachineStates::valves
     * @return
     */
    Valves& MachineStates::valves()
    {
        return m_valves;
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
     * Timer to use to trigger reading the vac station turbo speed sensor
     *
     * @brief MachineStates::startTurboSpeedMonitor
     */
    void MachineStates::startTurboSpeedMonitor()
    {
        if(!t_turboSpeedMonitor.isActive())
        {
            // Setup timer
            t_turboSpeedMonitor.setSingleShot(false);
            t_turboSpeedMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vac station turbo speed sensor
     *
     * @brief MachineStates::stopTurboSpeedMonitor
     */
    void MachineStates::stopTurboSpeedMonitor()
    {
        t_turboSpeedMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the vac station temperature sensor
     *
     * @brief MachineStates::startVacStationTemperatureMonitor
     */
    void MachineStates::startVacStationTemperatureMonitor()
    {
        if(!t_vacStationTemperatureMonitor.isActive())
        {
            // Setup timer
            t_vacStationTemperatureMonitor.setSingleShot(false);
            t_vacStationTemperatureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vac station temperature sensor
     *
     * @brief MachineStates::stopVacStationTemperatureMonitor
     */
    void MachineStates::stopVacStationTemperatureMonitor()
    {
        t_vacStationTemperatureMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the flow controller temperature sensor
     *
     * @brief MachineStates::startFlowControllerTemperatureMonitor
     */
    void MachineStates::startFlowControllerTemperatureMonitor()
    {
        if(!t_flowControllerTemperatureMonitor.isActive())
        {
            // Setup timer
            t_flowControllerTemperatureMonitor.setSingleShot(false);
            t_flowControllerTemperatureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the flow controller temperature sensor
     *
     * @brief MachineStates::stopFlowControllerTemperatureMonitor
     */
    void MachineStates::stopFlowControllerTemperatureMonitor()
    {
        t_flowControllerTemperatureMonitor.stop();
    }























    void MachineStates::disableTurboPump()
    {
        // Check current state
        if(!turboState)
        {
            emit emit_turboPumpAlreadyDisabled();
            return;
        }

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setTurboPump(false));
    }

    void MachineStates::enableTurboPump()
    {
        // Get the param trubo over ride if exists
        if(turboState || params.value("turbo").isNull() || !params.value("turbo").toBool() )
        {
            emit emit_turboPumpAlreadyEnabled();
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

    void MachineStates::getTurboSpeed()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboSpeed());
    }

    void MachineStates::getBearingTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboBearingTemperature());
    }

    void MachineStates::getTC110ElectronicsTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTC110ElectronicsTemperature());
    }

    void MachineStates::getPumpBottomTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getPumpBottomTemperature());
    }

    void MachineStates::getMotorTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getMotorTemperature());
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

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void MachineStates::validateSetGasModeMedium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void MachineStates::validateSetGasModeHelium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void MachineStates::validateGetBearingTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void MachineStates::validateGetTC110ElectronicsTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void MachineStates::validateGetPumpBottomTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void MachineStates::validateGetMotorTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

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
       m_valves.closeOutput();
       m_valves.closeVacuumOut();
       m_valves.closeFastExhuastPath();
       m_valves.closeSlowExhuastPath();
       m_valves.closeVacuumIn();

       // Stop timers
       stopVacuumPressureMonitor();
       stopVacuumTimer();
    }

}}}}
