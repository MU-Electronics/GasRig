#include "CommandValidatorState.h"

#include <QDebug>
#include <QStateMachine>
#include <QState>
#include <QVariant>
#include <QEvent>

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    /**
     * Subclass QState so we can overload the onEntry method
     *
     * @brief CommandValidatorState::CommandValidatorState
     * @param machine
     */
    CommandValidatorState::CommandValidatorState(QStateMachine *machine)
        :   QState(machine)
    {

    }


    /**
     * Passes the command data from the HALs to the validation state
     *
     * @brief CommandValidatorState::onEntry
     * @param e
     */
    void CommandValidatorState::onEntry(QEvent* e)
    {
        // If the signal is from the state machine
        if (e->type() == QEvent::StateMachineSignal)
        {
            // Get the signal
            QStateMachine::SignalEvent* se = dynamic_cast<QStateMachine::SignalEvent*>(e);

            // Check cast
            if(se == NULL)
            {
                // Create error package
                package.insert("cast_status", false);

                // Return no more action can be taken
                return;
            }

            // Created package
            package.insert("cast_status", true);

            if (se->arguments().size() > 0)
            {
                // Get the value of the argument
                QVariant arg = se->arguments().at(0);

                // Check we can conver the argument to a QVarientMap (standard type for all HAL emits)
                if (arg.canConvert<QVariantMap>())
                {
                    // Save the data to a local container to be retrieved by the state
                    package = arg.toMap();
                }
            }
        }
    }

}}}}
