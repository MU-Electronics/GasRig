#pragma once

#include <QString>
#include <QDebug>
#include <QJsonObject>

#include "Setting.h"

namespace App { namespace Settings{
    class View: public App::Settings::Setting
    {
        Q_OBJECT
    public:
        View();
        ~View();

        void read(QJsonObject &json);
        void write(QJsonObject &json);

    private:
        //explicit View(const View& rhs) = delete;
        //View& operator= (const View& rhs) = delete;
    };
}}


