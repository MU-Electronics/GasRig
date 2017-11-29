#include "ReadFlowControllerFlow.h"

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
    ReadFlowControllerFlow::ReadFlowControllerFlow(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Flow states
        ,   sml_flowControllerOneFlow_1(&machine)
        ,   sml_flowControllerTwoFlow_1(&machine)

            // Timer states
        ,   sml_startFlowControllerFlowMonitor(&machine)

            // Timer
        ,   t_flowControllerFlowMonitor(parent)
    {
        // Flow
        connect(&sml_flowControllerOneFlow_1, &QState::entered, this->flow(), &Functions::Flow::flowControllerOneFlow);
        connect(&sml_flowControllerTwoFlow_1, &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoFlow);

        // Timers
        connect(&sml_startFlowControllerFlowMonitor, &QState::entered, this, &ReadFlowControllerFlow::startFlowControllerFlowMonitor);
    }

    ReadFlowControllerFlow::~ReadFlowControllerFlow()
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
    void ReadFlowControllerFlow::setParams(int flowControllerTimeInter)
    {       
        // Time interval for flow sensor
        params.insert("flowControllerTimeInter", flowControllerTimeInter);

        // Setup timer
        t_flowControllerFlowMonitor.setInterval(flowControllerTimeInter);
    }


    /**
     * Start the state machine
     *
     * @brief ReadFlowControllerFlow::start
     */
    void ReadFlowControllerFlow::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief ReadFlowControllerFlow::stopped
     */
    void ReadFlowControllerFlow::stopped()
    {
        // Stop all the timers
        stopFlowControllerFlowMonitor();
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadFlowControllerFlow::buildMachine
     */
    void ReadFlowControllerFlow::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sml_startFlowControllerFlowMonitor);

        // Start the flow controller flow monitor
        sml_startFlowControllerFlowMonitor.addTransition(&t_flowControllerFlowMonitor, &QTimer::timeout, &sml_flowControllerOneFlow_1);

        // Read the flow controller flow sensor
        sml_flowControllerOneFlow_1.addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerFlowRate, &sml_flowControllerTwoFlow_1);
        sml_flowControllerTwoFlow_1.addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerFlowRate, &sml_startFlowControllerFlowMonitor);

        // Account for com issues
        sml_flowControllerOneFlow_1.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &sml_startFlowControllerFlowMonitor);
        sml_flowControllerTwoFlow_1.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &sml_startFlowControllerFlowMonitor);
    }


    /**
     * Timer to use to trigger reading the flow controller flow sensor
     *
     * @brief ReadFlowControllerFlow::startPressureMonitor
     */
    void ReadFlowControllerFlow::startFlowControllerFlowMonitor()
    {
        // Setup timer
        t_flowControllerFlowMonitor.setSingleShot(false);
        t_flowControllerFlowMonitor.start();
    }


    /**
     * Stop the timer triggering reading of the flow controller flow sensor
     *
     * @brief ReadFlowControllerFlow::stopPressureMonitor
     */
    void ReadFlowControllerFlow::stopFlowControllerFlowMonitor()
    {
        t_flowControllerFlowMonitor.stop();
    }
}}}






