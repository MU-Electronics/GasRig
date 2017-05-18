#include "Access.h"

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
