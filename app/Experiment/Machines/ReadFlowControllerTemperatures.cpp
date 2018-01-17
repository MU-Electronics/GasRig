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

            // States
        ,   sml_readTemperature_1(&machine)
        ,   sml_readTemperature_2(&machine)
        ,   sml_startFlowControllerMonitor(&machine)

            // Timers
        ,   t_flowControllerTemperatureMonitor(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Flow temperature
        connect(&sml_readTemperature_1, &QState::entered, this->flow(), &Functions::Flow::flowControllerOneTemperature);
        connect(&sml_readTemperature_2, &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoTemperature);

        // Timers
        connect(&sml_startFlowControllerMonitor, &QState::entered, this, &ReadFlowControllerTemperatures::startFlowControllerTemperatureMonitor);
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
        machine.setInitialState(&sml_startFlowControllerMonitor);

        // Start the flow controller temperature monitor
        sml_startFlowControllerMonitor.addTransition(&t_flowControllerTemperatureMonitor, &QTimer::timeout, &sml_readTemperature_1);

        // Read flow rate
        sml_readTemperature_1.addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerTemperature, &sml_readTemperature_2);
        sml_readTemperature_2.addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerTemperature, &sml_startFlowControllerMonitor);

        // Account for com issues
        transitionsBuilder()->stateComErrors(&sml_readTemperature_1, &sml_startFlowControllerMonitor);
        transitionsBuilder()->stateComErrors(&sml_readTemperature_2, &sml_startFlowControllerMonitor);
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







