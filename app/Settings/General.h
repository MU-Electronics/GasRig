#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVariantMap>

#include "../Services/JsonFile.h"

namespace App { namespace Settings{
    class General: public App::Services::JsonFile
    {

    public:
        General();

        QVariantMap admin;

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

    private:
        //explicit General(const General& rhs) = delete;
        //General& operator= (const General& rhs) = delete;
    };
}}


