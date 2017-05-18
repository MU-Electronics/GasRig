#include "Access.h"

// Include external libs
#include <QObject>
#include <QThread>
#include <QDebug>

// Include settings container
#include "../Settings/container.h"

namespace App { namespace Hardware
{
    /**
     * Class constructure performs:
     *      - Set parent thread class
     *      - Set settings member variable
     *
     * @brief Monitor::Monitor
     * @param parent
     */
    Access::Access(QObject *parent, Settings::Container settings)
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
    Access::~Access()
    {

    }


    /**
     * Allows for any configurations to be ran before the worker is set going
     *
     * @brief Access::configure
     */
    void Access::configure()
    {
        qDebug() << "Hardware thread child setup method ran";
    }


    /**
     * The thread worker that will be in a forever loop untill stopped
     * via the thread_abort parent member variable
     *
     * @brief Monitor::worker
     */
    void Access::worker()
    {
        //qDebug() << "Hardware thread";

        thread()->sleep(1);
    }

}}
