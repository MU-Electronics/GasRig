#include "ReadVacuum.h"

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
    ReadVacuum::ReadVacuum(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_vacPressureMonitor(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        connect(state("sml_vacPressure", true), &QState::entered, this->pressure(), &Functions::Pressure::vacPressure);
        connect(state("sml_startVacuumPressureMonitor", true), &QState::entered, this, &ReadVacuum::startVacuumPressureMonitor);
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
    void ReadVacuum::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ReadVacuum::start
     */
    void ReadVacuum::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadVacuum::buildMachine
     */
    void ReadVacuum::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("sml_startVacuumPressureMonitor", true));

        // Start the vacuum monitor
        state("sml_startVacuumPressureMonitor", true)->addTransition(&t_vacPressureMonitor, &QTimer::timeout, state("sml_vacPressure", true));

        // Read the vacuum sensor
        state("sml_vacPressure", true)->addTransition(&m_hardware, &Hardware::Access::emit_readVacuumPressure, state("sml_startVacuumPressureMonitor", true));

        // Account for com issues
        transitionsBuilder()->stateComErrors(state("sml_vacPressure", true), state("sml_startVacuumPressureMonitor", true));
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






