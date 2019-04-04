#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVariantMap>

#include "../Services/JsonFile.h"

namespace App { namespace Settings{
    class Hardware: public App::Services::JsonFile
    {

    public:
        Hardware();

        QVariantMap usb_connections;
        QVariantMap valve_connections;
        QVariantMap absoulte_maximums;
        QVariantMap polling_times;
        QVariantMap vacuum_guage;
        QVariantMap flow_controller_units;
        QVariantMap rig_config;

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

    private:
        //explicit Hardware(const Hardware& rhs) = delete;
        //Hardware& operator= (const Hardware& rhs) = delete;
    };
}}


