#include "View.h"

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

namespace App { namespace Settings
{
    View::View(QObject *parent)
    {

    }

    View::~View()
    {

    }


    void View::read(const QJsonObject &json)
    {
        // Get root to map
        QVariantMap rootMap = json.toVariantMap();

        // Store the details of the developer
        JFET = rootMap["JFET"].toMap();

        // Store the owners details
        owner = rootMap["owner"].toMap();

        QJsonObject test;
        write(test);
    }

    void View::write(QJsonObject &json) const
    {
        // Save JFET Details
        QJsonObject JFETDetails;
        JFETDetails["phone"] = JFET["phone"].toString();
        JFETDetails["address"] = JFET["address"].toString();
        JFETDetails["email"] = JFET["email"].toString();
        JFETDetails["developer"] = JFET["developer"].toString();
        JFETDetails["developer_email"] = JFET["developer_email"].toString();
        json["JFET"] = JFETDetails;

        // Save the owners details
        QJsonObject ownerDetails;
        ownerDetails["name"] = owner["name"].toString();
        ownerDetails["section"] = owner["section"].toString();
        ownerDetails["company"] = owner["company"].toString();
        ownerDetails["phone"] = owner["phone"].toString();
        ownerDetails["email"] = owner["email"].toString();
        json["owner"] = ownerDetails;

        qDebug() << json;
    }

}}
