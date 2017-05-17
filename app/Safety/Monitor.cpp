#include "Monitor.h"

#include <QObject>
#include <QThread>
#include <QDebug>

namespace App { namespace Safety
{    

    /**
     * Class constructure performs:
     *      - Set parent thread class
     *
     * @brief Monitor::Monitor
     * @param parent
     */
    Monitor::Monitor(QObject *parent)
        : Thread(parent, false, false)
    {

    }

    /**
     * Class detructure performs:
     *      - Nothing atm
     *
     * @brief Monitor::~Monitor
     */
    Monitor::~Monitor()
    {

    }

    /**
     * The thread worker that will be in a forever loop untill stopped
     * via the thread_abort parent member variable
     *
     * @brief Monitor::worker
     */
    void Monitor::worker()
    {
        //qDebug() << "Safety thread";

        thread()->sleep(10);
    }

}}
