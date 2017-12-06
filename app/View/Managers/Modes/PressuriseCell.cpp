#include "PressuriseCell.h"

// Include external libs
#include <QMap>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../../Experiment/Engine.h"


namespace App { namespace View { namespace Managers { namespace Modes
{
    PressuriseCell::PressuriseCell(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {

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

    }


}}}}
