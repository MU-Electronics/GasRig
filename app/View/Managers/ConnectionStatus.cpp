#include "ConnectionStatus.h"

// Include external libs
#include <QMap>
#include <QLoggingCategory>
#include "../../../bootstrap/LoggingCategory.h"


namespace App { namespace View { namespace Managers
{
    ConnectionStatus::ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        // USB connection start out as not working
        m_hardwareConnection.insert("VacStation", "0");
        m_hardwareConnection.insert("PressureSensor", "0");
        m_hardwareConnection.insert("LabJack", "0");
        m_hardwareConnection.insert("FlowController", "0");
//        m_hardwareConnection.insert("Supplies", "0");
//        m_hardwareConnection.insert("SystemCondition", "0");
//        m_hardwareConnection.insert("SafetyMonitor", "0");
//        m_hardwareConnection.insert("HardwareGateway", "0");

        // All connects ok
        m_hardwareConnection.insert("all_connections", false);
    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief ConnectionStatus::makeConnections
     * @param hardware
     * @param safety
     */
    void ConnectionStatus::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Listen for com port connection signals
        connect(&hardware, &Hardware::Access::emit_serialComUpdated, this, &ConnectionStatus::listen_comConnectionStatus);
        connect(&hardware, &Hardware::Access::emit_timeoutSerialError, this, &ConnectionStatus::listen_timeoutSerialError);
        connect(&hardware, &Hardware::Access::emit_critialSerialError, this, &ConnectionStatus::listen_critialSerialError);

        // Listen for power supply signals
        // Listen for safety monitor signals
        // Listen for system safe conition

        // Requests for hardware reconnects
        connect(this, &ConnectionStatus::emit_hardwareAccess, &hardware, &Hardware::Access::hardwareAccess);
    }



    void ConnectionStatus::allConnections()
    {
        if( m_hardwareConnection.value("VacStation").toInt() == 1 &&
            m_hardwareConnection.value("PressureSensor").toInt() == 1  &&
            m_hardwareConnection.value("LabJack").toInt() == 1  &&
            m_hardwareConnection.value("FlowController").toInt() == 1)
        {
            m_hardwareConnection.insert("all_connections", true);
            return;
        }

        // @todo change to false, true for debugging
        m_hardwareConnection.insert("all_connections", true);
    }

    void ConnectionStatus::request_reconnect(QString item)
    {
        // Update GUI with please wait
        m_hardwareConnection.insert(item, "4");
        emit_hardwareConnectionChanged(m_hardwareConnection);

        // Command package
        QVariantMap command;

        // Supples is part of labjack with MAY be a standard implimentation of SerialController
        if(item == "Supplies")
        {

        }
        // Safety monitor is another thread
        else if (item == "SafetyMonitor")
        {

        }
        // Flow controller custom implimentation of SerialController
        else if (item == "FlowController")
        {
            command["hardware"] = "FlowController";
            command["controller"] = "FlowControllerOne";
            command["method"] = "resetConnection";
        }
        // The rest are using standard implimentation of SerialController
        else
        {
            command["hardware"] = item;
            command["method"] = "resetConnection";                   
        }

        // Send the command to the hardware
        emit emit_hardwareAccess(command);
    }





    /**
     * List for com port opening successfully or not
     *
     * @brief ConnectionStatus::listen_comConnectionStatus
     * @param package
     */
    void ConnectionStatus::listen_comConnectionStatus(QVariantMap package)
    {
        // Connection success
        m_hardwareConnection.insert(package["responsability"].toString(), "1");

        // Connection successfully
        if(!package.value("status").toBool())
        {
            // Set failed
            m_hardwareConnection.insert(package["responsability"].toString(), "0");

            // Log failed event
            qCWarning(usbConnectionStatus) << "Failed to connect to com port!" << package;
        }

        // Update summary
        allConnections();

        // Update GUI
        emit_hardwareConnectionChanged(m_hardwareConnection);
    }


    /**
     * Listen for serial communication timeout events on any com port
     *
     * @brief ConnectionStatus::listen_timeoutSerialError
     * @param package
     */
    void ConnectionStatus::listen_timeoutSerialError(QVariantMap package)
    {
        // Set connection to timeout error
        m_hardwareConnection.insert(package["responsability"].toString(), "2");

        // Update summary
        allConnections();

        // Update GUI
        emit_hardwareConnectionChanged(m_hardwareConnection);
    }


    /**
     * Listen for any critial serial communication errors on any com port
     *
     * @brief ConnectionStatus::listen_critialSerialError
     * @param package
     */
    void ConnectionStatus::listen_critialSerialError(QVariantMap package)
    {
        // Set connection to critial error
        m_hardwareConnection.insert(package["responsability"].toString(), "3");

        // Update summary
        allConnections();

        // Update GUI
        emit_hardwareConnectionChanged(m_hardwareConnection);
    }



}}}

