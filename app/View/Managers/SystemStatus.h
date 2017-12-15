#pragma once

// Include external libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QVariantMap>
#include <QVariant>
#include <QtGlobal>

#include <QtCharts/QAbstractSeries>
QT_CHARTS_USE_NAMESPACE

// Include contract
#include "Manager.h"

// Include app deps
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include command constructor
#include "../../Hardware/CommandConstructor.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

namespace App { namespace View { namespace Managers
{


    class SystemStatus : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        // Valve statuses
        Q_PROPERTY(QVariantMap valveState READ valveState NOTIFY emit_valveStateChanged)

        // Vacuum statuses
        Q_PROPERTY(QVariantMap vacuumState READ vacuumState NOTIFY emit_vacuumStateChanged)

        // Flow controller statuses
        Q_PROPERTY(QVariantMap flowControllerState READ flowControllerState NOTIFY emit_flowControllerStateChanged)

        // Flow controller statuses
        Q_PROPERTY(QVariantMap pressureSensor READ pressureSensor NOTIFY emit_pressureSensorChanged)

        // Debug messages
        Q_PROPERTY(QString debugMessages READ debugMessages NOTIFY emit_debugMessagesChanged)

        AUTO_PROPERTY(QString, logLocation)

        public:
            // constructure and destructor
            SystemStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            // Return the data for the valve statuses
            QVariantMap valveState() const { return m_valveState; }

            // Return the data for the vacuum statuses
            QVariantMap vacuumState() const { return m_vacuumState; }

            // Return the data for the flow controller statuses
            QVariantMap flowControllerState() const { return m_flowController; }

            // Return the data for the pressure sensor statuses
            QVariantMap pressureSensor() const { return m_pressureSensor; }

            // Return the data for the debug messages
            QString debugMessages();

        signals:
            void emit_hardwareRequest(QVariantMap command);

            void emit_valveStateChanged(QVariantMap);
            void emit_vacuumStateChanged(QVariantMap);
            void emit_flowControllerStateChanged(QVariantMap);
            void emit_pressureSensorChanged(QVariantMap);
            void emit_debugMessagesChanged(QList<QMap<QString, QString>>);

        public slots:
            void pressureGraphUpdate(QAbstractSeries *series);
            void pressureGraphData(QVariantMap package);

            // Listen for debugger
            void logChanged(QMap<QString, QString> message);

            // Listen for labjack events
            void receiveValveStatus(QVariantMap package);

            // Listen for pressure sensor events
            void receivePressureSensorPressure(QVariantMap package);

            // Listen for flow controller events
            void receiveSetFlowControllerValveOverride(QVariantMap command);
            void receiveSetFlowControllerFlowRate(QVariantMap command);
            void receiveSetFlowControllerSoftStart(QVariantMap command);
            void receiveSetFlowControllerSoftStartTime(QVariantMap command);
            void receiveSetFlowControllerSourceControl(QVariantMap command);
            void receiveFlowControllerTemperature(QVariantMap command);
            void receiveFlowControllerFlowRate(QVariantMap command);

            // Listen for vacuum events
            void receiveVacSetPump(QVariantMap command);
            void receiveVacSetTurbo(QVariantMap command);
            void receiveVacSetGasMode(QVariantMap command);
            void receiveVacSetPumpMode(QVariantMap command);
            void receiveVacuumPressure(QVariantMap command);
            void receiveVacGetTurboSpeed(QVariantMap command);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;

            // Hold instance of command constructor
            Hardware::CommandConstructor m_commandConstructor;

            // Holds the valve statuses for all the valves
            QVariantMap m_valveState;

            // Holds the vacuum statuses for all values
            QVariantMap m_vacuumState;

            // Holds the flow controller statuses for all values
            QVariantMap m_flowController;

            // Holds the pressure sensor statuses for all values
            QVariantMap m_pressureSensor;

            // Holds errors that have occured
            QList<QMap<QString, QString>> m_debugMessages;

            // Has initial commands been sent to devices
            QMap<QString, bool> initalCommands;

            void setInitialValues(QVariantMap package);


            QVector<QPointF> m_data;
                int m_index;
    };
}}}


