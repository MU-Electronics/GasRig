#include "Flow.h"

#include <QString>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace States
{

    Flow::Flow(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)



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

    }


}}}}
