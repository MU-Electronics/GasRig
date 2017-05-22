#pragma once

// Include external libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QVariantMap>
#include <QVariant>
#include <QtGlobal>

// Include contract
#include "Manager.h"

// Include app deps
#include "../Utilities/Properties.h"

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace ViewManager
{


    class SystemStatus : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        Q_PROPERTY(QVariantMap valves READ valves)
        Q_PROPERTY(QList<qreal> pressure READ pressure)

        public:
            // constructure and destructor
            SystemStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);

            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Getters
            QVariantMap valves() const { return m_valves; }
            QList<qreal> pressure() const { return m_pressure; }

        signals:

        public slots:


        private:
            QQmlApplicationEngine* m_root;

            Settings::Container m_settings;

            // system status cache variables
            QVariantMap m_valves;
            QList<qreal> m_pressure;
    };
}}


