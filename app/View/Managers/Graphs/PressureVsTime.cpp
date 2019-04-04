#include "PressureVsTime.h"

#include <cmath>

#include <QVariantMap>
#include <QString>
#include <QtGlobal>
#include <QDateTime>


namespace App { namespace View { namespace Managers { namespace Graphs
{
    PressureVsTime::PressureVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine& experimentEngine)
        : Graph(parent),
          m_root(root),
          m_settings(settings),
            m_experimentEngine(experimentEngine)
    {

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

        // Disable unused compiler warnings
        UNUSED(safety);
    }



    /**
     * Updates the graph data
     *
     * @brief PressureVsTime::updateGraph
     * @param series
     */
    void PressureVsTime::update(QAbstractSeries *series)
    {
        // If series not null
        if (series)
            updateGraph(series, currentPressureId, m_pressureData);
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
        updateAxis(m_pressureData);

        // has data
        if(!hasData())
            hasData(true);

        // Emit we have new data
        emit emit_newPressureGraphData();
    }

}}}}


