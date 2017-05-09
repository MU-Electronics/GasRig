#include "View.h"

// External libs
#include <QQmlApplicationEngine>
#include <map>

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

    }

    void View::loadManagers()
    {
        // Setup the connection status view manager
        m_Manager["testing"] = new App::ViewManager::ConnectionStatus(this, &m_engine);

        // Setup the testing view manager
        m_Manager["testing"] = new App::ViewManager::Testing(this, &m_engine);
    }

    void View::connectManagerToView()
    {

    }

    void View::connectViewToModel()
    {

    }

    void View::connectSafetyToIO()
    {

    }
}}
