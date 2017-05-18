#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QVariantMap>
#include <QMap>

#include "Setting.h"

namespace App { namespace Settings{
    class Safety: public App::Settings::Setting
    {
        Q_OBJECT
    public:

        Safety(QObject *parent = 0);
        ~Safety();

        QVariantMap pressure;
        QVariantMap vacuum;
        QVariantMap flow_controller;
        QVariantMap turbo_pump;
        QVariantMap valve_error;


        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

    private:
        //explicit Safety(const Safety& rhs) = delete;
        //Safety& operator= (const Safety& rhs) = delete;
    };
}}


