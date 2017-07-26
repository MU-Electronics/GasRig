#include "ReadVacuum.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    ReadVacuum::ReadVacuum(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

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
        // Stop all the timers
        stopVacuumPressureMonitor();

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
        // Stop all the timers
        stopVacuumPressureMonitor();

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
        machine.setInitialState(&sm_startVacuumPressureMonitor);

        // Start the vacuum monitor
        sm_startVacuumPressureMonitor.addTransition(this, &MachineStates::emit_timerActive, &sm_timerWait);

        // Wait for a timer event
        sm_timerWait.addTransition(&t_vacPressureMonitor, &QTimer::timeout, &sm_vacPressure);

        // Read the vacuum sensor
        sm_vacPressure.addTransition(&m_hardware, &Hardware::Access::emit_readAnaloguePort, &sm_timerWait);

        // Account for com issues
        sm_vacPressure.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &sm_timerWait);
    }
}}}






