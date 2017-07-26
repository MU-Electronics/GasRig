#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class ReadFlowControllerFlow    :   public MachineStates
    {
        Q_OBJECT

        public:
            ReadFlowControllerFlow(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~ReadFlowControllerFlow();

            void setParams(int flowControllerTimeInter);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_readFlowControllerFlowStopped(QVariantMap params);
            void emit_readFlowControllerFlowFailed(QVariantMap params);

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

    };
}}}


