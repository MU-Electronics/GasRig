#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVariantMap>

#include "../Services/JsonFile.h"

namespace App { namespace Settings{

    class General: public QObject, private Services::JsonFile
    {

    public:
        General(QObject *parent, QString location);

        QVariantMap admin;

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;

    signals:
        void emit_saved();

    private:
        //explicit General(const General& rhs) = delete;
        //General& operator= (const General& rhs) = delete;
    };
}}


