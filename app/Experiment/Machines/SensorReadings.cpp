#include "SensorReadings.h"

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
    SensorReadings::SensorReadings(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

    }

    SensorReadings::~SensorReadings()
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
    void SensorReadings::setParams(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter)
    {
        // Timer invertval for vac sensor
        params.insert("vacSensorTimeInter", vacSensorTimeInter);

        // Time interval for pressure sensor
        params.insert("pressureSensorTimeInter", pressureSensorTimeInter);

        // Time interval for flow sensor
        params.insert("flowControllerTimeInter", flowControllerTimeInter);
    }


    /**
     * Start the state machine
     *
     * @brief SensorReadings::start
     */
    void SensorReadings::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief SensorReadings::start
     */
    void SensorReadings::stop()
    {
        // Stop all the timers


        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_sensorReadingsStopped(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief SensorReadings::stopAsFailed
     */
    void SensorReadings::stopAsFailed()
    {
        // Stop all the timers

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_sensorReadingsFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief SensorReadings::buildMachine
     */
    void SensorReadings::buildMachine()
    {
        // Where to start the machine
        //machine.setInitialState(&sm_systemPressure);

        // Check the system pressure
        //sm_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sm_validatePressureForVacuum);

    }
}}}






