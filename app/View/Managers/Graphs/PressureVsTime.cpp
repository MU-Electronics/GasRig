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
        connect(&hardware, &Hardware::Access::emit_pressureSensorPressure, this, &PressureVsTime::data);

        // Get the current date time
        QDateTime current = QDateTime::currentDateTime();

        // Append correct x axis limits
        graphMinX(current);
        graphMaxX(current.addSecs(hold));

        // Set random Y axis limits
        graphMinY(0.8);
        graphMaxY(5);
    }





    /**
     * Updates the graph data
     *
     * @brief PressureVsTime::updateGraph
     * @param series
     */
    void PressureVsTime::updateGraph(QAbstractSeries *series)
    {
        // If series not null
        if (series)
        {
            // Get the seriers info
            QXYSeries *xySeries = static_cast<QXYSeries *>(series);

            // Get total number of records
            int records = xySeries->count();

            // If there is no data then the graph was re-init so reset the current id index
            if(records == 0)
                currentId = 0;

            // Check buffer is not too large
            if(records == maxBuffer)
                xySeries->remove(0);

            if(m_data.count() == maxBuffer)
                m_data.removeAt(0);

            // Loop through new data currentId
            for (int i = currentId; i < m_data.size(); ++i)
            {
                // Append data
                xySeries->append(m_data.at(i));

                // Mark current id
                currentId = i;
            }
        }
    }





    /**
     * Saves the pressure data recoreded
     *
     * @brief PressureVsTime::data
     * @param package
     */
    void PressureVsTime::data(QVariantMap package)
    {
        // Get current pressure
        qreal pressure = package["pressure"].toReal();

        // Only store 1000 values
        if(m_data.count() >= 1000)
            m_data.pop_front();

        // Get the current time
        qreal time = qreal(QDateTime::currentMSecsSinceEpoch());

        // Add new data to container
        m_data.append(QPointF(time, pressure));

        // update axis to fit
        updateAxis();
    }


     /**
      * Updates the axis limits
      */
    void PressureVsTime::updateAxis()
    {
        // Does the x axis need scaling?
        if(qRound64(m_data.last().x()) > graphMaxX().toMSecsSinceEpoch())
        {
            // Increase X axis
            graphMinX(graphMinX().addSecs(scrollBy));
            graphMaxX(graphMaxX().addSecs(scrollBy));
        }

        // Does the Y axis need scaling
        double pressure = m_data.last().y();
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


