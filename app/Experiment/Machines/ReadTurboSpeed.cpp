#include "ReadTurboSpeed.h"

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
    ReadTurboSpeed::ReadTurboSpeed(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

    }

    ReadTurboSpeed::~ReadTurboSpeed()
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
    void ReadTurboSpeed::setParams(int interval)
    {       
        // Timer invertval for vac sensor
        params.insert("interval", interval);

        // Setup timers
        t_turboSpeedMonitor.setInterval(interval);
    }


    /**
     * Start the state machine
     *
     * @brief ReadTurboSpeed::start
     */
    void ReadTurboSpeed::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief ReadTurboSpeed::start
     */
    void ReadTurboSpeed::stop()
    {
        // @todo

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readTurboSpeedStopped(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief ReadTurboSpeed::stopAsFailed
     */
    void ReadTurboSpeed::stopAsFailed()
    {
        // @todo

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readTurboSpeedFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadTurboSpeed::buildMachine
     */
    void ReadTurboSpeed::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sm_startVacuumPressureMonitor);
    }
}}}






