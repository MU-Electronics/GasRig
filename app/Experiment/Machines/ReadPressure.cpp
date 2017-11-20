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

            // States
        ,   sml_startPressureMonitor(&machine)
        ,   sml_systemPressure(&machine)

            // Timers
        ,   t_pressureMonitor(parent)
    {
        connect(&sml_systemPressure, &QState::entered, this->pressure(), &States::Pressure::systemPressure);
        connect(&sml_startPressureMonitor, &QState::entered, this, &ReadPressure::startPressureMonitor);
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
        machine.setInitialState(&sml_startPressureMonitor);

        // Start the pressure monitor
        sml_startPressureMonitor.addTransition(&t_pressureMonitor, &QTimer::timeout, &sml_systemPressure);

        // Read the pressure sensor
        sml_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_startPressureMonitor);

        // Account for com issues
        sml_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &sml_startPressureMonitor);
    }


    /**
     * Timer to use to trigger reading the pressure sensor
     *
     * @brief ReadPressure::startPressureMonitor
     */
    void ReadPressure::startPressureMonitor()
    {
        // Setup timer
        t_pressureMonitor.setSingleShot(true);
        t_pressureMonitor.start();
    }
}}}






