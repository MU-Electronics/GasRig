#include "ValvesVsTime.h"

#include <cmath>

#include <QVariantMap>
#include <QString>
#include <QtGlobal>
#include <QTimer>
#include <QDateTime>

#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../../Experiment/Engine.h"

namespace App { namespace View { namespace Managers { namespace Graphs
{
    ValvesVsTime::ValvesVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings),
            m_experimentEngine(experimentEngine)
    {
        qRegisterMetaType<QAbstractSeries*>();
        qRegisterMetaType<QAbstractAxis*>();
    }


    /**
     * Connect method to signals and vis-versa
     *
     * @brief ValvesVsTime::makeConnections
     * @param hardware
     * @param safety
     */
    void ValvesVsTime::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Pressure sensor
        connect(&hardware, &Hardware::Access::emit_setValveState, this, &ValvesVsTime::valveData);

        // Get the current date time
        QDateTime current = QDateTime::currentDateTime();

        // Append correct x axis limits
        graphMinX(current);
        graphMaxX(current.addSecs(hold));

        // Set random Y axis limits
        graphMinY(0);
        graphMaxY(3);

        // No data currently
        hasData(false);
    }



    /**
     * Updates valve data
     *
     * @brief ValvesVsTime::updateValve
     * @param valve
     * @param series
     */
    void ValvesVsTime::update(int valve, QAbstractSeries *series)
    {
        // If series not null
        if (series)
        {
            if(valve == 1)
            {
                // Update graph
                updateGraph(series, currentValveOneId, m_valveOneData);
            }
            else if(valve == 2)
            {
                // Update graph
                updateGraph(series, currentValveTwoId, m_valveTwoData);
            }
            else if(valve == 3)
            {
                // Update graph
                updateGraph(series, currentValveThreeId, m_valveThreeData);
            }
            else if(valve == 4)
            {
                // Update graph
                updateGraph(series, currentValveFourId, m_valveFourData);
            }
            else if(valve == 5)
            {
                // Update graph
                updateGraph(series, currentValveFiveId, m_valveFiveData);
            }
            else if(valve == 6)
            {
                // Update graph
                updateGraph(series, currentValveSixId, m_valveSixData);
            }
            else if(valve == 7)
            {
                // Update graph
                updateGraph(series, currentValveSevenId, m_valveSevenData);
            }
            else if(valve == 8)
            {
                // Update graph
                updateGraph(series, currentValveEightId, m_valveEightData);
            }
            else if(valve == 9)
            {
                // Update graph
                updateGraph(series, currentValveNineId, m_valveNineData);
            }
        }
    }

    /**
     * Updates graph data
     *
     * @brief ValvesVsTime::updateGraph
     * @param series
     * @param currentId
     * @param data
     */
    void ValvesVsTime::updateGraph(QAbstractSeries *series, int &currentId, QVector<QPointF> &data)
    {
        // Get the seriers info
        QXYSeries* xySeries = static_cast<QXYSeries *>(series);

        // Get total number of records
        int records = xySeries->count();

        // If there is no data then the graph was re-init so reset the current id index
        if(records == 0)
            currentId = 0;

        // Check buffer is not too large
        if(records == maxBuffer)
            xySeries->remove(0);

        if(data.count() == maxBuffer)
            data.removeAt(0);

        // Loop through new data currentId
        for (int i = currentId; i < data.size(); ++i)
        {
            // Append data
            xySeries->append(data.at(i));

            // Mark current id
            currentId = i;
        }
    }



    /**
     * Updates the valve data
     *
     * @brief ValvesVsTime::valveData
     * @param package
     */
    void ValvesVsTime::valveData(QVariantMap package)
    {
        // Get valve number
        int valve = m_settings.hardware.valve_connections.key(package.value("port").toString()).toInt();

        // Update valve value
        int value = package.value("value").toReal();

        if(valve == 1)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 1 : 0;

            // Update data
            updateTimedData(graphValue, m_valveOneData);

            // Emit we have new data
            emit emit_newValveOneGraphData();
        }
        else if(valve == 2)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 1.4 : 0;

            // Update data
            updateTimedData(graphValue, m_valveTwoData);

            // Emit we have new data
            emit emit_newValveTwoGraphData();
        }
        else if(valve == 3)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 1.8 : 0;

            // Update data
            updateTimedData(graphValue, m_valveThreeData);

            // Emit we have new data
            emit emit_newValveThreeGraphData();
        }
        else if(valve == 4)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 2.2 : 0;

            // Update data
            updateTimedData(graphValue, m_valveFourData);

            // Emit we have new data
            emit emit_newValveFourGraphData();
        }
        else if(valve == 5)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 2.6 : 0;

            // Update data
            updateTimedData(graphValue, m_valveFiveData);

            // Emit we have new data
            emit emit_newValveFiveGraphData();
        }
        else if(valve == 6)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 3 : 0;

            // Update data
            updateTimedData(graphValue, m_valveSixData);

            // Emit we have new data
            emit emit_newValveSixGraphData();
        }
        else if(valve == 7)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 3.4 : 0;

            // Update data
            updateTimedData(graphValue, m_valveSevenData);

            // Emit we have new data
            emit emit_newValveSevenGraphData();
        }
        else if(valve == 8)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 3.8 : 0;

            // Update data
            updateTimedData(graphValue, m_valveEightData);

            // Emit we have new data
            emit emit_newValveEightGraphData();
        }
        else if(valve == 9)
        {
            // Find graph value
            qreal graphValue = (value == 1) ? 4.2 : 0;

            // Update data
            updateTimedData(graphValue, m_valveNineData);

            // Emit we have new data
            emit emit_newValveNineGraphData();
        }

    }



    /**
     * Adds data to store with time stamp
     *
     * @brief ValvesVsTime::updateTimedData
     * @param value
     * @param store
     */
    void ValvesVsTime::updateTimedData(qreal value, QVector<QPointF> &store)
    {
        // Only store 1000 values
        if(store.count() >= 1000)
            store.pop_front();

        // Get the current time
        qreal time = qreal(QDateTime::currentMSecsSinceEpoch());

        // Add new data to container
        store.append(QPointF(time, value));
    }

}}}}


