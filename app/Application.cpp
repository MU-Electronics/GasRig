#include "Application.h"

// External libs
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

// include view manangers
#include "ViewManager/Testing.h"
#include "ViewManager/ConnectionStatus.h"


namespace App
{
    Application::Application(QObject* parent, QQmlApplicationEngine* engine)
        : QObject(parent),
          m_engine(engine),

          // Create instacne for each addtions thread
          thread_safety(true),
          thread_IO(true),

          // Create instance for each view manager
          manager_testing(*new ViewManager::Testing(parent, engine)),
          manager_connection(*new ViewManager::ConnectionStatus(parent, engine))
    {
        // Start addtional application threads
        startAddtionalThread();

        // Load all managers
        registerManagers();

        // Connect views and managers together
        connectViewToManager();

        // Connect threads to one another
        connectThreads();
    }


    Application::~Application()
    {

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
    }


    /**
     * Loads the IO thread to control the gas rig
     *
     * @brief Application::loadIOThread
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Application::startAddtionalThread()
    {
        // IO Thread

        // Safety Thread

    }



    /**
     * Connect the views managers to models via the Qt connect method
     *
     * @brief View::connectViewToModel
     */
    void Application::connectViewToManager()
    {

    }


    /**
     * Connect the safety thread to IO thread via the Qt connect method
     *
     * @brief View::connectSafetyToIO
     */
    void Application::connectThreads()
    {

    }
}
