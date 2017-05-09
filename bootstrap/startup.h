#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

//class QQuickItem;
class QQmlApplicationEngine;

namespace Bootstrap{

    class Startup :  public QObject
    {

    Q_OBJECT

    public:
        Startup();
        ~Startup();
        bool success() const { return this->m_isValid; }

    private:
        QQmlApplicationEngine& m_engine;
        bool m_isValid;

        explicit Startup(const Startup& rhs) = delete;
        Startup& operator= (const Startup& rhs) = delete;
    };
}


