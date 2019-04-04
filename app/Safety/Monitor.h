#pragma once

// include external libs
#include <QObject>
#include <QThread>
#include <QMutex>

// include thread
#include "../Services/Thread.h"

// Include settings container
#include "../Settings/Container.h"

namespace App { namespace Safety
{
    class Monitor: public App::Services::Thread
    {
        Q_OBJECT
        public:
            Monitor(QObject *parent, Settings::Container *settings);
            ~Monitor();

        private:
            Settings::Container m_settings;

            void configure(QThread &thread);
            void worker();
    };
}}


