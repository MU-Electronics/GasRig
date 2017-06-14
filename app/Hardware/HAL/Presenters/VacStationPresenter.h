#pragma once

#include <QObject>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class VacStationPresenter
    {

        public:
            VacStationPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

            // Presenters

    };

}}}}

