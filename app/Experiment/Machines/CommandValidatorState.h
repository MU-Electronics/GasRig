#pragma once

#include <QObject>
#include <QState>
#include <QStateMachine>
#include <QVariantMap>

namespace App { namespace Experiment { namespace Machines
{

    class CommandValidatorState: public QState
    {
        public:
            CommandValidatorState(QStateMachine *machine);

            QVariantMap package;

        private:
            void onEntry(QEvent* e);
    };

}}}

