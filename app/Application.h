#pragma once

// External libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QMap>

// View managers
#include "ViewManager/Testing.h"
#include "ViewManager/ConnectionStatus.h"
#include "ViewManager/SystemStatus.h"

// Objects to the threaded
#include "Safety/Monitor.h"
#include "Hardware/access.h"

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
            ViewManager::SystemStatus& manager_systemStatus;

            void connectViewToManager();

        private:
            QQmlApplicationEngine* m_engine;

            // Addtional threads
            QThread thread_safety;
            QThread thread_hardware;

            // Objects to be moved to the threads
            Safety::Monitor& monitor;
            Hardware::Access& hardware;


            void registerManagers();
            void registerAddtionalThreads();
            void startAddtionalThread();

            void connectThreads();


            //explicit Application(const Application& rhs) = delete;
            //Application& operator= (const Application& rhs) = delete;
    };
}


