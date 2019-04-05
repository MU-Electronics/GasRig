#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QMap>
#include <QVariantMap>
#include <QSerialPortInfo>


// View contract
#include "Manager.h"

// Macro for c++ => QML properties
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

namespace App { namespace View { namespace Managers
{

    class ConnectionStatus : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        // USB connection statuses
        Q_PROPERTY(QVariantMap hardwareConnection READ hardwareConnection NOTIFY emit_hardwareConnectionChanged)

        public:
            ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine *experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Return the data for the connection statuses
            QVariantMap hardwareConnection() const { return m_hardwareConnection; }

            // List all serial com ports on PC
            Q_INVOKABLE static QVariant availablePorts() {
                QList<QSerialPortInfo> portsAvailable = QSerialPortInfo::availablePorts();
                QStringList names_PortsAvailable;
                for(const QSerialPortInfo& portInfo : portsAvailable) {
                    names_PortsAvailable<<portInfo.portName();
                }
                return QVariant::fromValue(names_PortsAvailable);
            }

        signals:
            void emit_hardwareConnectionChanged(QVariantMap);

            void emit_hardwareAccess(QVariantMap command);

        public slots:
            // Listen for com statues
            void listen_comConnectionStatus(QVariantMap package);
            void listen_critialSerialError(QVariantMap package);
            void listen_timeoutSerialError(QVariantMap package);
            void request_reconnect(QString item);
            void allConnections();


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            // Holds the connection statuses for all the hardware
            QVariantMap m_hardwareConnection;

    };
}}}


