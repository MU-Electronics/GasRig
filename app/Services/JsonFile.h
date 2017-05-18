#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QJsonObject>

namespace App { namespace Services {
    class JsonFile : public QObject
    {
        Q_OBJECT
    public:
        JsonFile(QObject *parent = 0);
        virtual ~JsonFile() {};

        enum SaveFormat {
            Json, Binary
        };

        QString m_loadedFile;
        SaveFormat m_format;

        bool load(QString location, SaveFormat format);
        bool update();

        virtual void read(const QJsonObject &json) = 0;
        virtual void write(QJsonObject &json) const = 0;
    };
}}


