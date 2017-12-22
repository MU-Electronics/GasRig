#include "VacuumVsTime.h"

#include <cmath>

#include <QVariantMap>
#include <QString>
#include <QtGlobal>
#include <QDateTime>

namespace App { namespace View { namespace Managers { namespace Graphs
{
    VacuumVsTime::VacuumVsTime(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
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
    void VacuumVsTime::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Pressure sensor
        connect(&hardware, &Hardware::Access::emit_readVacuumPressure, this, &VacuumVsTime::data);

        // Get the current date time
        QDateTime current = QDateTime::currentDateTime();

        // Append correct x axis limits
        graphMinX(current);
        graphMaxX(current.addSecs(hold));

        // Set random Y axis limits
        graphMinY(0.001);
        graphMaxY(2);

        // No data currently
        hasData(false);
    }



    /**
     * Updates the graph data
     *
     * @brief VacuumVsTime::updateGraph
     * @param series
     */
    void VacuumVsTime::update(QAbstractSeries *series)
    {
        // If series not null
        if (series)
            updateGraph(series, currentVacuumId, m_vacuumData);
    }



    /**
     * Saves the vacuum data recoreded
     *
     * @brief PressureVsTime::data
     * @param package
     */
    void VacuumVsTime::data(QVariantMap package)
    {
        // If port is the same as the vacuum guage port
        if(package["port"] == m_settings.hardware.vacuum_guage.value("connection").toString())
        {
            // Get pressure
            double vacuum = (std::pow(10, (1.667*package.value("calibrated").toDouble()-9.333)))/100;

            // Update data
            updateTimedData((qreal) vacuum, m_vacuumData);

            // update axis to fit
            updateAxis(m_vacuumData);

            // has data
            if(!hasData())
                hasData(true);

            // Emit we have new data
            emit emit_newVacuumGraphData();
        }
    }

}}}}


