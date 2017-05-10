#pragma once

#include <QObject>
#include <QQmlApplicationEngine>


#include "ViewManager/Testing.h"
#include "ViewManager/ConnectionStatus.h"

namespace App
{
    class Application:  public QObject
    {
        Q_OBJECT

        public:
            Application(QObject* parent, QQmlApplicationEngine *engine);
            ~Application();

            ViewManager::Testing& manager_testing;
            ViewManager::ConnectionStatus& manager_connection;

        private:
            QQmlApplicationEngine* m_engine;
            bool m_safety;
            bool m_IO;

            void registerManagers();
            void connectManagerToView();
            void connectViewToModel();
            void connectSafetyToIO();

            //explicit Application(const Application& rhs) = delete;
            //Application& operator= (const Application& rhs) = delete;
    };
}


