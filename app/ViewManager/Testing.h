#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>

#include "Manager.h"
#include <QString>
#include "../Utilities/Properties.h"

// Include settings container
#include "../Settings/container.h"

namespace App { namespace ViewManager
{
    class Testing : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        AUTO_PROPERTY (QString, exampleVar)

        public:
            Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);
            void makeConnections();
        signals:

        public slots:
            void requestVacuum(bool onOff);

        private:
            QQmlApplicationEngine* m_root;

            Settings::Container m_settings;

            //explicit Testing(const Testing& rhs) = delete;
            //Testing& operator= (const Testing& rhs) = delete;
    };
}}


