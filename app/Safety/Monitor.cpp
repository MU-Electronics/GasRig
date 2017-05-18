#include "Monitor.h"

#include <QObject>
#include <QThread>
#include <QDebug>

#include "../Settings/container.h"

namespace App { namespace Safety
{    

    /**
     * Class constructure performs:
     *      - Set parent thread class
     *      - Set settings member variable
     *
     * @brief Monitor::Monitor
     * @param parent
     */
    Monitor::Monitor(QObject *parent, Settings::Container settings)
        : Thread(parent, false, false),
          m_settings(settings)
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
     * Allows for any configurations to be ran before the worker is set going
     *
     * @brief Monitor::configure
     */
    void Monitor::configure()
    {
        qDebug() << "Safety thread child setup method ran";
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
        //qDebug() << m_settings.safety.pressure["rise"];

        thread()->sleep(10);
    }

}}
