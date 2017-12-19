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
#include "Functions/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class SafeValve    :   public Functions::MachineStates
    {
        Q_OBJECT

        public:
            SafeValve(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~SafeValve();

            void setParams(int id, bool state);

            void beforeStart();

            void stopped();

            void buildMachine();

        signals:

        private:
            // Referance to QObject
            QObject *parent;


            // Holds the application settings
            Settings::Container m_settings;


            // Valve state machines
            void valveOne(bool logic);
            void valveTwo(bool logic);
            void valveThree(bool logic);
            void valveFour(bool logic);
            void valveFive(bool logic);
            void valveSix(bool logic);
            void valveSeven(bool logic);
            void valveEight(bool logic);
            void valveNine(bool logic);

    };
}}}


