#include "ReadVacStationTemperatures.h"

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
    void ReadVacStationTemperatures::setParams(int interval)
    {       
        // Timer invertval for vac sensor
        params.insert("interval", interval);

        // Setup timers
        t_vacStationTemperatureMonitor.setInterval(interval);
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
        machine.setInitialState(&m_vacuum->sm_getBearingTemperature);

        // Get the bearing temperture
        m_vacuum->sm_getBearingTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &m_vacuum->sm_validateGetBearingTemperature);
            // Success
            m_vacuum->sm_validateGetBearingTemperature.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &m_vacuum->sm_getTC110ElectronicsTemperature);
            // Failed
            // Com issue

        // Get the electronics temperature
        m_vacuum->sm_getTC110ElectronicsTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &m_vacuum->sm_validateGetTC110ElectronicsTemperature);
            // Success
            m_vacuum->sm_validateGetTC110ElectronicsTemperature.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &m_vacuum->sm_getMotorTemperature);
            // Failed
            // Com issue

        // Get the motor temperature
        m_vacuum->sm_getMotorTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &m_vacuum->sm_validateGetMotorTemperature);
            // Success
            m_vacuum->sm_validateGetMotorTemperature.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &m_vacuum->sm_getPumpBottomTemperature);
            // Failed
            // Com issue

        // Get the pump bottom temperature
        m_vacuum->sm_getPumpBottomTemperature.addTransition(&m_hardware, &Hardware::Access::emit_getTemperature, &m_vacuum->sm_validateGetPumpBottomTemperature);
            // Success
            m_vacuum->sm_validateGetPumpBottomTemperature.addTransition(this->vacuum(), &States::Vacuum::emit_validationSuccess, &sm_stop);
            // Failed
            // Com issue

        // Timer  @TODO IMPLIMENT THE TIMER TO TRIGGER CYCLING

    }
}}}











