#include "Valves.h"

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

namespace App { namespace Experiment { namespace Machines { namespace States
{

    Valves::Valves(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)

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

    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Valves::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    Valves::~Valves()
    {

    }

    void Valves::connectStatesToMethods()
    {
        // Link close valve states
        connect(&sm_closeHighPressureInput, &QState::entered, this, &Valves::closeHighPressureInput);
        connect(&sm_closeHighPressureNitrogen, &QState::entered, this, &Valves::closeHighPressureNitrogen);
        connect(&sm_closeFlowController, &QState::entered, this, &Valves::closeFlowController);
        connect(&sm_closeExhuast, &QState::entered, this, &Valves::closeExhuast);
        connect(&sm_closeOutput, &QState::entered, this, &Valves::closeOutput);
        connect(&sm_closeSlowExhuastPath, &QState::entered, this, &Valves::closeSlowExhuastPath);
        connect(&sm_closeFastExhuastPath, &QState::entered, this, &Valves::closeFastExhuastPath);
        connect(&sm_closeVacuumIn, &QState::entered, this, &Valves::closeVacuumIn);
        connect(&sm_closeVacuumOut, &QState::entered, this, &Valves::closeVacuumOut);

        // Link close valve validator states
        connect(&sm_validateCloseHighPressureInput, &CommandValidatorState::entered, this, &Valves::validateCloseHighPressureInput);
        connect(&sm_validateCloseHighPressureNitrogen, &CommandValidatorState::entered, this, &Valves::validateCloseHighPressureNitrogen);
        connect(&sm_validateCloseFlowController, &CommandValidatorState::entered, this, &Valves::validateCloseFlowController);
        connect(&sm_validateCloseExhuast, &CommandValidatorState::entered, this, &Valves::validateCloseExhuast);
        connect(&sm_validateCloseOutput, &CommandValidatorState::entered, this, &Valves::validateCloseOutput);
        connect(&sm_validateCloseSlowExhuastPath, &CommandValidatorState::entered, this, &Valves::validateCloseSlowExhuastPath);
        connect(&sm_validateCloseFastExhuastPath, &CommandValidatorState::entered, this, &Valves::validateCloseFastExhuastPath);
        connect(&sm_validateCloseVacuumIn, &CommandValidatorState::entered, this, &Valves::validateCloseVacuumIn);
        connect(&sm_validateCloseVacuumOut, &CommandValidatorState::entered, this, &Valves::validateCloseVacuumOut);

        // Link open valve states
        connect(&sm_openHighPressureInput, &QState::entered, this, &Valves::openHighPressureInput);
        connect(&sm_openHighPressureNitrogen, &QState::entered, this, &Valves::openHighPressureNitrogen);
        connect(&sm_openFlowController, &QState::entered, this, &Valves::openFlowController);
        connect(&sm_openExhuast, &QState::entered, this, &Valves::openExhuast);
        connect(&sm_openOutput, &QState::entered, this, &Valves::openOutput);
        connect(&sm_openSlowExhuastPath, &QState::entered, this, &Valves::openSlowExhuastPath);
        connect(&sm_openFastExhuastPath, &QState::entered, this, &Valves::openFastExhuastPath);
        connect(&sm_openVacuumIn, &QState::entered, this, &Valves::openVacuumIn);
        connect(&sm_openVacuumOut, &QState::entered, this, &Valves::openVacuumOut);

        // Link open valve validator states
        connect(&sm_validateOpenHighPressureInput, &CommandValidatorState::entered, this, &Valves::validateOpenHighPressureInput);
        connect(&sm_validateOpenHighPressureNitrogen, &CommandValidatorState::entered, this, &Valves::validateOpenHighPressureNitrogen);
        connect(&sm_validateOpenFlowController, &CommandValidatorState::entered, this, &Valves::validateOpenFlowController);
        connect(&sm_validateOpenExhuast, &CommandValidatorState::entered, this, &Valves::validateOpenExhuast);
        connect(&sm_validateOpenOutput, &CommandValidatorState::entered, this, &Valves::validateOpenOutput);
        connect(&sm_validateOpenSlowExhuastPath, &CommandValidatorState::entered, this, &Valves::validateOpenSlowExhuastPath);
        connect(&sm_validateOpenFastExhuastPath, &CommandValidatorState::entered, this, &Valves::validateOpenFastExhuastPath);
        connect(&sm_validateOpenVacuumIn, &CommandValidatorState::entered, this, &Valves::validateOpenVacuumIn);
        connect(&sm_validateOpenVacuumOut, &CommandValidatorState::entered, this, &Valves::validateOpenVacuumOut);
    }

