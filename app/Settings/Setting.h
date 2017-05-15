#pragma once

#include <QObject>
#include <QString>
#include <QDebug>
#include <QJsonObject>

namespace App { namespace Settings{
    class Setting : public QObject
    {
        Q_OBJECT
    public:
        Setting(QObject *parent = 0);
        virtual ~Setting() {};

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


