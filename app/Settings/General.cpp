#include "General.h"

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

namespace App { namespace Settings
{
    General::General(QObject *parent, QString location)
        :   QObject(parent)
        ,   JsonFile()
    {
        // Load json data
        load(location, SaveFormat::Json);
    }

    void General::read(const QJsonObject &json)
    {
        // Get root to map
        QVariantMap rootMap = json.toVariantMap();

        // Store the admin login details
        admin = rootMap["admin_login"].toMap();

    }

    void General::write(QJsonObject &json) const
    {
        // Save admins username and password
        QJsonObject adminData;
        adminData["username"] = admin["username"].toString();
        adminData["password"] = admin["password"].toString();
        json["admin_login"] = adminData;
    }

}}
