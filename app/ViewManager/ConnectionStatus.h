#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include "Manager.h"
#include <QString>
#include <QMap>
#include <QVariantMap>
#include "../Utilities/Properties.h"

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace ViewManager
{

    class ConnectionStatus : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        // USB connection statuses
        Q_PROPERTY(QVariantMap hardwareConnection READ hardwareConnection NOTIFY emit_hardwareConnectionChanged)

        // Valve statuses
        Q_PROPERTY(QVariantMap valveState READ valveState NOTIFY emit_valveStateChanged)


        public:
            ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Return the data for the connection statuses
            QVariantMap hardwareConnection() const { return m_hardwareConnection; }

            // Return the data for the valve statuses
            QVariantMap valveState() const { return m_valveState; }

        signals:
            void emit_hardwareConnectionChanged(QVariantMap);
            void emit_valveStateChanged(QVariantMap);

            void emit_hardwareAccess(QVariantMap command);

        public slots:
            void listen_comConnectionStatus(QVariantMap package);
            void listen_critialSerialError(QVariantMap package);
            void listen_timeoutSerialError(QVariantMap package);

            void request_reconnect(QString item);


            void receiveValveStatus(QVariantMap package);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Holds the connection statuses for all the hardware
            QVariantMap m_hardwareConnection;

            // Holds the valve statuses for all the valves
            QVariantMap m_valveState;
    };
}}


