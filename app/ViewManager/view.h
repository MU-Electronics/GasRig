#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <map>
#include <memory>
#include <string>
#include "Manager.h"


namespace App { namespace ViewManager
{
    class View:  public QObject
    {
        public:
            View(QObject* parent, QQmlApplicationEngine& engine);

            std::map<std::string, std::shared_ptr<App::ViewManager::Manager>> m_Manager;

        private:
            QQmlApplicationEngine& m_engine;
            bool m_safety;
            bool m_IO;

            void loadManagers();
            void connectManagerToView();
            void connectViewToModel();
            void connectSafetyToIO();

            //explicit View(const View& rhs) = delete;
            //View& operator= (const View& rhs) = delete;
    };
}}


