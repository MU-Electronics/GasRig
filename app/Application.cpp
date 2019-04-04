#include "Application.h"

// External libs
#include <QQmlContext>
#include <QQmlEngine>
#include <QThread>
#include <QString>

// For debugging only to be removed


namespace App
{
    /**
     * Class constructure performs:
     *      - Set QObject
     *      - Sets the engine member variable
     *      - Create instant of the settings container to be past to other objects
     *      - Registers threads
     *      - Starts threads
     *      - Registers the main view manager which then registers sub managers
     *      - Connects the threads signals and slots together
     *      - For development only runs a debugging methods (to be removed before first release)
     *
     * @brief Application::Application
     * @param parent
     * @param engine
     */
    Application::Application(QObject* parent, QQmlApplicationEngine* engine)
        :   QObject(parent)
        ,   m_engine(engine)

            // debugging instance


           // Create instance of the settings container
        ,  settings_container(*new Settings::Container(parent))

            // Start objects that are to be threaded
        ,   monitor(*new Safety::Monitor(this, &settings_container))
        ,   hardware(*new Hardware::Access(this, &settings_container))

            // Include the expeirment engine
        ,    experiment_engine(*new Experiment::Engine(this, &settings_container, hardware, monitor))

            // Create instance for each view manager
        ,   manager_global(*new View::Managers::Global(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_testing(*new View::Managers::Testing(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_connection(*new View::Managers::ConnectionStatus(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_systemStatus(*new View::Managers::SystemStatus(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_machineStatus(*new View::Managers::MachineStatus(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_mode_pressuriseCell(*new View::Managers::Modes::PressuriseCell(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_graph_pressureVsTime(*new View::Managers::Graphs::PressureVsTime(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_graph_valvesVsTime(*new View::Managers::Graphs::ValvesVsTime(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_graph_vacuumVsTime(*new View::Managers::Graphs::VacuumVsTime(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_scripts_editor(*new View::Managers::Scripts::Editor(parent, m_engine, &settings_container, experiment_engine))
        ,   manager_scripts_add(*new View::Managers::Scripts::Add(parent, m_engine, &settings_container, experiment_engine))

    {
        // Register qml types with qml
        registerQmlTypes();

        // Load all managers
        registerManagers();

        // Connect view managers to threads
        connectViewToThreads();

        // Connect threads to one another
        connectThreads();

        // Register experiment engine threads
        connectEngineToThreads();

        // Register addtion threads
        registerAddtionalThreads();

        // Start addtional application threads
        startAddtionalThread();
    }


    /**
     * Class destructor performs:
     *    - Stops all running threads
     *
     * @brief Application::~Application
     */
    Application::~Application()
    {
        // Stop the hardware thread
        hardware.abort();
        thread_hardware.wait();
        //qDebug() << "Deleting hardware thread in thread " << this->QObject::thread()->currentThreadId();

        // Stop the safety thread
        monitor.abort();
        thread_safety.wait();
        //qDebug() << "Deleting safety thread in thread: " << this->QObject::thread()->currentThreadId();
    }


    /**
     * Register all the view manager instances
     *
     * @brief Application::registerManagers
     */
    void Application::registerManagers()
    {
        // Set global manger
        m_engine->rootContext()->setContextProperty("GlobalManager", &manager_global);

        // Set testing manager
        m_engine->rootContext()->setContextProperty("TestingManager", &manager_testing);

        // Set connection status manager
        m_engine->rootContext()->setContextProperty("ConnectionStatusManager", &manager_connection);

        // Set system status manager
        m_engine->rootContext()->setContextProperty("SystemStatusManager", &manager_systemStatus);

        // Set machine status manger
        m_engine->rootContext()->setContextProperty("MachineStatusManager", &manager_machineStatus);

        // Set mode pressurise cell manager
        m_engine->rootContext()->setContextProperty("PressuriseCellManager", &manager_mode_pressuriseCell);

        // Set pressure vs time graph manager
        m_engine->rootContext()->setContextProperty("PressuriseVsTimeGraph", &manager_graph_pressureVsTime);

        // Set valves vs time graph manager
        m_engine->rootContext()->setContextProperty("ValvesVsTimeGraph", &manager_graph_valvesVsTime);

        // Set valves vs time graph manager
        m_engine->rootContext()->setContextProperty("VacuumVsTimeGraph", &manager_graph_vacuumVsTime);

        // Set scripts
        m_engine->rootContext()->setContextProperty("ScriptEditorManager", &manager_scripts_editor);
        m_engine->rootContext()->setContextProperty("ScriptAddManager", &manager_scripts_add);



    }



    /**
     * Register qml types
     *
     * @brief Application::registerQmlTypes
     */
    void Application::registerQmlTypes()
    {
        // Rig diagram qml type
        //qmlRegisterType<App::View::QmlTypes::RigDiagram>("App", 1, 0, "RigDiagram");
       // qRegisterMetaType<QMap<qint64, QMap<QString, QMap<QString, QVariant>>>*>("QMap<qint64, QMap<QString, QMap<QString, QVariant>>>*");
    }


    /**
     * Setups up any additional threads to be ran but does not start then
     *
     * @brief Application::registerAddtionalThreads
     */
    void Application::registerAddtionalThreads()
    {
        // Hardware Thread
        hardware.setup(thread_hardware);
        hardware.moveToThread(&thread_hardware);

        // Safety Thread
        monitor.setup(thread_safety);
        monitor.moveToThread(&thread_safety);
    }


    /**
     * Loads the hardware and safety monitor thread
     *
     * @brief Application::startAddtionalThread
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void Application::startAddtionalThread()
    {
        // Double check no instance is running and start thread for hardware
        hardware.abort();
        thread_hardware.wait();
        hardware.requestWork();

        // Double check no instance is running and start thread for safety
        monitor.abort();
        thread_safety.wait();
        monitor.requestWork();
    }


    /**
     * Connect the view managers to threads
     *
     * @brief Application::connectViewToThreads
     */
    void Application::connectViewToThreads()
    {
        // Make connections for global view manager
        manager_global.makeConnections(hardware, monitor);

        // Make connections for testing view manager
        manager_testing.makeConnections(hardware, monitor);

        // Make connections for system settings view manager
        manager_systemStatus.makeConnections(hardware, monitor);

        // Make connections for connection status view manager
        manager_connection.makeConnections(hardware, monitor);

        // Make connections for machine status view manager
        manager_machineStatus.makeConnections(hardware, monitor);

        // Make connections for machine status view manager
        manager_mode_pressuriseCell.makeConnections(hardware, monitor);

        // Make connections for machine pressure vs time graph
        manager_graph_pressureVsTime.makeConnections(hardware, monitor);

        // Make connections for machine valves vs time graph
        manager_graph_valvesVsTime.makeConnections(hardware, monitor);

        // Make connections for machine vacuum vs time graph
        manager_graph_vacuumVsTime.makeConnections(hardware, monitor);

        // Script
        manager_scripts_editor.makeConnections(hardware, monitor);
        manager_scripts_add.makeConnections(hardware, monitor);
    }


    /**
     * Connects the experiment engine to the threads
     *
     * @brief Application::connectEngineToThreads
     */
    void Application::connectEngineToThreads()
    {
        experiment_engine.makeConnections();
    }


    /**
     * Connect the safety thread to IO thread via the Qt connect method
     *
     * @brief Application::connectThreads
     */
    void Application::connectThreads()
    {


    }
}
