#include "Application.h"

// External libs
#include <QQmlContext>
#include <QQmlEngine>
#include <QThread>
#include <QString>

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
        ,   experiment_engine(*new Experiment::Engine(this, &settings_container, hardware, monitor))

            // View manager factory
        ,   manager_factory(*new View::ManagerFactory())

    {
        // Register qml types with qml
        registerQmlTypes();

        // Create managers
        createManagers();

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
     * Create instance for each view manager
     *
     * @brief Application::createManagers
     */
    void Application::createManagers()
    {
        manager_factory.create<View::Managers::Global>("Global", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::Testing>("Testing", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::ConnectionStatus>("ConnectionStatus", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::SystemStatus>("SystemStatus", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::MachineStatus>("MachineStatus", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::Modes::PressuriseCell>("Modes::PressuriseCell", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::Graphs::PressureVsTime>("Graphs::PressureVsTime", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::Graphs::ValvesVsTime>("Graphs::ValvesVsTime", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::Graphs::VacuumVsTime>("Graphs::VacuumVsTime", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::Scripts::Editor>("Scripts::Editor", this, m_engine, &settings_container, &experiment_engine);
        manager_factory.create<View::Managers::Scripts::Add>("Scripts::Add", this, m_engine, &settings_container, &experiment_engine);
    }


    /**
     * Register all the view manager instances
     *
     * @brief Application::registerManagers
     */
    void Application::registerManagers()
    {
        // Set global manger
        m_engine->rootContext()->setContextProperty("GlobalManager", manager_factory.get<View::Managers::Global>("Global"));

        // Set testing manager
        m_engine->rootContext()->setContextProperty("TestingManager", manager_factory.get<View::Managers::Testing>("Testing"));

        // Set connection status manager
        m_engine->rootContext()->setContextProperty("ConnectionStatusManager", manager_factory.get<View::Managers::ConnectionStatus>("ConnectionStatus"));

        // Set system status manager
        m_engine->rootContext()->setContextProperty("SystemStatusManager", manager_factory.get<View::Managers::SystemStatus>("SystemStatus"));

        // Set machine status manger
        m_engine->rootContext()->setContextProperty("MachineStatusManager", manager_factory.get<View::Managers::MachineStatus>("MachineStatus"));

        // Set mode pressurise cell manager
        m_engine->rootContext()->setContextProperty("PressuriseCellManager", manager_factory.get<View::Managers::Modes::PressuriseCell>("Modes::PressuriseCell"));

        // Set pressure vs time graph manager
        m_engine->rootContext()->setContextProperty("PressuriseVsTimeGraph", manager_factory.get<View::Managers::Graphs::PressureVsTime>("Graphs::PressureVsTime"));

        // Set valves vs time graph manager
        m_engine->rootContext()->setContextProperty("ValvesVsTimeGraph", manager_factory.get<View::Managers::Graphs::ValvesVsTime>("Graphs::ValvesVsTime"));

        // Set valves vs time graph manager
        m_engine->rootContext()->setContextProperty("VacuumVsTimeGraph", manager_factory.get<View::Managers::Graphs::VacuumVsTime>("Graphs::VacuumVsTime"));

        // Set scripts
        m_engine->rootContext()->setContextProperty("ScriptEditorManager", manager_factory.get<View::Managers::Scripts::Editor>("Scripts::Editor"));
        m_engine->rootContext()->setContextProperty("ScriptAddManager", manager_factory.get<View::Managers::Scripts::Add>("Scripts::Add"));
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
        manager_factory.get<View::Managers::Global>("Global")->makeConnections(hardware, monitor);

        // Make connections for testing view manager
        manager_factory.get<View::Managers::Testing>("Testing")->makeConnections(hardware, monitor);

        // Make connections for system settings view manager
        manager_factory.get<View::Managers::SystemStatus>("SystemStatus")->makeConnections(hardware, monitor);

        // Make connections for connection status view manager
        manager_factory.get<View::Managers::ConnectionStatus>("ConnectionStatus")->makeConnections(hardware, monitor);

        // Make connections for machine status view manager
        manager_factory.get<View::Managers::MachineStatus>("MachineStatus")->makeConnections(hardware, monitor);

        // Make connections for machine status view manager
        manager_factory.get<View::Managers::Modes::PressuriseCell>("Modes::PressuriseCell")->makeConnections(hardware, monitor);

        // Make connections for machine pressure vs time graph
        manager_factory.get<View::Managers::Graphs::PressureVsTime>("Graphs::PressureVsTime")->makeConnections(hardware, monitor);

        // Make connections for machine valves vs time graph
        manager_factory.get<View::Managers::Graphs::ValvesVsTime>("Graphs::ValvesVsTime")->makeConnections(hardware, monitor);

        // Make connections for machine vacuum vs time graph
        manager_factory.get<View::Managers::Graphs::VacuumVsTime>("Graphs::VacuumVsTime")->makeConnections(hardware, monitor);

        // Script
        manager_factory.get<View::Managers::Scripts::Editor>("Scripts::Editor")->makeConnections(hardware, monitor);
        manager_factory.get<View::Managers::Scripts::Add>("Scripts::Add")->makeConnections(hardware, monitor);
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
