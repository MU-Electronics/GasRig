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

            // States
        ,   sml_startVacuumTemperatureTimer(&machine)
        ,   sml_getBearingTemperature(&machine)
        ,   sml_getTC110ElectronicsTemperature(&machine)
        ,   sml_getMotorTemperature(&machine)
        ,   sml_getPumpBottomTemperature(&machine)

            // Timers
        ,   t_temperatureMonitor(parent)
    {
        // States
        connect(&sml_getBearingTemperature, &QState::entered, this->vacuum(), &Functions::Vacuum::getBearingTemperature);
        connect(&sml_getTC110ElectronicsTemperature, &QState::entered, this->vacuum(), &Functions::Vacuum::getTC110ElectronicsTemperature);
        connect(&sml_getMotorTemperature, &QState::entered, this->vacuum(), &Functions::Vacuum::getMotorTemperature);
        connect(&sml_getPumpBottomTemperature, &QState::entered, this->vacuum(), &Functions::Vacuum::getPumpBottomTemperature);

        // Timer
        connect(&sml_startVacuumTemperatureTimer, &QState::entered, this, &ReadVacStationTemperatures::startTemperatureTimer);
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
    void ReadVacStationTemperatures::start()
    {
        machine.start();
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
        machine.setInitialState(&sml_startVacuumTemperatureTimer);

        // Start timer
        sml_startVacuumTemperatureTimer.addTransition(&t_temperatureMonitor, &QTimer::timeout, &sml_getBearingTemperature);

        // Get the bearing temperture
        sml_getBearingTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &sml_getTC110ElectronicsTemperature);

        // Get the electronics temperature
        sml_getTC110ElectronicsTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &sml_getMotorTemperature);

        // Get the motor temperature
        sml_getMotorTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &sml_getPumpBottomTemperature);

        // Get the pump bottom temperature
        sml_getPumpBottomTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &sml_startVacuumTemperatureTimer);
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











