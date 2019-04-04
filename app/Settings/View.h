#pragma once

#include <QObject>
#include <QJsonObject>
#include <QVariantMap>

#include "../Services/JsonFile.h"

namespace App { namespace Settings{
    class View: public QObject, public App::Services::JsonFile
    {

    public:
        View(QObject *parent, QString location);

        QVariantMap JFET;
        QVariantMap owner;

        void read(const QJsonObject &json);
        void write(QJsonObject &json) const;
    signals:
        void emit_saved();
    private:
        //explicit View(const View& rhs) = delete;
        //View& operator= (const View& rhs) = delete;
    };
}}


