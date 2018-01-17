#include "ReadTurboSpeed.h"

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
    ReadTurboSpeed::ReadTurboSpeed(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // States
        ,   sml_startTurboSpeedTimer(&machine)
        ,   sml_readTurboSpeed(&machine)

            // Timers
        ,   t_turboSpeed(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        connect(&sml_readTurboSpeed, &QState::entered, this->vacuum(), &Functions::Vacuum::getTurboSpeed);
        connect(&sml_startTurboSpeedTimer, &QState::entered, this, &ReadTurboSpeed::startTurboTimer);
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
        t_turboSpeed.setInterval(interval);
    }


    /**
     * Start the state machine
     *
     * @brief ReadTurboSpeed::start
     */
    void ReadTurboSpeed::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ReadTurboSpeed::stopped
     */
    void ReadTurboSpeed::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadTurboSpeed::buildMachine
     */
    void ReadTurboSpeed::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(&sml_startTurboSpeedTimer);

        // Start the speed monitor
        sml_startTurboSpeedTimer.addTransition(&t_turboSpeed, &QTimer::timeout, &sml_readTurboSpeed);

        // Read the speed sensor
        sml_readTurboSpeed.addTransition(&m_hardware, &Hardware::Access::emit_getTurboSpeed, &sml_startTurboSpeedTimer);

        // Account for com issues
        transitionsBuilder()->stateComErrors(&sml_readTurboSpeed, &sml_startTurboSpeedTimer);
    }


    /**
     * Timer to use to trigger reading the turbo speed
     *
     * @brief ReadTurboSpeed::startTurboTimer
     */
    void ReadTurboSpeed::startTurboTimer()
    {
        // Setup timer
        t_turboSpeed.setSingleShot(true);
        t_turboSpeed.start();
    }
}}}






