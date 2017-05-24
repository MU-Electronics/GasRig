#include "ConnectionStatus.h"
#include <iostream>
#include <QMap>

// Include settings container
#include "../Settings/container.h"
// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"



namespace App { namespace ViewManager
{
    ConnectionStatus::ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        // USB connection start out as not working
        m_hardwareConnection.insert("VacStation", "0");
        m_hardwareConnection.insert("PressureSensor", "0");
        m_hardwareConnection.insert("LabJack", "0");
        m_hardwareConnection.insert("FlowControllerOne", "0");
        m_hardwareConnection.insert("FlowControllerTwo", "0");
        m_hardwareConnection.insert("SystemCondition", "0");
        m_hardwareConnection.insert("SafetyMonitor", "0");
        m_hardwareConnection.insert("Supplies", "0");


        // Request USB status for each device so we can update the status cache here

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

    }




    /**
     * List for com port opening successfully or not
     *
     * @brief ConnectionStatus::listen_comConnectionStatus
     * @param package
     */
    void ConnectionStatus::listen_comConnectionStatus(QVariantMap package)
    {
        // Connection successfully
        if(package.value("open").toBool())
        {
            m_hardwareConnection.insert(package["responsability"].toString(), "1");
            return;
        }

        // Connection faile
        m_hardwareConnection.insert(package["responsability"].toString(), "0");
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
    }

}}

