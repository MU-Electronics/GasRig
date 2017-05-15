#pragma once

#include <QString>
#include <QDebug>
#include <QJsonObject>

#include "Setting.h"

namespace App { namespace Settings{
    class Hardware: public App::Settings::Setting
    {
        Q_OBJECT
    public:
        Hardware();
        ~Hardware();

        void read(QJsonObject &json);
        void write(QJsonObject &json);

    private:
        //explicit Hardware(const Hardware& rhs) = delete;
        //Hardware& operator= (const Hardware& rhs) = delete;
    };
}}


