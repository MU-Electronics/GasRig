#include "Monitor.h"

#include <QObject>
#include <QThread>
#include <QDebug>

namespace App { namespace Safety
{    

    Monitor::Monitor(QObject *parent)
        : Thread(parent, false, false)
    {

    }

    Monitor::~Monitor()
    {

    }

    void Monitor::worker()
    {
        qDebug() << "Safety thread";

        thread()->sleep(10);
    }

}}
