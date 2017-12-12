#include "Valves.h"

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

    Valves::Valves(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
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

        // Generate command
        QVariantMap command = m_commandConstructor.setValveState(valveName, state);

        // Register id
        registerId(command.value("command_identifier").toString());

        // Emit siganl to HAL
        emit hardwareRequest(command);
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
        CommandValidatorState* command = dynamic_cast<CommandValidatorState*>(sender());

        // Failed data to passon
        QVariantMap returnPackage;

        // If cast successfull
        if(command != NULL && command->package.value("cast_status").toBool())
        {
            // Get the package data from the instance
            QVariantMap package = command->package;

            // Find the correct valve name
            QString valveName = m_settings.hardware.valve_connections.value(number).toString();

            // If id was not registered then the signal name was correct but the signal is not
            if(!isRegister(package.value("command_identifier").toString()))
            {
                returnPackage.insert("message", "ID was wrong in the package, not an error but skipping signal");
                returnPackage.insert("acutal_id", package.value("command_identifier").toInt());
                returnPackage.insert("ids", stringOfIds());

                // Tell everyone the signal was wrong
                emit emit_validationWrongId(returnPackage);

                // Do nothing else
                return;
            }

            // Check valve is the same
            if(package.value("port").toString() == valveName && state == package.value("value").toBool())
            {
                // Data to pass on
                returnPackage.insert("requested_valve", valveName);
                returnPackage.insert("requested_valve_id", number);
                returnPackage.insert("valve_changed", package.value("port").toString());
                returnPackage.insert("requested_state", state);
                returnPackage.insert("state", package.value("value").toBool());

                // Emit safe to proceed
                emit emit_validationSuccess(returnPackage);

                return;
            }
            qDebug() << "fail";

            // Failed data to passon
            returnPackage.insert("message", "The valve failed to update correctly");
            returnPackage.insert("requested_valve", valveName);
            returnPackage.insert("requested_valve_id", number);
            returnPackage.insert("valve_changed", package.value("port").toString());
            returnPackage.insert("requested_state", state);
            returnPackage.insert("state", package.value("value").toBool());
        }
        else
        {
            returnPackage.insert("message", "Validation casting failed");
            returnPackage.insert("requested_valve_id", number);
            returnPackage.insert("requested_state", state);
        }

        // Emit not safe to proceed
        emit emit_validationFailed(returnPackage);
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
        valveHelper("2", false);
    }

    void Valves::closeFastExhuastPath()
    {
        valveHelper("4", false);
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
        validateValveHelper("2", false);
    }

    void Valves::validateCloseFastExhuastPath()
    {
        validateValveHelper("4", false);
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
        valveHelper("2", true);
    }

    void Valves::openFastExhuastPath()
    {
        valveHelper("4", true);
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
        validateValveHelper("2", true);
    }

    void Valves::validateOpenFastExhuastPath()
    {
        validateValveHelper("4", true);
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
