#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class SafeValve    :   public MachineStates
    {
        Q_OBJECT

        public:
            SafeValve(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~SafeValve();

            void setParams(int id, bool state);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_safeValveFinished(QVariantMap params);
            void emit_safeValveFailed(QVariantMap params);

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

            // Valve state machines
            void valveOne(bool state);
            void valveTwo(bool state);
            void valveThree(bool state);
            void valveFour(bool state);
            void valveFive(bool state);
            void valveSix(bool state);
            void valveSeven(bool state);
            void valveEight(bool state);
            void valveNine(bool state);

    };
}}}


