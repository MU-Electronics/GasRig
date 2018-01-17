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

            // Timer
        ,   t_flowControllerFlowMonitor(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Flow
        connect(state("flowControllerOneFlow", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneFlow);
        connect(state("flowControllerTwoFlow", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoFlow);

        connect(validator("flowControllerOneFlow", true), &QState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneFlow);
        connect(validator("flowControllerTwoFlow", true), &QState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoFlow);


        // Timers
        connect(state("startFlowControllerFlowMonitor", true), &QState::entered, this, &ReadFlowControllerFlow::startFlowControllerFlowMonitor);
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
    void ReadFlowControllerFlow::beforeStart()
    {

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
        sm_master.setInitialState(state("startFlowControllerFlowMonitor", true));

        // Start the flow controller flow monitor
        state("startFlowControllerFlowMonitor", true)->addTransition(&t_flowControllerFlowMonitor, &QTimer::timeout, state("flowControllerOneFlow", true));

        // Read the flow controller flow sensor
        state("flowControllerOneFlow", true)->addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerFlowRate, validator("flowControllerOneFlow", true));
            // Validate flow controller output
            validator("flowControllerOneFlow", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, state("flowControllerTwoFlow", true));
            validator("flowControllerOneFlow", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("flowControllerTwoFlow", true));

        state("flowControllerTwoFlow", true)->addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerFlowRate, validator("flowControllerTwoFlow", true));
            // Validate flow controller output
            validator("flowControllerTwoFlow", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, state("startFlowControllerFlowMonitor", true));
            validator("flowControllerTwoFlow", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("startFlowControllerFlowMonitor", true));


        // Account for com issues
        transitionsBuilder()->stateComErrors(state("flowControllerOneFlow", true), state("startFlowControllerFlowMonitor", true));
        transitionsBuilder()->stateComErrors(state("flowControllerTwoFlow", true), state("startFlowControllerFlowMonitor", true));
        transitionsBuilder()->stateComErrors(validator("flowControllerOneFlow", true), state("startFlowControllerFlowMonitor", true));
        transitionsBuilder()->stateComErrors(validator("flowControllerTwoFlow", true), state("startFlowControllerFlowMonitor", true));
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






