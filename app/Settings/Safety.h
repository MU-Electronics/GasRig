#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QJsonObject>

#include "Setting.h"

namespace App { namespace Settings{
    class Safety: public App::Settings::Setting
    {
        Q_OBJECT
    public:

        Safety(QObject *parent = 0);
        ~Safety();

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

    private:
        //explicit Safety(const Safety& rhs) = delete;
        //Safety& operator= (const Safety& rhs) = delete;
    };
}}


