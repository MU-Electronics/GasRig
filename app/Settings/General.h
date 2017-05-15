#pragma once

#include <QString>
#include <QDebug>
#include <QJsonObject>

#include "Setting.h"

namespace App { namespace Settings{
    class General: public App::Settings::Setting
    {
        Q_OBJECT
    public:
        General();
        ~General();

        void read(QJsonObject &json);
        void write(QJsonObject &json);

    private:
        //explicit General(const General& rhs) = delete;
        //General& operator= (const General& rhs) = delete;
    };
}}


