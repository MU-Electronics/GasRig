#include "ReadFlowControllerFlow.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    ReadFlowControllerFlow::ReadFlowControllerFlow(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

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
     * @brief ReadFlowControllerFlow::start
     */
    void ReadFlowControllerFlow::stop()
    {
        // Stop all the timers
        stopFlowControllerFlowMonitor();

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readFlowControllerFlowStopped(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief ReadFlowControllerFlow::stopAsFailed
     */
    void ReadFlowControllerFlow::stopAsFailed()
    {
        // Stop all the timers
        stopFlowControllerFlowMonitor();

        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_readFlowControllerFlowFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief ReadFlowControllerFlow::buildMachine
     */
    void ReadFlowControllerFlow::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sm_startFlowControllerFlowMonitor);

        // Start the flow controller flow monitor
        sm_startFlowControllerFlowMonitor.addTransition(this, &MachineStates::emit_timerActive, &sm_timerWait);

        // Wait for a timer event
        sm_timerWait.addTransition(&t_flowControllerFlowMonitor, &QTimer::timeout, &sm_flowControllerOneFlow);

        // Read the flow controller flow sensor
        sm_flowControllerOneFlow.addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerFlowRate, &sm_flowControllerTwoFlow);
        sm_flowControllerTwoFlow.addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerFlowRate, &sm_timerWait);

        // Account for com issues
        sm_flowControllerTwoFlow.addTransition(&m_hardware, &Hardware::Access::emit_timeoutSerialError, &sm_timerWait);
    }
}}}





