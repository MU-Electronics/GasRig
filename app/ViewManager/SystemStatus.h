#pragma once

// Include external libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QVariantMap>
#include <QVariant>

// Include contract
#include "Manager.h"

// Include app deps
#include "../Utilities/Properties.h"

namespace App { namespace ViewManager
{


    class SystemStatus : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        Q_PROPERTY(QVariantMap valves READ valves)


        public:
            // constructure and destructor
            SystemStatus(QObject *parent, QQmlApplicationEngine *root);
            ~SystemStatus();

            // Getters
            QVariantMap valves() const { return m_valves; }

        signals:

        public slots:


        private:
            QQmlApplicationEngine* m_root;

            // system status cache variables
            QVariantMap m_valves;
    };
}}


