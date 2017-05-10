#include "Access.h"

#include <QObject>
#include <QThread>
#include <QDebug>

namespace App { namespace Hardware
{
    Access::Access()
    {

    }

    Access::~Access()
    {

    }

    void Access::setup(QThread &thread)
    {
        //m_thread = &thread;
        qDebug("connect");
        connect(&thread, SIGNAL(started()), this, SLOT(runner()));
    }

    void Access::runner()
    {
        qDebug("Running");
        for(int i = 0; i < 100; i++)
        {
            qDebug() << "B: " <<i;
        }
    }

}}
