#include "Application.h"

// External libs
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QThread>
#include <QString>

// Include view manangers
#include "ViewManager/Testing.h"
#include "ViewManager/ConnectionStatus.h"
#include "ViewManager/SystemStatus.h"

// Include objects for threading
#include "Safety/Monitor.h"
#include "Hardware/access.h"

namespace App
{
    Application::Application(QObject* parent, QQmlApplicationEngine* engine)
        : QObject(parent),
          m_engine(engine),

          // Start objects that are to be threaded
          monitor(*new Safety::Monitor()),
          hardware(*new Hardware::Access()),

          // Create instance for each view manager
          manager_testing(*new ViewManager::Testing(parent, engine)),
          manager_connection(*new ViewManager::ConnectionStatus(parent, engine)),
          manager_systemStatus(*new ViewManager::SystemStatus(parent, engine))
    {
        // Start addtional application threads
        startAddtionalThread();

        // Load all managers
        registerManagers();

        // Connect threads to one another
        connectThreads();
    }


    Application::~Application()
    {
        // Shut down the threads
        thread_hardware.terminate();
        thread_safety.terminate();
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
     * Loads the hardware and safety monitor thread
     *
     * @brief Application::startAddtionalThread
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Application::startAddtionalThread()
    {
        // Hardware Thread
        hardware.setup(thread_hardware);
        hardware.moveToThread(&thread_hardware);
        thread_hardware.start();

        // Safety Thread
        monitor.setup(thread_safety);
        monitor.moveToThread(&thread_safety);
        thread_safety.start();
    }


    /**
     * Connect the safety thread to IO thread via the Qt connect method
     *
     * @brief Application::connectThreads
     */
    void Application::connectThreads()
    {

    }
}
