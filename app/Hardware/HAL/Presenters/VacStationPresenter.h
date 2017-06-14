#pragma once

// Include external deps
#include <QObject>

// Include the interface
#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class VacStationPresenter   :   public PresenterSupport
    {

        public:
            VacStationPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

            // Presenters
            QVariantMap getTemperature(QVariantMap commands, QStringList package);
            QVariantMap getTurboSpeed(QVariantMap commands, QStringList package);
            QVariantMap getError(QVariantMap commands, QStringList package);
            QVariantMap getGasMode(QVariantMap commands, QStringList package);
            QVariantMap getBackingPumpMode(QVariantMap commands, QStringList package);
            QVariantMap getTurboPumpState(QVariantMap commands, QStringList package);
            QVariantMap getPumpingState(QVariantMap commands, QStringList package);

            QVariantMap setGasMode(QVariantMap commands, QStringList package);
            QVariantMap setBackingPumpMode(QVariantMap commands, QStringList package);
            QVariantMap setTurboPumpState(QVariantMap commands, QStringList package);
            QVariantMap setPumpingState(QVariantMap commands, QStringList package);

    };

}}}}

