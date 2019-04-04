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
    ReadPressure::ReadPressure(QObject *parent, Settings::Container* settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_pressureMonitor(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        connect(state("systemPressure", true), &QState::entered, this->pressure(), &Functions::Pressure::systemPressure);
        connect(validator("systemPressure", true), &QState::entered, this->pressure(), &Functions::Pressure::validatePressure);
        connect(state("startPressureMonitor", true), &QState::entered, this, &ReadPressure::startPressureMonitor);
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
        sm_master.setInitialState(state("startPressureMonitor", true));

        // Start the pressure monitor
        state("startPressureMonitor", true)->addTransition(&t_pressureMonitor, &QTimer::timeout, state("systemPressure", true));

        // Read the pressure sensor
        state("systemPressure", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("systemPressure", true));
            // Validate
            validator("systemPressure", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, state("startPressureMonitor", true));
            validator("systemPressure", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, state("startPressureMonitor", true));

        // Account for com issues
        transitionsBuilder()->stateComErrors(state("systemPressure", true), state("startPressureMonitor", true));
        transitionsBuilder()->stateComErrors(validator("systemPressure", true), state("startPressureMonitor", true));
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






