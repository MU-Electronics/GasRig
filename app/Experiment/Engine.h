#pragma once

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace Experiment
{
    class Engine    :   public QObject
    {
        Q_OBJECT

        public:
            Engine(QObject *parent, Settings::Container settings);

            void makeConnections(Hardware::Access &hardware, Safety::Monitor &safety);

        private:
            // Holds the application settings
            Settings::Container m_settings;


    };
}}


