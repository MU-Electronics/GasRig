#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include "Manager.h"
#include <QString>
#include "../Utilities/Properties.h"

namespace App { namespace ViewManager
{
    class ConnectionStatus : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        AUTO_PROPERTY (QString, exampleVar)

        public:
            ConnectionStatus(QObject *parent, QQmlApplicationEngine *root);
            ~ConnectionStatus();

            void helloWorld();
        signals:

        public slots:

        private:
            QQmlApplicationEngine* m_root;

            //explicit ConnectionStatus(const ConnectionStatus& rhs) = delete;
            //ConnectionStatus& operator= (const ConnectionStatus& rhs) = delete;
    };
}}


