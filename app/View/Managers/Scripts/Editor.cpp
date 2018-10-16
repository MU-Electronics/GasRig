#include "Editor.h"

// Include external libs
#include <QMap>

namespace App { namespace View { namespace Managers { namespace Scripts
{
    Editor::Editor(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Global::makeConnections
     * @param hardware
     * @param safety
     */
    void Editor::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Disable unused compiler warnings
        UNUSED(hardware);
        UNUSED(safety);
    }


}}}}

