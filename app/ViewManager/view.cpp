#include "View.h"

// External libs
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <map>
#include <memory>

// include view manangers
#include "Testing.h"
#include "ConnectionStatus.h"


namespace App { namespace ViewManager
{
    View::View(QObject* parent, QQmlApplicationEngine& engine)
        : QObject(parent),
          m_engine(engine),
          m_safety(true),
          m_IO(true)
    {
        // Load all managers
        loadManagers();

        // Connect all managers
        connectManagerToView();
        connectViewToModel();

        // connect IO thread and Safety thread
        connectSafetyToIO();
    }

    /**
     * Register all the view manager instances
     *
     * @brief View::loadManagers
     */
    void View::loadManagers()
    {
        // Setup the connection status view manager
        m_Manager["connection_status"] = std::shared_ptr<App::ViewManager::Manager>(new App::ViewManager::ConnectionStatus(this, &m_engine));

        // Setup the testing view manager
        m_Manager["testing"] = std::shared_ptr<App::ViewManager::Manager>(new App::ViewManager::Testing(this, &m_engine));
    }


    /**
     * Connect the view managers to views via the Qt connect method
     *
     * @brief View::connectManagerToView
     */
    void View::connectManagerToView()
    {
        auto& rootContext = m_engine.rootContext();
        for (auto& manager : m_Manager)
        {
            rootContext.setContextProperty(manager.first, &manager.second);
        }
    }


    /**
     * Connect the views managers to models via the Qt connect method
     *
     * @brief View::connectViewToModel
     */
    void View::connectViewToModel()
    {

    }


    /**
     * Connect the safety thread to IO thread via the Qt connect method
     *
     * @brief View::connectSafetyToIO
     */
    void View::connectSafetyToIO()
    {

    }
}}
