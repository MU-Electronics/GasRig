#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include "Manager.h"
#include <QString>
#include "../Utilities/Properties.h"

// Include settings container
#include "../Settings/container.h"

namespace App { namespace ViewManager
{
    class ConnectionStatus : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        AUTO_PROPERTY (QString, exampleVar)

        public:
            ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);
            ~ConnectionStatus();

            void helloWorld();
        signals:

        public slots:

        private:
            QQmlApplicationEngine* m_root;

            Settings::Container m_settings;

            //explicit ConnectionStatus(const ConnectionStatus& rhs) = delete;
            //ConnectionStatus& operator= (const ConnectionStatus& rhs) = delete;
    };
}}


