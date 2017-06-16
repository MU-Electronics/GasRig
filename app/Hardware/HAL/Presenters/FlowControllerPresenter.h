#pragma once

// Include external deps
#include <QObject>

// Include the interface
#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class FlowControllerPresenter   :   public PresenterSupport
    {

        public:
            FlowControllerPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

            // Presenters
            QVariantMap getIdentifier(QVariantMap commands, QStringList package);
            QVariantMap getFlowRate(QVariantMap commands, QStringList package);
            QVariantMap getSetFlowRate(QVariantMap commands, QStringList package);
            QVariantMap getValveOverride(QVariantMap commands, QStringList package);
            QVariantMap getControllerTemperature(QVariantMap commands, QStringList package);

            QVariantMap setSourceControll(QVariantMap commands, QStringList package);
            QVariantMap setFlowRate(QVariantMap commands, QStringList package);
            QVariantMap setValveOverride(QVariantMap commands, QStringList package);
            QVariantMap setSoftStart(QVariantMap commands, QStringList package);
            QVariantMap setSoftStartTime(QVariantMap commands, QStringList package);
            QVariantMap setFlowUnit(QVariantMap commands, QStringList package);
            QVariantMap setTemperatureUnit(QVariantMap commands, QStringList package);

        private:
            QStringList parse(QStringList &package);
    };

}}}}

