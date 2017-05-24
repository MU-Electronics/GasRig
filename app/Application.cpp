#include "Application.h"

// External libs
#include <QQmlApplicationEngine>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QThread>
#include <QString>
#include <QDebug>

// Include view manangers
#include "ViewManager/Testing.h"
#include "ViewManager/ConnectionStatus.h"
#include "ViewManager/SystemStatus.h"

// Include objects for threading
#include "Safety/Monitor.h"
#include "Hardware/Access.h"

// Include the setting contain
#include "Settings/container.h"

// For debugging only to be removed


namespace App
{
    /**
     * Class constructure performs:
     *      - Set QObject
     *      - Sets the engine member variable
     *      - Create instant of the settings container to be past to other objects
     *      - Registers threads
     *      - Starts threads
     *      - Registers the main view manager which then registers sub managers
     *      - Connects the threads signals and slots together
     *      - For development only runs a debugging methods (to be removed before first release)
     *
     * @brief Application::Application
     * @param parent
     * @param engine
     */
    Application::Application(QObject* parent, QQmlApplicationEngine* engine)
        : QObject(parent),
          m_engine(engine),

          // Create instance of the settings container
          settings_container(*new Settings::Container),

          // Start objects that are to be threaded
          monitor(*new Safety::Monitor(this, settings_container)),
          hardware(*new Hardware::Access(this, settings_container)),

          // Create instance for each view manager
          manager_testing(*new ViewManager::Testing(parent, engine, settings_container)),
          manager_connection(*new ViewManager::ConnectionStatus(parent, engine, settings_container)),
          manager_systemStatus(*new ViewManager::SystemStatus(parent, engine, settings_container))
    {
        // Load all managers
        registerManagers();

        // Connect view managers to threads
        connectViewToThreads();

        // Connect threads to one another
        connectThreads();

        // Register addtion threads
        registerAddtionalThreads();

        // Start addtional application threads
        startAddtionalThread();

        // Debugging area for testing
        debug();
    }


    /**
     * Class destructor performs:
     *    - Stops all running threads
     *
     * @brief Application::~Application
     */
    Application::~Application()
    {
        // Stop the hardware thread
        hardware.abort();
        thread_hardware.wait();
        //qDebug() << "Deleting hardware thread in thread " << this->QObject::thread()->currentThreadId();

        // Stop the safety thread
        monitor.abort();
        thread_safety.wait();
        //qDebug() << "Deleting safety thread in thread: " << this->QObject::thread()->currentThreadId();
    }


    /**
     * Register all the view manager instances
     *
     * @brief View::loadManagers
     */
    void Application::registerManagers()
    {
        // Set testing manager
        m_engine->rootContext()->setContextProperty("TestingManager", &manager_testing);

        // Set connection status manager
        m_engine->rootContext()->setContextProperty("ConnectionStatusManager", &manager_connection);

        // Set system status manager
        m_engine->rootContext()->setContextProperty("SystemStatusManager", &manager_systemStatus);
    }


    /**
     * Setups up any additional threads to be ran but does not start then
     *
     * @brief Application::registerAddtionalThreads
     */
    void Application::registerAddtionalThreads()
    {
        // Hardware Thread
        hardware.setup(thread_hardware);
        hardware.moveToThread(&thread_hardware);

        // Safety Thread
        monitor.setup(thread_safety);
        monitor.moveToThread(&thread_safety);
    }


    /**
     * Loads the hardware and safety monitor thread
     *
     * @brief Application::startAddtionalThread
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Application::startAddtionalThread()
    {
        // Double check no instance is running and start thread for hardware
        hardware.abort();
        thread_hardware.wait();
        hardware.requestWork();

        // Double check no instance is running and start thread for safety
        monitor.abort();
        thread_safety.wait();
        monitor.requestWork();
    }


    /**
     * Connect the view managers to threads
     *
     * @brief Application::connectViewToThreads
     */
    void Application::connectViewToThreads()
    {
        // Make connections for testing view manager
        manager_testing.makeConnections(hardware, monitor);

        // Make connections for system settings view manager
        manager_systemStatus.makeConnections(hardware, monitor);

        // Make connections for connection status view manager
        manager_connection.makeConnections(hardware, monitor);

    }

    /**
     * Connect the safety thread to IO thread via the Qt connect method
     *
     * @brief Application::connectThreads
     */
    void Application::connectThreads()
    {


    }

    /**
     * A simple area that allows classes etc to be tested before used
     * NOTE: Will be remove before first release
     *
     * @brief debug
     */
    void Application::debug()
    {

    }
}
