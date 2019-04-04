#pragma once

// Include external libs
#include <QObject>
#include <QQmlApplicationEngine>
#include <QString>
#include <QVariantMap>
#include <QVariant>
#include <QtGlobal>
#include <QDateTime>
#include <QString>

// Include contract
#include "../Manager.h"

// Include app deps
#include "../../../Utilities/Properties.h"

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../../Experiment/Engine.h"

// Experiment Engine with state machine methods
#include "Graph.h"

namespace App { namespace View { namespace Managers { namespace Graphs
{


    class VacuumVsTime : public Graph, public App::View::Managers::Manager
    {
        Q_OBJECT
        public:
            // constructure and destructor
            VacuumVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine &experimentEngine);
            ~VacuumVsTime(){}

            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:
            void emit_newVacuumGraphData();

        public slots:
            void update(QAbstractSeries *series);

            void data(QVariantMap package);

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;        

            // Saves the graph data
            QVector<QPointF> m_vacuumData;

            // Save last place QML updated
            int currentVacuumId = 0;
    };
}}}}


