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

// Include machine container
#include "../../Experiment/Machines/Machines.h"

namespace App { namespace View { namespace Managers
{

    class MachineStatus : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        AUTO_PROPERTY(int, stateMachineRunning)
        Q_PROPERTY(QVariantMap vacDownMachine READ vacDownMachine NOTIFY emit_vacDownMachineChanged)
        Q_PROPERTY(QVariantMap pulseValveMachine READ pulseValveMachine NOTIFY emit_pulseValveMachineChanged)
        Q_PROPERTY(QVariantMap pressuriseMachine READ pressuriseMachine NOTIFY emit_pressuriseMachineChanged)
        Q_PROPERTY(QVariantMap continiousPressuriseMachine READ continiousPressuriseMachine NOTIFY emit_continiousPressuriseMachineChanged)
        Q_PROPERTY(QVariantMap ventMachine READ ventMachine NOTIFY emit_ventMachineChanged)
        Q_PROPERTY(QVariantMap purgeMachine READ purgeMachine NOTIFY emit_purgeMachineChanged)
        Q_PROPERTY(QVariantMap safetyValveMachine READ safetyValveMachine NOTIFY emit_safetyValveMachineChanged)
        Q_PROPERTY(QVariantMap controllable READ controllable NOTIFY emit_controllableChanged)

        public:
            MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            void setControllable(QString id, int level);

            // Return the data for the state machines
            QVariantMap vacDownMachine() const { return m_vacDownMachine; }
            QVariantMap pulseValveMachine() const { return m_pulseValveMachine; }
            QVariantMap pressuriseMachine() const { return m_pressuriseMachine; }
            QVariantMap continiousPressuriseMachine() const { return m_continiousPressuriseMachine; }
            QVariantMap ventMachine() const { return m_ventMachine; }
            QVariantMap purgeMachine() const { return m_purgeMachine; }
            QVariantMap safetyValveMachine() const { return m_safetyValveMachine; }
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
            void emit_safetyValveMachineChanged(QVariantMap);
            void emit_continiousPressuriseMachineChanged(QVariantMap);

        public slots:
            void ventStarted(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);
            void ventStopping(QVariantMap params);
            void ventStopped(QVariantMap params);

            void vacDownStarted(double value, int valueType, bool turbo, int gasMode, int mode);
            void vacDownStopping(QVariantMap params);
            void vacDownStopped(QVariantMap params);

            void pulseValveStarted(int valve, int cycles, int timeOpen, int timeClosed);
            void pulseValveStopping(QVariantMap params);
            void pulseValveStopped(QVariantMap params);

            void pressuriseStarted(double pressure, bool initVacDown, int stepSize, bool inputValve);
            void pressuriseStopping(QVariantMap params);
            void pressuriseStopped(QVariantMap params);

            void continuousPressuriseStarted(int maxTime, int monitorTime, double topUp, double leak, double pressure, int stepSize, bool inputValve, bool outputValve, bool exhuastValveOnly);
            void continuousPressuriseStopping(QVariantMap params);
            void continuousPressuriseStopped(QVariantMap params);

            void purgeStarted(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo);
            void purgeStopping(QVariantMap params);
            void purgeStopped(QVariantMap params);

            void safetyValveStarted(int id, bool state);
            void safetyValveStopping(QVariantMap params);
            void safetyValveStopped(QVariantMap params);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;

            // Holds the vac down machine statuses
            QVariantMap m_vacDownMachine;

            // Holds the pulse valve machine statuses
            QVariantMap m_pulseValveMachine;

            // Holds the pressurise machine statuses
            QVariantMap m_pressuriseMachine;

            // Holds the continious pressurise machine statuses
            QVariantMap m_continiousPressuriseMachine;

            // Holds the vent machine statuses
            QVariantMap m_ventMachine;

            // Holds the purge machine status
            QVariantMap m_purgeMachine;

            // Holds the saftye valve state machine
            QVariantMap m_safetyValveMachine;

            // Holds the purge machine status
            QVariantMap m_controllable;

    };
}}}


