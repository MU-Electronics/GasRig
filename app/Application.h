#pragma once

// External libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QMap>

// Settings container
#include "Settings/Container.h"

// View managers
#include "View/Managers/Testing.h"
#include "View/Managers/ConnectionStatus.h"
#include "View/Managers/SystemStatus.h"
#include "View/Managers/MachineStatus.h"
#include "View/Managers/Global.h"
#include "View/Managers/Modes/PressuriseCell.h"
#include "View/Managers/Scripts/Editor.h"
#include "View/Managers/Scripts/Add.h"

// Include view graph managers
#include "View/Managers/Graphs/PressureVsTime.h"
#include "View/Managers/Graphs/ValvesVsTime.h"
#include "View/Managers/Graphs/VacuumVsTime.h"

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
            View::Managers::Global& manager_global;
            View::Managers::Testing& manager_testing;
            View::Managers::ConnectionStatus& manager_connection;
            View::Managers::SystemStatus& manager_systemStatus;
            View::Managers::MachineStatus& manager_machineStatus;
            View::Managers::Modes::PressuriseCell& manager_mode_pressuriseCell;
            View::Managers::Graphs::PressureVsTime& manager_graph_pressureVsTime;
            View::Managers::Graphs::ValvesVsTime& manager_graph_valvesVsTime;
            View::Managers::Graphs::VacuumVsTime& manager_graph_vacuumVsTime;
            View::Managers::Scripts::Editor& manager_scripts_editor;
            View::Managers::Scripts::Add& manager_scripts_add;

            void registerManagers();
            void registerQmlTypes();
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


