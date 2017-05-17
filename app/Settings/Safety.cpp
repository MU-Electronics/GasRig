#include "Safety.h"

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>


namespace App { namespace Settings
{

    /**
     * Init class
     *
     * @brief Safety::Safety
     * @param parent
     */
    Safety::Safety(QObject *parent)
    {

    }



    /**
     * Delete Class
     *
     * @brief Safety::~Safety
     */
    Safety::~Safety()
    {

    }


    /**
     * Format the QJsonObject into a useable member varaible
     *      - Example: valve_error.value("valve_open_to_exhaust").toMap().value("3");
     *
     * @brief read
     * @param json
     */
    void Safety::read(const QJsonObject &json)
    {
        // Store max pressure drop and rise
        pressure["pressure_rise"] = json["pressure_rise"].toString();
        pressure["pressure_fall"] = json["pressure_fall"].toString();

        // Store max vacuum drop and rise
        vacuum["vacuum_rise"] = json["vacuum_rise"].toString();
        vacuum["vacuum_fall"] = json["vacuum_fall"].toString();

        // Store valve configuration
        QVariantMap root_map = json.toVariantMap();
        valve_error = root_map["valve_combinations"].toMap();


        // @todo REMOVE THIS just debugging
        QJsonObject temp;
        write(temp);
    }


    /**
     * Format a QJsonObject to write to a json file
     *
     * @brief write
     * @param json
     */
    void Safety::write(QJsonObject &json) const
    {
        // Save max pressure drop and rise
        json["pressure_rise"] = pressure["pressure_rise"].toString();
        json["pressure_fall"] = pressure["pressure_fall"].toString();

        // Save max vacuum drop and rise
        json["vacuum_rise"] = vacuum["vacuum_rise"].toString();
        json["vacuum_fall"] = vacuum["vacuum_fall"].toString();

        // Store valve configuration
        QJsonObject valveErrors;

        // Loop thgrough set congiuration
        for(auto & outer : valve_error.toStdMap())
        {
            // Cache inner value states for each error condition
            QJsonObject errorState;

            // Set the valve  states
            for(auto & inner : outer.second.toMap().toStdMap())
            {
                // Get the valve id
                QString valveId = inner.first;

                // Get the valve state
                bool state = inner.second.toBool();

                // Set the states in the cache inner container
                errorState[valveId] = state;
            }

            // Write the state of the cache inner container to end container
            valveErrors[outer.first] = errorState;
        }

        json["valve_combinations"] = valveErrors;

        // Show end object
        qDebug() << json;

    }

}}




