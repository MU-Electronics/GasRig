#include "Add.h"

// Include external libs
#include <QMap>

namespace App { namespace View { namespace Managers { namespace Scripts
{
    Add::Add(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Add::makeConnections
     * @param hardware
     * @param safety
     */
    void Add::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Disable unused compiler warnings
        UNUSED(hardware);
        UNUSED(safety);
    }


}}}}