    /**
     * Open valve helper
     *
     * @brief Valves::openValveHelper
     * @param number
     */
    void Valves::valveHelper(QString number, bool state)
    {
        // Find the correct valve name
        QString valveName = m_settings.hardware.valve_connections.value(number).toString();

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setValveState(valveName, state));
    }

    /**
     * Vavlidate valve helper
     *
     * @brief Valves::openValveHelper
     * @param number
     */
    void Valves::validateValveHelper(QString number, bool state)
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
     * @brief Valves::systemPressure
     */
    void Valves::closeHighPressureInput()
    {
        valveHelper("7", false);
    }

    void Valves::closeHighPressureNitrogen()
    {
        valveHelper("9", false);
    }

    void Valves::closeFlowController()
    {
        valveHelper("8", false);
    }

    void Valves::closeExhuast()
    {
        valveHelper("3", false);
    }

    void Valves::closeOutput()
    {
        valveHelper("1", false);
    }

    void Valves::closeSlowExhuastPath()
    {
        valveHelper("4", false);
    }

    void Valves::closeFastExhuastPath()
    {
        valveHelper("2", false);
    }

    void Valves::closeVacuumIn()
    {
        valveHelper("5", false);
    }

    void Valves::closeVacuumOut()
    {
        valveHelper("6", false);
    }


    /**
     * Request closing of the high pressure input valve
     *
     * @brief MachineStates::systemPressure
     */
    void Valves::validateCloseHighPressureInput()
    {
        validateValveHelper("7", false);
    }

    void Valves::validateCloseHighPressureNitrogen()
    {
        validateValveHelper("9", false);
    }

    void Valves::validateCloseFlowController()
    {
        validateValveHelper("8", false);
    }

    void Valves::validateCloseExhuast()
    {
        validateValveHelper("3", false);
    }

    void Valves::validateCloseOutput()
    {
        validateValveHelper("1", false);
    }

    void Valves::validateCloseSlowExhuastPath()
    {
        validateValveHelper("4", false);
    }

    void Valves::validateCloseFastExhuastPath()
    {
        validateValveHelper("2", false);
    }

    void Valves::validateCloseVacuumIn()
    {
        validateValveHelper("5", false);
    }

    void Valves::validateCloseVacuumOut()
    {
        validateValveHelper("6", false);
    }






    void Valves::openHighPressureInput()
    {
        valveHelper("7", true);
    }

    void Valves::openHighPressureNitrogen()
    {
        valveHelper("9", true);
    }

    void Valves::openFlowController()
    {
        valveHelper("8", true);
    }

    void Valves::openExhuast()
    {
        valveHelper("3", true);
    }

    void Valves::openOutput()
    {
        valveHelper("1", true);
    }

    void Valves::openSlowExhuastPath()
    {
        valveHelper("4", true);
    }

    void Valves::openFastExhuastPath()
    {
        valveHelper("2", true);
    }

    void Valves::openVacuumIn()
    {
        valveHelper("5", true);
    }

    void Valves::openVacuumOut()
    {
        valveHelper("6", true);
    }





    void Valves::validateOpenHighPressureInput()
    {
        validateValveHelper("7", true);
    }

    void Valves::validateOpenHighPressureNitrogen()
    {
        validateValveHelper("9", true);
    }

    void Valves::validateOpenFlowController()
    {
        validateValveHelper("8", true);
    }

    void Valves::validateOpenExhuast()
    {
        validateValveHelper("3", true);
    }

    void Valves::validateOpenOutput()
    {
        validateValveHelper("1", true);
    }

    void Valves::validateOpenSlowExhuastPath()
    {
        validateValveHelper("4", true);
    }

    void Valves::validateOpenFastExhuastPath()
    {
        validateValveHelper("2", true);
    }

    void Valves::validateOpenVacuumIn()
    {
        validateValveHelper("5", true);
    }

    void Valves::validateOpenVacuumOut()
    {
        validateValveHelper("6", true);
    }

}}}}
