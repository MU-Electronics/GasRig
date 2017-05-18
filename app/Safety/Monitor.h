#pragma once

// include external libs
#include <QObject>
#include <QThread>
#include <QMutex>

// include thread
#include "../Services/Thread.h"

// Include settings container
#include "../Settings/container.h"

namespace App { namespace Safety
{
    class Monitor: public App::Services::Thread
    {
        Q_OBJECT
        public:
            Monitor(QObject *parent, App::Settings::Container settings);
            ~Monitor();

        private:
            Settings::Container m_settings;

            void configure();
            void worker();
    };
}}


