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
        // Start the thread running when thread is ready
        connect(&thread, SIGNAL(started()), this, SLOT(runner()));


    }

    void Access::setupHAL()
    {

    }

    void Access::runner()
    {
        // Setup the hardware layers
        setupHAL();

        // Monitor the devices
        while(true)
        {
            // Monitor vac station prams


            // Monitor pressure sensor prams

            // Monotor labjack prams

            // Monitor flow controller prams
        }
    }

}}
