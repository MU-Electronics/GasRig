#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>

// Include interface
#include "Manager.h"

// Include macros
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

namespace App { namespace View { namespace Managers
{

    class MachineStatus : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        // Vac down state machine status
        Q_PROPERTY(QVariantMap vacDownMachine READ vacDownMachine NOTIFY emit_vacDownMachineChanged)

        public:
            MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Return the data for the vac down state machine
            QVariantMap vacDownMachine() const { return m_vacDownMachine; }

        signals:
            void emit_hardwareAccess(QVariantMap command);

            // Signals for properties
            void emit_vacDownMachineChanged(QVariantMap);

        public slots:
            void vacDownStarted(int mintues, bool turbo, int gasMode, int mode);
            void vacDownStopped();


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;

            // Holds the vac down machine statuses
            QVariantMap m_vacDownMachine;

    };
}}}


