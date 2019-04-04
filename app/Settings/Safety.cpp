#include "Safety.h"

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>


namespace App { namespace Settings
{

    /**
     * Init class
     *
     * @brief Safety::Safety
     * @param parent
     */
    Safety::Safety(QObject *parent, QString location)
        :   QObject(parent)
        ,   JsonFile()
    {
        // Load json data
        load(location, SaveFormat::Json);
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
        // Get root to map
        QVariantMap rootMap = json.toVariantMap();

        // Store max pressure drop and rise
        pressure = rootMap["pressure"].toMap();

        // Store max vacuum drop and rise
        vacuum = rootMap["vacuum"].toMap();

        // Save min and max flow rate
        flow_controller = rootMap["flow_controller"].toMap();

        // Turbo pump settings
        turbo_pump = rootMap["turbo_pump"].toMap();

        // Store valve configuration
        valve_error = rootMap["valve_combinations"].toMap();

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
        QJsonObject pressureData;
        pressureData["rise"] = pressure["rise"].toString();
        pressureData["fall"] = pressure["fall"].toString();
        json["pressure"] = pressureData;

        // Save max vacuum drop and rise
        QJsonObject vacuumData;
        vacuumData["rise"] = vacuum["rise"].toString();
        vacuumData["fall"] = vacuum["fall"].toString();
        json["vacuum"] = vacuumData;

        // Save min and max flow rate
        QJsonObject flowControllerData;
        flowControllerData["min_flow"] = flow_controller["min_flow"].toString();
        flowControllerData["max_flow"] = flow_controller["max_flow"].toString();
        json["flow_controller"] = flowControllerData;

        // Trubo pump settings
        QJsonObject turboPumpData;
        turboPumpData["temp_bottom"] = turbo_pump["temp_bottom"].toString();
        turboPumpData["temp_electronics"] = turbo_pump["temp_electronics"].toString();
        turboPumpData["temp_bearing"] = turbo_pump["temp_bearing"].toString();
        turboPumpData["temp_motor"] = turbo_pump["temp_motor"].toString();
        json["turbo_pump"] = turboPumpData;

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

    }

}}




