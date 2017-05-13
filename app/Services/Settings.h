#pragma once

#include <QString>
#include <QDebug>
#include <QJsonObject>

namespace App { namespace Services{
    class Settings
    {
    public:
        Settings(QString location, Settings::SaveFormat format);
        ~Settings();

        enum SaveFormat {
            Json, Binary
        };

        QString m_loadedFile;
        SaveFormat m_format;

        bool load();
        bool update() const;

        virtual void read(const QJsonObject &json) = 0;
        virtual void write(QJsonObject &json) const = 0;
    private:
        explicit Settings(const Settings& rhs) = delete;
        Settings& operator= (const Settings& rhs) = delete;
    };
}}


