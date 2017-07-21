#include "ReadVacStationTemperatures.h"

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
    ReadVacStationTemperatures::ReadVacStationTemperatures(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

    }

    ReadVacStationTemperatures::~ReadVacStationTemperatures()
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
    void ReadVacStationTemperatures::setParams(int vacSensorTimeInter)
    {       
        // Timer invertval for vac sensor
        params.insert("vacSensorTimeInter", vacSensorTimeInter);

        // Setup timers
        t_vacPressureMonitor.setInterval(vacSensorTimeInter);
    }


    /**
     * Start the state machine
     *
     * @brief ReadVacStationTemperatures::start
     */
    void ReadVacStationTemperatures::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief ReadVacStationTemperatures::start
     */
    void ReadVacStationTemperatures::stop()
    {
        // @todo

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readVacStationTemperaturesStopped(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief ReadVacStationTemperatures::stopAsFailed
     */
    void ReadVacStationTemperatures::stopAsFailed()
    {
        // @todo

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readVacStationTemperaturesFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadVacStationTemperatures::buildMachine
     */
    void ReadVacStationTemperatures::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sm_startVacuumPressureMonitor);
    }
}}}






