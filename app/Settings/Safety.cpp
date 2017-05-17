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
    }


    /**
     * Format a QJsonObject to write to a json file
     *
     * @brief write
     * @param json
     */
    void Safety::write(QJsonObject &json) const
    {

    }

}}
