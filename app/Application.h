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

            // View managers
            ViewManager::Testing& manager_testing;
            ViewManager::ConnectionStatus& manager_connection;

        private:
            QQmlApplicationEngine* m_engine;

            // Addtional threads
            bool thread_safety;
            bool thread_IO;


            void registerManagers();
            void startAddtionalThread();
            void connectViewToManager();
            void connectThreads();


            //explicit Application(const Application& rhs) = delete;
            //Application& operator= (const Application& rhs) = delete;
    };
}


