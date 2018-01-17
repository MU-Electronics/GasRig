#include "ReadPressure.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "Functions/MachineStates.h"

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
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        connect(&sml_systemPressure, &QState::entered, this->pressure(), &Functions::Pressure::systemPressure);
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
    void ReadPressure::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ReadPressure::stopped
     */
    void ReadPressure::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadPressure::buildMachine
     */
    void ReadPressure::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(&sml_startPressureMonitor);

        // Start the pressure monitor
        sml_startPressureMonitor.addTransition(&t_pressureMonitor, &QTimer::timeout, &sml_systemPressure);

        // Read the pressure sensor
        sml_systemPressure.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_startPressureMonitor);

        // Account for com issues
        transitionsBuilder()->stateComErrors(&sml_systemPressure, &sml_startPressureMonitor);
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






