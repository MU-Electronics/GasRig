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

#include <QtCharts/QAbstractSeries>
QT_CHARTS_USE_NAMESPACE

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

namespace App { namespace View { namespace Managers { namespace Graphs
{


    class PressureVsTime : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        AUTO_PROPERTY(double, graphMaxY)
        AUTO_PROPERTY(double, graphMinY)
        AUTO_PROPERTY(QDateTime, graphMaxX)
        AUTO_PROPERTY(QDateTime, graphMinX)

        public:
            // constructure and destructor
            PressureVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:


        public slots:
            void updateGraph(QAbstractSeries *series);
            void data(QVariantMap package);


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;

            // Saves the graph data
            QVector<QPointF> m_data;

            // Scroll by
            int scrollBy = 20;
            int hold = 300;

            // Save last place QML updated
            int currentId = 0;

            // Limit the buffer size
            int maxBuffer = 4000;

            // Update axis
            void updateAxis();
    };
}}}}


