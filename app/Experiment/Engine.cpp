#include "Engine.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace Experiment
{
    Engine::Engine(QObject *parent, Settings::Container settings)
        :   QObject(parent),
            m_settings(settings)
    {

    }

    Engine::~Engine(){}

    /**
     * Make connections to other parts of the application
     *
     * @brief Engine::makeConnections
     */
    void Engine::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {

    }
}}
