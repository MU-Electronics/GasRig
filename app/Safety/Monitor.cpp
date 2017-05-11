#include "Monitor.h"

#include <QObject>
#include <QThread>
#include <QDebug>

namespace App { namespace Safety
{
    Monitor::Monitor()
    {

    }

    Monitor::~Monitor()
    {

    }

    void Monitor::setup(QThread &thread)
    {
        qDebug("connect");
        connect(&thread, SIGNAL(started()), this, SLOT(runner()));
    }

    void Monitor::runner()
    {
        qDebug("Running");
        for(int i = 0; i < 2; i++)
        {
            qDebug() << "A: " <<i;
        }
    }

}}
