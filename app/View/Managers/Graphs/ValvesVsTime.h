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


    class ValvesVsTime : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        AUTO_PROPERTY(double, graphMaxY)
        AUTO_PROPERTY(double, graphMinY)
        AUTO_PROPERTY(QDateTime, graphMaxX)
        AUTO_PROPERTY(QDateTime, graphMinX)
        READONLY_PROPERTY(bool, hasData)

        public:
            // constructure and destructor
            ValvesVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:
            void emit_newValveOneGraphData();
            void emit_newValveTwoGraphData();
            void emit_newValveThreeGraphData();
            void emit_newValveFourGraphData();
            void emit_newValveFiveGraphData();
            void emit_newValveSixGraphData();
            void emit_newValveSevenGraphData();
            void emit_newValveEightGraphData();
            void emit_newValveNineGraphData();

        public slots:
            void update(int valve, QAbstractSeries *series);

            void valveData(QVariantMap package);


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;        

            // Saves the graph data
            QVector<QPointF> m_valveOneData;
            QVector<QPointF> m_valveTwoData;
            QVector<QPointF> m_valveThreeData;
            QVector<QPointF> m_valveFourData;
            QVector<QPointF> m_valveFiveData;
            QVector<QPointF> m_valveSixData;
            QVector<QPointF> m_valveSevenData;
            QVector<QPointF> m_valveEightData;
            QVector<QPointF> m_valveNineData;

            // Scroll by
            int scrollBy = 20;
            int hold = 300;

            // Save last place QML updated
            int currentValveOneId = 0;
            int currentValveTwoId = 0;
            int currentValveThreeId = 0;
            int currentValveFourId = 0;
            int currentValveFiveId = 0;
            int currentValveSixId = 0;
            int currentValveSevenId = 0;
            int currentValveEightId = 0;
            int currentValveNineId = 0;

            // Limit the buffer size
            int maxBuffer = 4000;

            // Update axis
            void updateGraph(QAbstractSeries *series, int &currentId, QVector<QPointF> &data);

            void updateTimedData(qreal value, QVector<QPointF> &store);
    };
}}}}


