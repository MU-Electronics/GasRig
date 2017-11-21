#include "ReadVacuum.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "States/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    ReadVacuum::ReadVacuum(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // States
        ,   sml_vacPressure(&machine)
        ,   sml_startVacuumPressureMonitor(&machine)

            // Timers
        ,   t_vacPressureMonitor(parent)
    {
        connect(&sml_vacPressure, &QState::entered, this->pressure(), &States::Pressure::vacPressure);
        connect(&sml_startVacuumPressureMonitor, &QState::entered, this, &ReadVacuum::startVacuumPressureMonitor);
    }

    ReadVacuum::~ReadVacuum()
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
    void ReadVacuum::setParams(int vacSensorTimeInter)
    {
        // Timer invertval for vac sensor
        params.insert("vacSensorTimeInter", vacSensorTimeInter);

        // Setup timers
        t_vacPressureMonitor.setInterval(vacSensorTimeInter);
    }


    /**
     * Start the state machine
     *
     * @brief ReadVacuum::start
     */
    void ReadVacuum::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief ReadVacuum::start
     */
    void ReadVacuum::stop()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readVacuumStopped(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief ReadVacuum::stopAsFailed
     */
    void ReadVacuum::stopAsFailed()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readVacuumFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadVacuum::buildMachine
     */
    void ReadVacuum::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sml_startVacuumPressureMonitor);

        // Start the vacuum monitor
        sml_startVacuumPressureMonitor.addTransition(&t_vacPressureMonitor, &QTimer::timeout, &sml_vacPressure);

        // Read the vacuum sensor
        sml_vacPressure.addTransition(&m_hardware, &Hardware::Access::emit_readVacuumPressure, &sml_startVacuumPressureMonitor);

        // Account for com issues
        sml_vacPressure.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &sml_startVacuumPressureMonitor);
    }

    /**
     * Timer to use to trigger reading the vacuum sensor
     *
     * @brief ReadVacuum::startVacuumPressureMonitor
     */
    void ReadVacuum::startVacuumPressureMonitor()
    {
        // Setup timer
        t_vacPressureMonitor.setSingleShot(true);
        t_vacPressureMonitor.start();
    }
}}}






