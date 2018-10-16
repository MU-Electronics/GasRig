#include "PressuriseCell.h"

// Include external libs
#include <QMap>


namespace App { namespace View { namespace Managers { namespace Modes
{
    PressuriseCell::PressuriseCell(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
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
     * @brief PressuriseCell::makeConnections
     * @param hardware
     * @param safety
     */
    void PressuriseCell::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Disable unused compiler warnings
        UNUSED(hardware);
        UNUSED(safety);
    }


}}}}

