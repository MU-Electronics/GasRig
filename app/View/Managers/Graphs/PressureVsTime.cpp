#include "PressureVsTime.h"

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
    PressureVsTime::PressureVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
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
     * @brief PressureVsTime::makeConnections
     * @param hardware
     * @param safety
     */
    void PressureVsTime::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Pressure sensor
        connect(&hardware, &Hardware::Access::emit_pressureSensorPressure, this, &PressureVsTime::pressureData);
        connect(&hardware, &Hardware::Access::emit_setValveState, this, &PressureVsTime::valveData);

        // Get the current date time
        QDateTime current = QDateTime::currentDateTime();

        // Append correct x axis limits
        graphMinX(current);
        graphMaxX(current.addSecs(hold));

        // Set random Y axis limits
        graphMinY(0.8);
        graphMaxY(5);

        // No data currently
        hasData(false);
    }



    /**
     * Updates the graph data
     *
     * @brief PressureVsTime::updateGraph
     * @param series
     */
    void PressureVsTime::updatePressure(QAbstractSeries *series)
    {
        // If series not null
        if (series)
            updateGraph(series, currentPressureId, m_pressureData);
    }


    /**
     * Updates valve data
     *
     * @brief PressureVsTime::updateValve
     * @param valve
     * @param series
     */
    void PressureVsTime::updateValve(int valve, QAbstractSeries *series)
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
            else if(valve == 7)
            {
                // Update graph
                updateGraph(series, currentValveSevenId, m_valveSevenData);
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
     * @brief PressureVsTime::updateGraph
     * @param series
     * @param currentId
     * @param data
     */
    void PressureVsTime::updateGraph(QAbstractSeries *series, int &currentId, QVector<QPointF> &data)
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

        // Delete pointer
        // delete xySeries;
    }


    /**
     * Saves the pressure data recoreded
     *
     * @brief PressureVsTime::data
     * @param package
     */
    void PressureVsTime::pressureData(QVariantMap package)
    {
        // Update data
        updateTimedData(package["pressure"].toReal(), m_pressureData);

        // update axis to fit
        updatePressureAxis();

        // has data
        if(!hasData())
            hasData(true);

        // Emit we have new data
        emit emit_newPressureGraphData();
    }


    /**
     * Updates the valve data
     *
     * @brief PressureVsTime::valveData
     * @param package
     */
    void PressureVsTime::valveData(QVariantMap package)
    {
        // Get valve number
        int valve = m_settings.hardware.valve_connections.key(package.value("port").toString()).toInt();

        // Update valve value
        int value = package.value("value").toReal();

        if(valve == 1)
        {
            // Update data
            updateTimedData(value, m_valveOneData);

            // Emit we have new data
            emit emit_newValveOneGraphData();
        }
        else if(valve == 2)
        {
            // Update data
            updateTimedData(value, m_valveTwoData);

            // Emit we have new data
            emit emit_newValveTwoGraphData();
        }
        else if(valve == 7)
        {
            // Update data
            updateTimedData(value, m_valveSevenData);

            // Emit we have new data
            emit emit_newValveSevenGraphData();
        }
        else if(valve == 9)
        {
            // Update data
            updateTimedData(value, m_valveNineData);

            // Emit we have new data
            emit emit_newValveNineGraphData();
        }

    }



    /**
     * Adds data to store with time stamp
     *
     * @brief PressureVsTime::updateTimedData
     * @param value
     * @param store
     */
    void PressureVsTime::updateTimedData(qreal value, QVector<QPointF> &store)
    {
        // Only store 1000 values
        if(store.count() >= 1000)
            store.pop_front();

        // Get the current time
        qreal time = qreal(QDateTime::currentMSecsSinceEpoch());

        // Add new data to container
        store.append(QPointF(time, value));
    }


     /**
      * Updates the axis limits
      */
    void PressureVsTime::updatePressureAxis()
    {
        // Does the x axis need scaling?
        if(qRound64(m_pressureData.last().x()) > graphMaxX().toMSecsSinceEpoch())
        {
            // Increase X axis
            graphMinX(graphMinX().addSecs(scrollBy));
            graphMaxX(graphMaxX().addSecs(scrollBy));
        }

        // Does the Y axis need scaling
        double pressure = m_pressureData.last().y();
        if (pressure > graphMaxY())
        {
            // Increase Y axis
            graphMaxY(pressure + 1);
        }
        else if (pressure < graphMinY())
        {
            // Increase Y axis
            graphMinY(pressure - 1);
        }
    }

}}}}


