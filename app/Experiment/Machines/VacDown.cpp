#include "VacDown.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    VacDown::VacDown(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

    }

    VacDown::~VacDown()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void VacDown::setParams(int mintues, bool turbo, int gasMode)
    {
        // How long the vac should run
        params.insert("mintues", mintues);

        // Should the turbo ever be enabled
        params.insert("turbo", turbo);

        // What gas mode should be set
        params.insert("gasMode", gasMode);

    }


    /**
     * Start the state machine
     *
     * @brief VacDown::start
     */
    void VacDown::start()
    {
        machine.start();
    }


    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void VacDown::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sm_systemPressure);

        // Check the system pressure
        sm_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sm_validatePressureForVacuum);
            // Pressure is low enough
            sm_validatePressureForVacuum.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureInput);
            // Pressure is too high

        // Close the high pressure valve
        sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureInput);
            // Valve closed successfully
            // Valve failed to close

        // Close the nitrogen valve
            // Valve closed successfully
            // Valve failed to close

        // Close the flow controller valve
            // Valve closed successfully
            // Valve failed to close

        // Disable the vac station turbo
            // Turbo pump was disabled
            // Turbo pump could not be disabled

        // Start monitoring the vacuum sensor
            // Should the turbo turn on?

        // Turn the backing pump on
            // Backing pump turned on
            // Backing pump did not turn on

        // Start the timer for length of vac session
            // Timer expirered

        // Close machine
            // Disable turbo
            // Turn off backing pump
            // Disable vacuum sensor monitor

    }
}}}






