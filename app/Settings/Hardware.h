#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVariantMap>

#include "Setting.h"

namespace App { namespace Settings{
    class Hardware: public App::Settings::Setting
    {
        Q_OBJECT
    public:
        Hardware(QObject *parent = 0);
        ~Hardware();

        QVariantMap usb_connections;
        QVariantMap valve_connections;
        QVariantMap absoulte_maximums;
        QVariantMap polling_times;

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

    private:
        //explicit Hardware(const Hardware& rhs) = delete;
        //Hardware& operator= (const Hardware& rhs) = delete;
    };
}}


