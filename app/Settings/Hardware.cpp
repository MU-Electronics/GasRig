#include "Hardware.h"

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

namespace App { namespace Settings
{
    Hardware::Hardware()
    {

    }


    void Hardware::read(const QJsonObject &json)
    {
        // Get root to map
        QVariantMap rootMap = json.toVariantMap();

        // Store usb connection info
        auto connectionData = rootMap["usb_connections"].toMap();
        usb_connections["pressure_sensor"] = connectionData["pressure_sensor"].toMap();
        usb_connections["vacuum_station"] = connectionData["vacuum_station"].toMap();
        usb_connections["labJack"] = connectionData["labJack"].toMap();
        usb_connections["flow_controller"] = connectionData["flow_controller"].toMap();

        // Store valve connections
        valve_connections = rootMap["valve_connections"].toMap();

        // Store valve connections
        vacuum_guage = rootMap["vacuum_guage"].toMap();

        // Store absolute maxmiums
        absoulte_maximums = rootMap["absoulte_maximums"].toMap();

        // Store polling times
        polling_times = rootMap["polling_times"].toMap();
    }

    void Hardware::write(QJsonObject &json) const
    {
        // Save usb connection info
        QJsonObject usbConnectionsData;

        // Loop trhough usb set congiuration
        for(auto & outer : usb_connections.toStdMap())
        {
            // Cache inner value states for each error condition
            QJsonObject usbConnectionData;

            // Set the valve  states
            for(auto & inner : outer.second.toMap().toStdMap())
            {
                // Get the valve id
                QString valveId = inner.first;

                // Get the valve state
                bool state = inner.second.toBool();

                // Set the states in the cache inner container
                usbConnectionData[valveId] = state;
            }

            // Write the state of the cache inner container to end container
            usbConnectionsData[outer.first] = usbConnectionData;
        }
        json["usb_connections"] = usbConnectionsData;

        // Save valve connections
        QJsonObject valveConnectionsData;
        valveConnectionsData["1"] = valve_connections["1"].toString();
        valveConnectionsData["2"] = valve_connections["2"].toString();
        valveConnectionsData["3"] = valve_connections["3"].toString();
        valveConnectionsData["4"] = valve_connections["4"].toString();
        valveConnectionsData["5"] = valve_connections["5"].toString();
        valveConnectionsData["6"] = valve_connections["6"].toString();
        valveConnectionsData["7"] = valve_connections["7"].toString();
        valveConnectionsData["8"] = valve_connections["8"].toString();
        valveConnectionsData["9"] = valve_connections["9"].toString();
        json["valve_connections"] = valveConnectionsData;


        // Save absolute maxmiums
        QJsonObject absoluteMaxData;
        absoluteMaxData["pressure"] = absoulte_maximums["pressure"].toString();
        absoluteMaxData["value_timing"] = absoulte_maximums["value_timing"].toString();
        absoluteMaxData["vacuum_max"] = absoulte_maximums["vacuum_max"].toString();
        json["absoulte_maximums"] = absoluteMaxData;


        // Save polling times
        QJsonObject pollingData;
        pollingData["pressure_sensor"] = polling_times["pressure_sensor"].toString();
        pollingData["vacuum_sensor"] = polling_times["vacuum_sensor"].toString();
        json["polling_times"] = pollingData;
    }

}}
