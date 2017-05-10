#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <map>
#include <memory>
#include <string>
#include <QString>
#include "Manager.h"


#include "Testing.h"
#include "ConnectionStatus.h"

namespace App { namespace ViewManager
{
    class View:  public QObject
    {
        Q_OBJECT
        public:
            View(QObject* parent, QQmlApplicationEngine *engine);
            ~View();

            Testing& manager_testing;
            ConnectionStatus& manager_connection;

        private:
            QQmlApplicationEngine* m_engine;
            bool m_safety;
            bool m_IO;

            void registerManagers();
            void connectManagerToView();
            void connectViewToModel();
            void connectSafetyToIO();

            //explicit View(const View& rhs) = delete;
            //View& operator= (const View& rhs) = delete;
    };
}}


