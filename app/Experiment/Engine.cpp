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


    /**
     * Start a new vac down state machine running
     *
     * @brief Engine::VacDown
     * @param mintues
     * @param turbo
     * @param gasMode
     * @param mode
     */
    void Engine::vacDown(int mintues, bool turbo, int gasMode, int mode)
    {
        // Set the params
        m_vacDown.setParams(mintues, turbo, gasMode, mode);

        // Build the machine
        m_vacDown.buildMachine();

        // Start the machine
        m_vacDown.start();
    }

    /**
     * Stops a running instance of vac down state machine
     *
     * @brief Engine::StopVacDown
     */
    void Engine::stopVacDown()
    {
        m_vacDown.stop();
    }


}}
