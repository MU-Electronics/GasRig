#pragma once

#include <QObject>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class PressureSensorPresenter
    {

        public:
            PressureSensorPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

            // Presenters
            QVariantMap confirmInit(QVariantMap commands, QStringList package);
            QVariantMap readSerialNumber(QVariantMap commands, QStringList package);
            QVariantMap readPressure(QVariantMap commands, QStringList package);

    };

}}}}

