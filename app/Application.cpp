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
          m_safety(true),
          m_IO(true),
          manager_testing(*new ViewManager::Testing(parent, engine)),
          manager_connection(*new ViewManager::ConnectionStatus(parent, engine))
    {
        // Load all managers
        registerManagers();

        // Connect all managers
        connectViewToModel();

        // connect IO thread and Safety thread
        connectSafetyToIO();
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
     * Connect the views managers to models via the Qt connect method
     *
     * @brief View::connectViewToModel
     */
    void Application::connectViewToModel()
    {

    }


    /**
     * Connect the safety thread to IO thread via the Qt connect method
     *
     * @brief View::connectSafetyToIO
     */
    void Application::connectSafetyToIO()
    {

    }
}
