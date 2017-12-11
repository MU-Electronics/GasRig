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

        AUTO_PROPERTY(int, stateMachineRunning)
        Q_PROPERTY(QVariantMap vacDownMachine READ vacDownMachine NOTIFY emit_vacDownMachineChanged)
        Q_PROPERTY(QVariantMap pulseValveMachine READ pulseValveMachine NOTIFY emit_pulseValveMachineChanged)
        Q_PROPERTY(QVariantMap pressuriseMachine READ pressuriseMachine NOTIFY emit_pressuriseMachineChanged)
        Q_PROPERTY(QVariantMap ventMachine READ ventMachine NOTIFY emit_ventMachineChanged)
        Q_PROPERTY(QVariantMap purgeMachine READ purgeMachine NOTIFY emit_purgeMachineChanged)
        Q_PROPERTY(QVariantMap controllable READ controllable NOTIFY emit_controllableChanged)

        public:
            MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            void setControllable(QString id, int level);

            // Return the data for the state machines
            QVariantMap vacDownMachine() const { return m_vacDownMachine; }
            QVariantMap pulseValveMachine() const { return m_pulseValveMachine; }
            QVariantMap pressuriseMachine() const { return m_pressuriseMachine; }
            QVariantMap ventMachine() const { return m_ventMachine; }
            QVariantMap purgeMachine() const { return m_purgeMachine; }
            QVariantMap controllable() const { return m_controllable; }

        signals:
            void emit_hardwareAccess(QVariantMap command);

            // Signals for properties
            void emit_vacDownMachineChanged(QVariantMap);
            void emit_pulseValveMachineChanged(QVariantMap);
            void emit_pressuriseMachineChanged(QVariantMap);
            void emit_ventMachineChanged(QVariantMap);
            void emit_purgeMachineChanged(QVariantMap);
            void emit_controllableChanged(QVariantMap);

        public slots:
            void ventStarted(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);
            void ventStopping();
            void ventStopped();

            void vacDownStarted(double value, int valueType, bool turbo, int gasMode, int mode);
            void vacDownStopping();
            void vacDownStopped();

            void pulseValveStarted(int valve, int cycles, int timeOpen, int timeClosed);
            void pulseValveStopping();
            void pulseValveStopped();

            void pressuriseStarted(double pressure, bool initVacDown, int stepSize, bool inputValve);
            void pressuriseStopping();
            void pressuriseStopped();

            void purgeStarted(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo);
            void purgeStopping();
            void purgeStopped();

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

            // Holds the purge machine status
            QVariantMap m_purgeMachine;

            // Holds the purge machine status
            QVariantMap m_controllable;

    };
}}}


