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
        timers()->t_vacPressureMonitor.setInterval(vacSensorTimeInter);
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
        timers()->stopVacuumPressureMonitor();

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
        timers()->stopVacuumPressureMonitor();

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
        machine.setInitialState(&timers()->sm_startVacuumPressureMonitor);

        // Start the vacuum monitor
        timers()->sm_startVacuumPressureMonitor.addTransition(this->timers(), &States::Timers::emit_timerActive, &timers()->sm_timerWait);

        // Wait for a timer event
        timers()->sm_timerWait.addTransition(&timers()->t_vacPressureMonitor, &QTimer::timeout, &pressure()->sm_vacPressure);

        // Read the vacuum sensor
        pressure()->sm_vacPressure.addTransition(&m_hardware, &Hardware::Access::emit_readAnaloguePort, &timers()->sm_timerWait);

        // Account for com issues
        pressure()->sm_vacPressure.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &timers()->sm_timerWait);
    }
}}}






