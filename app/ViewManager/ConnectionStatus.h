#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include "Manager.h"
#include <QString>
#include <QMap>
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
        Q_PROPERTY(QVariantMap hardwareConnection READ hardwareConnection NOTIFY hardwareConnectionChanged)

        public:
            ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Return the data for the connection statuses
            QVariantMap hardwareConnection() const { return m_hardwareConnection; }



        signals:
            void hardwareConnectionChanged(QVariantMap);

        public slots:
            void listen_comConnectionStatus(QVariantMap package);
            void listen_critialSerialError(QVariantMap package);
            void listen_timeoutSerialError(QVariantMap package);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Holds the connection statuses for all the hardware
            QVariantMap m_hardwareConnection;
    };
}}


