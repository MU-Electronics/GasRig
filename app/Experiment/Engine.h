#pragma once

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Include state machine
#include "Machines/VacDown.h"

namespace App { namespace Experiment
{
    class Engine    :   public QObject
    {
        Q_OBJECT

        public:
            Engine(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety);

            ~Engine();

            void makeConnections();

            void VacDown(int mintues, bool turbo, int gasMode, int mode);

        private:
            // Holds the application settings
            Settings::Container m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the safety monitor
            Safety::Monitor &m_safety;

            // Vac down machine
            Machines::VacDown& m_vacDown;

    };
}}


