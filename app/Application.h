#pragma once

// External libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QMap>

// Settings container
#include "Settings/container.h"

// View managers
#include "ViewManager/Testing.h"
#include "ViewManager/ConnectionStatus.h"
#include "ViewManager/SystemStatus.h"

// Objects to the threaded
#include "Safety/Monitor.h"
#include "Hardware/Access.h"

// Include the expeirment engine
#include "Experiment/Engine.h"

namespace App
{
    class Application:  public QObject
    {
        Q_OBJECT

        public:
            Application(QObject* parent, QQmlApplicationEngine *engine);
            ~Application();

            void connectViewToManager();

        private:
            QQmlApplicationEngine* m_engine;

            // Settings container
            Settings::Container& settings_container;

            // Addtional threads
            QThread thread_safety;
            QThread thread_hardware;

            // Objects to be moved to the threads
            Safety::Monitor& monitor;
            Hardware::Access& hardware;

            // Hold the instance of expeirment engine
            Experiment::Engine& experiment_engine;

            // View managers
            ViewManager::Testing& manager_testing;
            ViewManager::ConnectionStatus& manager_connection;
            ViewManager::SystemStatus& manager_systemStatus;

            void registerManagers();
            void registerAddtionalThreads();
            void startAddtionalThread();
            void connectThreads();
            void connectViewToThreads();
            void connectEngineToThreads();

            void debug();


            //explicit Application(const Application& rhs) = delete;
            //Application& operator= (const Application& rhs) = delete;
    };
}


