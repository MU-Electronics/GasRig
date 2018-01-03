#include "Pressure.h"

#include <cmath>

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

    Pressure::Pressure(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Pressure::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Monitor turbo
        connect(&m_hardware, &Hardware::Access::emit_setTurboPumpState, this, &Pressure::monitorTurboState);

        // Connect the states to functions
        connectStatesToMethods();
    }

    Pressure::~Pressure()
    {

    }

    void Pressure::connectStatesToMethods()
    {
    }



    /**
     * Request a reading of the system pressure
     *
     * @brief MachineStates::systemPressure
     */
    void Pressure::systemPressure()
    {
        emit hardwareRequest(m_commandConstructor.getPressureReading(1));
    }



    /**
     * Monitor the turbo state
     *
     * @brief Pressure::monitorTurboState
     */
    void Pressure::monitorTurboState(QVariantMap package )
    {
        // Status of turbo pump
        m_turboState = package.value("state").toBool();
    }



    /**
     * Validate a reading of the system pressure
     *
     * @brief Pressure::validateSystemPressure
     */
    void Pressure::validatePressureForVacuum()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Get the pressure
        float pressureIn = package.value("pressure").toFloat();

        // Get the max pressure allowed
        float maxPressure = m_settings.safety.vacuum.value("vacuum_from").toFloat();

        // Check the pressure is safe to vac down
        if(pressureIn < maxPressure)
        {
            // Store the stage info
            QVariantMap data;
            data.insert("pressure", pressureIn);

            // Emit safe to proceed
            emit emit_validationSuccess(data);

            return;
        }

        // Store the error
        errorDetails.clear();
        errorDetails.insert("message", "pressure in system is too high for the vac station; Exhuast the system first.");
        errorDetails.insert("system_pressure", pressureIn);
        errorDetails.insert("system_pressure_max", maxPressure);

        // Emit not safe to proceed
        emit emit_validationFailed(errorDetails);
    }


    void Pressure::vacPressure()
    {
        emit hardwareRequest(m_commandConstructor.getVacuumPressure( m_settings.hardware.vacuum_guage.value("connection").toString(),
                                                                     m_settings.hardware.vacuum_guage.value("slope").toDouble(),
                                                                     m_settings.hardware.vacuum_guage.value("offset").toDouble()));
    }

    void Pressure::validateVacPressureForTurbo()
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
            m_vacuumPressure = (std::pow(10, (1.667*package.value("calibrated").toDouble()-9.333)))/100;

            // Apply some hysteresis
            if( (m_turboState == false && m_vacuumPressure < setPressure) || (m_turboState == true && m_vacuumPressure < setPressure + 1) )
            {
                QVariantMap success;
                success.insert("pressure", m_pressureSensor);
                emit emit_validationSuccess(success);
                return;
            }
        }

        // Store the error
        errorDetails.clear();
        errorDetails.insert("message", "the vacuum pressure is too high for the turbo pump to be turned on");
        emit emit_validationFailed(errorDetails);
    }


}}}}
