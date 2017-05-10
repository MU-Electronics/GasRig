#include "Monitor.h"

#include <QThread>

namespace App { namespace Safety
{


    Monitor::Monitor(QObject *parent)
    {

    }

    Monitor::~Monitor()
    {


    }

    void Monitor::setup(QThread &thread)
    {
        //m_thread = &thread;
        connect(&thread, SIGNAL(started()), this, SLOT(start()));
    }

    void Monitor::start()
    {

    }

}}
