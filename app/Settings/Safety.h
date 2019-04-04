#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVariantMap>

#include "../Services/JsonFile.h"

namespace App { namespace Settings{
    class Safety: public QObject, public App::Services::JsonFile
    {

    public:

        Safety(QObject *parent, QString location);

        QVariantMap pressure;
        QVariantMap vacuum;
        QVariantMap flow_controller;
        QVariantMap turbo_pump;
        QVariantMap valve_error;


        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;
    signals:
        void emit_saved();
    private:
        //explicit Safety(const Safety& rhs) = delete;
        //Safety& operator= (const Safety& rhs) = delete;
    };
}}


