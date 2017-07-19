#include "Engine.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Include state machines
#include "Machines/Machines.h"

namespace App { namespace Experiment
{
    Engine::Engine(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   QObject(parent)
        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machines(*new Machines::Machines(parent, settings, hardware, safety))
    {

    }

    Engine::~Engine(){}


    /**
     * Make connections to other parts of the application
     *
     * @brief Engine::makeConnections
     */
    void Engine::makeConnections()
    {

    }


    /**
     * Returns the container of machines
     *
     * @brief Engine::machines
     * @return
     */
    Machines::Machines& Engine::machines()
    {
        return m_machines;
    }


}}
