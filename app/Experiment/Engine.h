#pragma once

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/Container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Include state machine
#include "Machines/Machines.h"

namespace App { namespace Experiment
{
    class Engine    :   public QObject
    {
        Q_OBJECT

        public:
            Engine(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety);

            ~Engine();

            void makeConnections();

            Machines::Machines &machines();

        private:
            // Holds the application settings
            Settings::Container m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the safety monitor
            Safety::Monitor &m_safety;

            // Machines container
            Machines::Machines& m_machines;

    };
}}


