#include "ReadPressure.h"

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
    ReadPressure::ReadPressure(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

    }

    ReadPressure::~ReadPressure()
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
    void ReadPressure::setParams(int pressureSensorTimeInter)
    {       
        // Time interval for pressure sensor
        params.insert("pressureSensorTimeInter", pressureSensorTimeInter);

        // Setup timers
        t_pressureMonitor.setInterval(pressureSensorTimeInter);
    }


    /**
     * Start the state machine
     *
     * @brief ReadPressure::start
     */
    void ReadPressure::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief ReadPressure::start
     */
    void ReadPressure::stop()
    {
        // Stop all the timers
        stopPressureMonitor();

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readPressureStopped(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief ReadPressure::stopAsFailed
     */
    void ReadPressure::stopAsFailed()
    {
        // Stop all the timers
        stopPressureMonitor();

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readPressureFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadPressure::buildMachine
     */
    void ReadPressure::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sm_startPressureMonitor);

        // Start the pressure monitor
        sm_startPressureMonitor.addTransition(this, &States::MachineStates::emit_timerActive, &sm_timerWait);

        // Wait for a timer event
        sm_timerWait.addTransition(&t_pressureMonitor, &QTimer::timeout, &m_pressure->sm_systemPressure);

        // Read the pressure sensor
        m_pressure->sm_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sm_timerWait);

        // Account for com issues
        m_pressure->sm_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &sm_timerWait);

    }
}}}






