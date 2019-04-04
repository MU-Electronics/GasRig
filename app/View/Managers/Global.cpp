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

        // Rig settings
        auto rigSettings = m_settings.hardware.rig_config;
        m_rigSettings["valve_4"] = rigSettings["valve_4"];
        m_rigSettings["valve_6"] = rigSettings["valve_6"];
        m_rigSettings["flow_controller_1"] = rigSettings["flow_controller_1"];
        m_rigSettings["flow_controller_2"] = rigSettings["flow_controller_2"];
        m_rigSettings["valve_8"] = false;

        if(m_rigSettings["flow_controller_1"].toBool() == true || m_rigSettings["flow_controller_2"] == true)
            m_rigSettings["valve_8"] = true;
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

