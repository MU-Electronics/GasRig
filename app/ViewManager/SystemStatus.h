#pragma once

// Include external libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QVariantMap>
#include <QVariant>
#include <QtGlobal>

// Include contract
#include "Manager.h"

// Include app deps
#include "../Utilities/Properties.h"

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace ViewManager
{


    class SystemStatus : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT

        // Valve statuses
        Q_PROPERTY(QVariantMap valveState READ valveState NOTIFY emit_valveStateChanged)

        // Vacuum statuses
        Q_PROPERTY(QVariantMap vacuumState READ vacuumState NOTIFY emit_vacuumStateChanged)

        // Flow controller statuses
        Q_PROPERTY(QVariantMap flowControllerState READ flowControllerState NOTIFY emit_flowControllerStateChanged)

        public:
            // constructure and destructor
            SystemStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);

            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Return the data for the valve statuses
            QVariantMap valveState() const { return m_valveState; }

            // Return the data for the vacuum statuses
            QVariantMap vacuumState() const { return m_vacuumState; }

            // Return the data for the flow controller statuses
            QVariantMap flowControllerState() const { return m_flowController; }

        signals:
            void emit_valveStateChanged(QVariantMap);
            void emit_vacuumStateChanged(QVariantMap);
            void emit_flowControllerStateChanged(QVariantMap);

        public slots:
            // Listen for labjack events
            void receiveValveStatus(QVariantMap package);

            // Listen for flow controller events
            void receiveSetFlowControllerValveOverride(QVariantMap command);
            void receiveSetFlowControllerFlowRate(QVariantMap command);
            void receiveSetFlowControllerSoftStart(QVariantMap command);
            void receiveSetFlowControllerSoftStartTime(QVariantMap command);
            void receiveSetFlowControllerSourceControl(QVariantMap command);

            // Listen for vacuum events
            void receiveVacSetPump(QVariantMap command);
            void receiveVacSetTurbo(QVariantMap command);
            void receiveVacSetGasMode(QVariantMap command);
            void receiveVacSetPumpMode(QVariantMap command);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Holds the valve statuses for all the valves
            QVariantMap m_valveState;

            // Holds the vacuum statuses for all values
            QVariantMap m_vacuumState;

            // Holds the flow controller statuses for all values
            QVariantMap m_flowController;

    };
}}


