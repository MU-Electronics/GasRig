#pragma once

// include external libs
#include <QObject>
#include <QThread>
#include <QMutex>

// include thread
#include "../Services/Thread.h"

namespace App { namespace Safety
{
    class Monitor: public App::Services::Thread
    {
        Q_OBJECT
        public:
            Monitor(QObject *parent = 0);
            ~Monitor();

        private:
            void worker();
    };
}}


