#include "Vacuum.h"

#include <QString>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    Vacuum::Vacuum(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Vacuum::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    Vacuum::~Vacuum()
    {

    }

    void Vacuum::connectStatesToMethods()
    {

    }




    void Vacuum::disableTurboPump()
    {
        // Check current state
        if(!m_turboState)
        {
            emit emit_turboPumpAlreadyDisabled();
            return;
        }

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setTurboPump(false));
    }

    void Vacuum::enableTurboPump()
    {
        // Get the param trubo over ride if exists
        if(m_turboState || m_params.value("turbo").isNull() || !m_params.value("turbo").toBool() )
        {
            emit emit_turboPumpAlreadyEnabled();
            return;
        }

        // Emit siganl to HAL
       emit hardwareRequest(m_commandConstructor.setTurboPump(true));
    }

    void Vacuum::disableBackingPump()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPump(false));
    }

    void Vacuum::enableBackingPump()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPump(true));
    }

    void Vacuum::setGasModeHeavy()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(0));
    }

    void Vacuum::setGasModeMedium()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(1));
    }

    void Vacuum::setGasModeHelium()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(2));
    }

    void Vacuum::getTurboSpeed()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboSpeed());
    }

    void Vacuum::getBearingTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboBearingTemperature());
    }

    void Vacuum::getTC110ElectronicsTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTC110ElectronicsTemperature());
    }

    void Vacuum::getPumpBottomTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getPumpBottomTemperature());
    }

    void Vacuum::getMotorTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getMotorTemperature());
    }


    void Vacuum::validateTurboSpeedZero()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Turbo speed
        double turboSpeed = package.value("speed").toDouble();

        // Is the turbo still spinning?
        if(turboSpeed < 1)
        {
            // Store the success
            QVariantMap success;
            success.insert("message", "the turbo pump is not spinning");
            success.insert("current_stated", turboSpeed);

            // Emit safe to proceed
            emit emit_validationSuccess(success);

            return;
        }


        // Store the error
        QVariantMap error;
        error.insert("message", "the turbo pump is still spinner");
        error.insert("current_speed", turboSpeed);

        // Emit not safe to proceed
        emit emit_validationFailed(error);
    }


    void Vacuum::validateDisableTurboPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        if(package.value("state").toBool() == false)
        {
            // Store turbo off
            m_turboState = false;

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

    void Vacuum::validateEnableTurboPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Store enable turbo
        m_turboState = package.value("state").toBool();

        if(m_turboState)
        {
            QVariantMap success;
            success.insert("requested_state", true);
            emit emit_validationSuccess(success);

            return;
        }

        QVariantMap fail;
        fail.insert("message", "the turbo pump could not be enabled");
        fail.insert("requested_state", true);
        fail.insert("current_stated", m_turboState);
        emit emit_validationFailed(fail);


    }

    void Vacuum::validateDisableBackingPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);
        qDebug()<<"validating disable pump";

        qDebug() << package;
    }

    void Vacuum::validateEnableBackingPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        //qDebug() << package;
    }

    void Vacuum::validateSetGasModeHeavy()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void Vacuum::validateSetGasModeMedium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void Vacuum::validateSetGasModeHelium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void Vacuum::validateGetBearingTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void Vacuum::validateGetTC110ElectronicsTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void Vacuum::validateGetPumpBottomTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void Vacuum::validateGetMotorTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }


}}}}
