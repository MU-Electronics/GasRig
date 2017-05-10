#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include "Manager.h"
#include <QString>
#include "../Utilities/Properties.h"

namespace App { namespace ViewManager
{
    class Testing : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        AUTO_PROPERTY (QString, exampleVar)

        public:
            Testing(QObject *parent, QQmlApplicationEngine *root);
            void helloWorld();
        signals:

        public slots:

        private:
            QQmlApplicationEngine* m_root;

            //explicit Testing(const Testing& rhs) = delete;
            //Testing& operator= (const Testing& rhs) = delete;
    };
}}


