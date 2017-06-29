#include "Engine.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Include state machine
#include "Machines/VacDown.h"

namespace App { namespace Experiment
{
    Engine::Engine(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   QObject(parent),
            m_settings(settings),
            m_hardware(hardware),
            m_safety(safety)
        ,   m_vacDown(*new Machines::VacDown(parent, settings, hardware, safety))
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

    void Engine::VacDown(int mintues, bool turbo, int gasMode)
    {
        // Set the params
        m_vacDown.setParams(mintues, turbo, gasMode);

        // Build the machine
        m_vacDown.buildMachine();

        // Start the machine
        m_vacDown.start();
    }
}}
