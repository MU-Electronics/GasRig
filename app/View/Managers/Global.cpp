#include "Global.h"

// Include external libs
#include <QMap>

namespace App { namespace View { namespace Managers
{
    Global::Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        // Disable unused compiler warnings
        UNUSED(experimentEngine);
    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Global::makeConnections
     * @param hardware
     * @param safety
     */
    void Global::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Disable unused compiler warnings
        UNUSED(hardware);
    }


}}}

