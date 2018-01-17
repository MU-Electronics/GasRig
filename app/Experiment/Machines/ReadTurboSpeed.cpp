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

            // Timers
        ,   t_turboSpeed(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        connect(state("sml_readTurboSpeed", true), &QState::entered, this->vacuum(), &Functions::Vacuum::getTurboSpeed);
        connect(state("sml_startTurboSpeedTimer", true), &QState::entered, this, &ReadTurboSpeed::startTurboTimer);
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
        sm_master.setInitialState(state("sml_startTurboSpeedTimer", true));

        // Start the speed monitor
        state("sml_startTurboSpeedTimer", true)->addTransition(&t_turboSpeed, &QTimer::timeout, state("sml_readTurboSpeed", true));

        // Read the speed sensor
        state("sml_readTurboSpeed", true)->addTransition(&m_hardware, &Hardware::Access::emit_getTurboSpeed, state("sml_startTurboSpeedTimer", true));

        // Account for com issues
        transitionsBuilder()->stateComErrors(state("sml_readTurboSpeed", true), state("sml_startTurboSpeedTimer", true));
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






