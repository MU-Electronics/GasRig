#include "Hardware.h"

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

namespace App { namespace Settings
{
    Hardware::Hardware(QObject *parent)
    {

    }

    Hardware::~Hardware()
    {

    }


    void Hardware::read(const QJsonObject &json)
    {
        // Store usb connection info


        // Store valve connections


        // Store absolute maxmiums


        // Store polling times


        // For debugging will be removed
        QJsonObject test;
        write(test);
    }

    void Hardware::write(QJsonObject &json) const
    {



        qDebug() << json;
    }

}}
