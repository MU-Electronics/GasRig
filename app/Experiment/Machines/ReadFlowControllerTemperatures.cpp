#include "ReadFlowControllerTemperatures.h"

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
    ReadFlowControllerTemperatures::ReadFlowControllerTemperatures(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_flowControllerTemperatureMonitor(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Flow temperature
        connect(state("sml_readTemperature_1", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneTemperature);
        connect(state("sml_readTemperature_2", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoTemperature);

        // Timers
        connect(state("sml_startFlowControllerMonitor", true), &QState::entered, this, &ReadFlowControllerTemperatures::startFlowControllerTemperatureMonitor);
    }

    ReadFlowControllerTemperatures::~ReadFlowControllerTemperatures()
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
    void ReadFlowControllerTemperatures::setParams(int interval)
    {
        // Timer invertval for vac sensor
        params.insert("interval", interval);

        // Setup timers
        t_flowControllerTemperatureMonitor.setInterval(interval);
    }


    /**
     * Start the state machine
     *
     * @brief ReadFlowControllerTemperatures::start
     */
    void ReadFlowControllerTemperatures::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ReadFlowControllerTemperatures::stopped
     */
    void ReadFlowControllerTemperatures::stopped()
    {
    }



    /**
     * Builds the machine connections
     *
     * @brief ReadFlowControllerTemperatures::buildMachine
     */
    void ReadFlowControllerTemperatures::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("sml_startFlowControllerMonitor", true));

        // Start the flow controller temperature monitor
        state("sml_startFlowControllerMonitor", true)->addTransition(&t_flowControllerTemperatureMonitor, &QTimer::timeout, state("sml_readTemperature_1", true));

        // Read flow rate
        state("sml_readTemperature_1", true)->addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerTemperature, state("sml_readTemperature_2", true));
        state("sml_readTemperature_2", true)->addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerTemperature, state("sml_startFlowControllerMonitor", true));

        // Account for com issues
        transitionsBuilder()->stateComErrors(state("sml_readTemperature_1", true), state("sml_startFlowControllerMonitor", true));
        transitionsBuilder()->stateComErrors(state("sml_readTemperature_2", true), state("sml_startFlowControllerMonitor", true));
    }


    /**
     * Timer to use to trigger reading the flow controller temperature sensor
     *
     * @brief ReadFlowControllerTemperatures::startFlowControllerTemperatureMonitor
     */
    void ReadFlowControllerTemperatures::startFlowControllerTemperatureMonitor()
    {
        // Setup timer
        t_flowControllerTemperatureMonitor.setSingleShot(true);
        t_flowControllerTemperatureMonitor.start();
    }
}}}







