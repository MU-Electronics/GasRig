#pragma once

// Include external deps
#include <QObject>

// Include the interface
#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class PressureSensorPresenter   :   public PresenterSupport
    {

        public:
            PressureSensorPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

            // Validate package info
            bool expectedPackage(QVariantMap commands, QStringList package, QString commandId, int expectedLength);
            QVariantMap generateError(QString method, QVariantMap commands, QStringList package);

            // Presenters
            QVariantMap confirmInit(QVariantMap commands, QStringList package);
            QVariantMap readSerialNumber(QVariantMap commands, QStringList package);
            QVariantMap readPressure(QVariantMap commands, QStringList package);

        private:
            // Hold validation failed information
            int error_returnedPackageSize = -1;
            int error_returnedCommandId = -1;

    };

}}}}

