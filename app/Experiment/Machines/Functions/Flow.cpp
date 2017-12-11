#include "Flow.h"

#include <QString>

// Include functions helper
#include "FunctionsHelper.h"

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    Flow::Flow(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)

            // States relating to controlling the flow controller
        ,   sm_flowControllerOneFlow(&machine)
        ,   sm_flowControllerTwoFlow(&machine)

            // States relating to validating the vac station
        ,   sm_validateFlowControllerOneFlow(&machine)
        ,   sm_validateFlowControllerTwoFlow(&machine)

    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Flow::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    Flow::~Flow()
    {

    }

    void Flow::connectStatesToMethods()
    {

        // Link flow controller states
        connect(&sm_flowControllerOneFlow, &QState::entered, this, &Flow::flowControllerOneFlow);
        connect(&sm_flowControllerTwoFlow, &QState::entered, this, &Flow::flowControllerTwoFlow);

        // Link flow controller validation states
        connect(&sm_validateFlowControllerOneFlow, &CommandValidatorState::entered, this, &Flow::validateFlowControllerOneFlow);
        connect(&sm_validateFlowControllerTwoFlow, &CommandValidatorState::entered, this, &Flow::validateFlowControllerTwoFlow);
    }




    void Flow::flowControllerOneValveOverrideOpen()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerOne", 1));
    }


    void Flow::validateFlowControllerOneValveOverrideOpen()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check the returned valve override state
        if(package.value("override").toInt() == 1 && package.value("controller").toString() == "FlowControllerOne")
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
        }

        QVariantMap error;
        emit emit_validationFailed(error);
    }




    void Flow::flowControllerTwoValveOverrideOpen()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerTwo", 1));
    }

    void Flow::validateFlowControllerTwoValveOverrideOpen()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check the returned valve override state
        if(package.value("override").toInt() == 1 && package.value("controller").toString() == "FlowControllerTwo")
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
        }

        QVariantMap error;
        emit emit_validationFailed(error);
    }




    void Flow::flowControllerOneValveOverrideClose()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerOne", 2));
    }

    void Flow::validateFlowControllerOneValveOverrideClose()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check the returned valve override state
        if(package.value("override").toInt() == 2 && package.value("controller").toString() == "FlowControllerOne")
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
        }

        QVariantMap error;
        emit emit_validationFailed(error);
    }




    void Flow::flowControllerTwoValveOverrideClose()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerTwo", 2));
    }

    void Flow::validateFlowControllerTwoValveOverrideClose()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check the returned valve override state
        if(package.value("override").toInt() == 2 && package.value("controller").toString() == "FlowControllerTwo")
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
        }

        QVariantMap error;
        emit emit_validationFailed(error);
    }




    void Flow::flowControllerOneValveOverrideOff()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerOne", 0));
    }

    void Flow::validateFlowControllerOneValveOverrideOff()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check the returned valve override state
        if(package.value("override").toInt() == 0 && package.value("controller").toString() == "FlowControllerOne")
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
        }

        QVariantMap error;
        emit emit_validationFailed(error);
    }





    void Flow::flowControllerTwoValveOverrideOff()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerTwo", 0));
    }

    void Flow::validateFlowControllerTwoValveOverrideOff()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Check the returned valve override state
        if(package.value("override").toInt() == 0 && package.value("controller").toString() == "FlowControllerTwo")
        {
            QVariantMap success;
            emit emit_validationSuccess(success);
        }

        QVariantMap error;
        emit emit_validationFailed(error);
    }





    void Flow::flowControllerOneTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerTemperature("FlowControllerOne"));
    }


    void Flow::validateFlowControllerOneTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);
    }

    void Flow::flowControllerTwoTemperature()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerTemperature("FlowControllerTwo"));
    }


    void Flow::validateFlowControllerTwoTemperature()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);
    }


    void Flow::flowControllerOneFlow()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerFlowRate("FlowControllerOne"));
    }


    void Flow::validateFlowControllerOneFlow()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }

    void Flow::flowControllerTwoFlow()
    {
        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerFlowRate("FlowControllerTwo"));
    }


    void Flow::validateFlowControllerTwoFlow()
    {
        // Get the validator state instance
        CommandValidatorState* state = (CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        QVariantMap success;
        emit emit_validationSuccess(success);

        qDebug() << package;
    }




}}}}
