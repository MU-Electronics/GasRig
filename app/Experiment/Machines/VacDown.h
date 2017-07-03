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
    class VacDown    :   public MachineStates
    {

        public:
            VacDown(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~VacDown();

            void setParams(int mintues, bool turbo, int gasMode, int mode);

            void start();

            void stop();

            void buildMachine();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

    };
}}}


