#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>

// Include interface
#include "Manager.h"

// Include macros
#include "../Utilities/Properties.h"

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../Experiment/Engine.h"

namespace App { namespace ViewManager
{

    class MachineStatus : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT

        public:
            MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:
            void emit_hardwareAccess(QVariantMap command);

        public slots:
            void vacDownStarted(int mintues, bool turbo, int gasMode, int mode);
            void vacDownStopped();


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;

    };
}}


