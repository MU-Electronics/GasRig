#include "Application.h"

// External libs
#include <QQmlApplicationEngine>
#include <QObject>
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

// For debugging only to be removed
#include "Settings/Safety.h"

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
        // Register addtion threads
        registerAddtionalThreads();

        // Start addtional application threads
        startAddtionalThread();  

        // Load all managers
        registerManagers();

        // Connect threads to one another
        connectThreads();

        // Debugging area for testing
        debug();
    }


    Application::~Application()
    {
        // Stop the hardware thread
        hardware.abort();
        thread_hardware.wait();
        qDebug() << "Deleting hardware thread in thread " << this->QObject::thread()->currentThreadId();

        // Stop the safety thread
        monitor.abort();
        thread_safety.wait();
        qDebug() << "Deleting safety thread in thread: " << this->QObject::thread()->currentThreadId();
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

        Settings::Safety safety_settings;
        safety_settings.load(":/config/Safety.json", safety_settings.SaveFormat::Json);

    }
}
