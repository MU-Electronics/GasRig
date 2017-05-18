#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVariantMap>

#include "Setting.h"

namespace App { namespace Settings{
    class View: public App::Settings::Setting
    {
        Q_OBJECT
    public:
        View(QObject *parent = 0);
        ~View();

        QVariantMap JFET;
        QVariantMap owner;

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

    private:
        //explicit View(const View& rhs) = delete;
        //View& operator= (const View& rhs) = delete;
    };
}}


