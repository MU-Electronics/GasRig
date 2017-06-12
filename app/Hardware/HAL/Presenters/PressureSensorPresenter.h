#pragma once

#include <QObject>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class PressureSensorPresenter
    {

        public:
            PressureSensorPresenter(QObject *parent);

            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);
    };

}}}}

