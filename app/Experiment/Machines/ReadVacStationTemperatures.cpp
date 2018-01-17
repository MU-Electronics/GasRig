#include "ReadVacStationTemperatures.h"

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
    ReadVacStationTemperatures::ReadVacStationTemperatures(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_temperatureMonitor(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // States
        connect(state("sml_getBearingTemperature", true), &QState::entered, this->vacuum(), &Functions::Vacuum::getBearingTemperature);
        connect(state("sml_getTC110ElectronicsTemperature", true), &QState::entered, this->vacuum(), &Functions::Vacuum::getTC110ElectronicsTemperature);
        connect(state("sml_getMotorTemperature", true), &QState::entered, this->vacuum(), &Functions::Vacuum::getMotorTemperature);
        connect(state("sml_getPumpBottomTemperature", true), &QState::entered, this->vacuum(), &Functions::Vacuum::getPumpBottomTemperature);

        // Timer
        connect(state("sml_startVacuumTemperatureTimer", true), &QState::entered, this, &ReadVacStationTemperatures::startTemperatureTimer);
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
    void ReadVacStationTemperatures::setParams(int interval)
    {       
        // Timer invertval for vac sensor
        params.insert("interval", interval);

        // Setup timers
        t_temperatureMonitor.setInterval(interval);
    }


    /**
     * Start the state machine
     *
     * @brief ReadVacStationTemperatures::start
     */
    void ReadVacStationTemperatures::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ReadVacStationTemperatures::stopped
     */
    void ReadVacStationTemperatures::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadVacStationTemperatures::buildMachine
     */
    void ReadVacStationTemperatures::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("sml_startVacuumTemperatureTimer", true));

        // Start timer
        state("sml_startVacuumTemperatureTimer", true)->addTransition(&t_temperatureMonitor, &QTimer::timeout, state("sml_getBearingTemperature", true));

        // Get the bearing temperture
        state("sml_getBearingTemperature", true)->addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, state("sml_getTC110ElectronicsTemperature", true));
            // Account for com issues
            transitionsBuilder()->stateComErrors(state("sml_getBearingTemperature", true), state("sml_getTC110ElectronicsTemperature", true));

        // Get the electronics temperature
        state("sml_getTC110ElectronicsTemperature", true)->addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, state("sml_getMotorTemperature", true));
            // Account for com issues
            transitionsBuilder()->stateComErrors(state("sml_getTC110ElectronicsTemperature", true), state("sml_getMotorTemperature", true));

        // Get the motor temperature
        state("sml_getMotorTemperature", true)->addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, state("sml_getPumpBottomTemperature", true));
            // Account for com issues
            transitionsBuilder()->stateComErrors(state("sml_getMotorTemperature", true), state("sml_getPumpBottomTemperature", true));

        // Get the pump bottom temperature
        state("sml_getPumpBottomTemperature", true)->addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, state("sml_startVacuumTemperatureTimer", true));
            // Account for com issues
            transitionsBuilder()->stateComErrors(state("sml_getPumpBottomTemperature", true), state("sml_startVacuumTemperatureTimer", true));
    }


    /**
     * Timer to use to trigger reading the vac station temperature
     *
     * @brief ReadVacStationTemperatures::startTemperatureTimer
     */
    void ReadVacStationTemperatures::startTemperatureTimer()
    {
        // Setup timer
        t_temperatureMonitor.setSingleShot(true);
        t_temperatureMonitor.start();
    }
}}}











