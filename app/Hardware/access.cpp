#include "Access.h"

#include <QObject>
#include <QThread>
#include <QDebug>

namespace App { namespace Hardware
{
    Access::Access(QObject *parent)
        : Thread(parent, false, false)
    {

    }

    Access::~Access()
    {

    }

    void Access::worker()
    {
        qDebug() << "Hardware thread";

        thread()->sleep(8);
    }

}}
