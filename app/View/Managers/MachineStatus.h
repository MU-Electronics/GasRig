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
        Q_PROPERTY(QVariantMap pulseValveMachine READ pulseValveMachine NOTIFY emit_pulseValveMachineChanged)
        Q_PROPERTY(QVariantMap pressuriseMachine READ pressuriseMachine NOTIFY emit_pressuriseMachineChanged)
        Q_PROPERTY(QVariantMap ventMachine READ ventMachine NOTIFY emit_ventMachineChanged)

        public:
            MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Return the data for the state machines
            QVariantMap vacDownMachine() const { return m_vacDownMachine; }
            QVariantMap pulseValveMachine() const { return m_pulseValveMachine; }
            QVariantMap pressuriseMachine() const { return m_pressuriseMachine; }
            QVariantMap ventMachine() const { return m_ventMachine; }

        signals:
            void emit_hardwareAccess(QVariantMap command);

            // Signals for properties
            void emit_vacDownMachineChanged(QVariantMap);
            void emit_pulseValveMachineChanged(QVariantMap);
            void emit_pressuriseMachineChanged(QVariantMap);
            void emit_ventMachineChanged(QVariantMap);

        public slots:
            void ventStarted(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);
            void ventStopped();

            void vacDownStarted(int mintues, bool turbo, int gasMode, int mode);
            void vacDownStopped();

            void pulseValveStarted(int valve, int cycles, int timeOpen, int timeClosed);
            void pulseValveStopped();

            void pressuriseStarted(double pressure, bool initVacDown, int stepSize, bool inputValve);
            void pressuriseStopped();


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;

            // Holds the vac down machine statuses
            QVariantMap m_vacDownMachine;

            // Holds the pulse valve machine statuses
            QVariantMap m_pulseValveMachine;

            // Holds the pressurise machine statuses
            QVariantMap m_pressuriseMachine;

            // Holds the vent machine statuses
            QVariantMap m_ventMachine;

    };
}}}


