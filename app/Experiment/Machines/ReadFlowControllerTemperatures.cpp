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
    ReadFlowControllerTemperatures::ReadFlowControllerTemperatures(QObject *parent, Settings::Container* settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_flowControllerTemperatureMonitor(parent)
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        // Flow temperature
        connect(state("readTemperature_1", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneTemperature);
        connect(state("readTemperature_2", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoTemperature);

        connect(validator("readTemperature_1", true), &QState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneTemperature);
        connect(validator("readTemperature_2", true), &QState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoTemperature);


        // Timers
        connect(state("startFlowControllerMonitor", true), &QState::entered, this, &ReadFlowControllerTemperatures::startFlowControllerTemperatureMonitor);
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
        sm_master.setInitialState(state("startFlowControllerMonitor", true));

        // Start the flow controller temperature monitor
        state("startFlowControllerMonitor", true)->addTransition(&t_flowControllerTemperatureMonitor, &QTimer::timeout, state("readTemperature_1", true));

        // Read flow rate
        state("readTemperature_1", true)->addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerTemperature, validator("readTemperature_1", true));
            // Validate
            validator("readTemperature_1", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, state("readTemperature_2", true));
            validator("readTemperature_1", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("readTemperature_2", true));
        state("readTemperature_2", true)->addTransition(&m_hardware, &Hardware::Access::emit_getFlowControllerTemperature, validator("readTemperature_2", true));
            // Validate
            validator("readTemperature_2", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, state("startFlowControllerMonitor", true));
            validator("readTemperature_2", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("startFlowControllerMonitor", true));

        // Account for com issues
        transitionsBuilder()->stateComErrors(state("readTemperature_1", true), state("startFlowControllerMonitor", true));
        transitionsBuilder()->stateComErrors(state("readTemperature_2", true), state("startFlowControllerMonitor", true));
        transitionsBuilder()->stateComErrors(validator("readTemperature_1", true), state("startFlowControllerMonitor", true));
        transitionsBuilder()->stateComErrors(validator("readTemperature_2", true), state("startFlowControllerMonitor", true));
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







