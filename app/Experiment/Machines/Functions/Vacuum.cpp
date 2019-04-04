#include "Vacuum.h"

#include <QString>

// Include functions helper
#include "FunctionsHelper.h"

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

    Vacuum::Vacuum(QObject *parent, Settings::Container *settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
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
        errorDetails.clear();
        errorDetails.insert("message", "the turbo pump is still spinner");
        errorDetails.insert("current_speed", turboSpeed);

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);
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
        errorDetails.clear();
        errorDetails.insert("message", "the turbo pump could not be disabled");
        errorDetails.insert("current_stated", package.value("state").toBool());
        errorDetails.insert("requested_state", false);

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);
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

        errorDetails.clear();
        errorDetails.insert("message", "the turbo pump could not be enabled");
        errorDetails.insert("requested_state", true);
        errorDetails.insert("current_stated", m_turboState);
        emit emit_validationFailed(errorDetails);


    }

    void Vacuum::validateDisableBackingPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(!package.value("state").toBool())
        {
            QVariantMap success;
            emit emit_validationSuccess(success);

            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Backing pump could not be disabled");
        errorDetails.insert("requested_state", false);
        errorDetails.insert("current_stated", package.value("state").toBool());
        emit emit_validationFailed(errorDetails);
    }

    void Vacuum::validateEnableBackingPump()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(package.value("state").toBool())
        {
            QVariantMap success;
            emit emit_validationSuccess(success);

            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Backing pump could not be enabled");
        errorDetails.insert("requested_state", true);
        errorDetails.insert("current_stated", package.value("state").toBool());
        emit emit_validationFailed(errorDetails);
    }

    void Vacuum::validateSetGasModeHeavy()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(package.value("mode").toInt() == 0)
        {
            QVariantMap success;
            emit emit_validationSuccess(success);

            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Could not set gas mode to heavy");
        errorDetails.insert("requested_mode", 0);
        errorDetails.insert("current_mode", package.value("mode").toInt());
        emit emit_validationFailed(errorDetails);

    }

    void Vacuum::validateSetGasModeMedium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(package.value("mode").toInt() == 1)
        {
            QVariantMap success;
            emit emit_validationSuccess(success);

            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Could not set gas mode to medium");
        errorDetails.insert("requested_mode", 1);
        errorDetails.insert("current_mode", package.value("mode").toInt());
        emit emit_validationFailed(errorDetails);
    }

    void Vacuum::validateSetGasModeHelium()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(package.value("mode").toInt() == 2)
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Could not set gas mode to helium");
        errorDetails.insert("requested_mode", 2);
        errorDetails.insert("current_mode", package.value("mode").toInt());
        emit emit_validationFailed(errorDetails);
    }

    void Vacuum::validateGetBearingTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        // Check state
        if(package.value("location").toInt() == 3)
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Could not get bearing temperature");
        errorDetails.insert("requested_location", 3);
        errorDetails.insert("current_location", package.value("location").toInt());
        errorDetails.insert("temperature", package.value("temperature").toInt());
        emit emit_validationFailed(errorDetails);
    }

    void Vacuum::validateGetTC110ElectronicsTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(package.value("location").toInt() == 2)
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Could not get TC110 electronics temperature");
        errorDetails.insert("requested_location", 2);
        errorDetails.insert("current_location", package.value("location").toInt());
        errorDetails.insert("temperature", package.value("temperature").toInt());
        emit emit_validationFailed(errorDetails);
    }

    void Vacuum::validateGetPumpBottomTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(package.value("location").toInt() == 1)
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Could not get pump bottom temperature");
        errorDetails.insert("requested_location", 1);
        errorDetails.insert("current_location", package.value("location").toInt());
        errorDetails.insert("temperature", package.value("temperature").toInt());
        emit emit_validationFailed(errorDetails);
    }

    void Vacuum::validateGetMotorTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check state
        if(package.value("location").toInt() == 4)
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
            return;
        }

        errorDetails.clear();
        errorDetails.insert("message", "Could not get motor temperature");
        errorDetails.insert("requested_location", 4);
        errorDetails.insert("current_location", package.value("location").toInt());
        errorDetails.insert("temperature", package.value("temperature").toInt());
        emit emit_validationFailed(errorDetails);
    }


}}}}
