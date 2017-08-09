#pragma once

// Include external deps
#include <QObject>

// Include the interface
#include "PresenterSupport.h"

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class LabJackPresenter   :   public PresenterSupport
    {

        public:
            LabJackPresenter(QObject *parent);

            // Select the correct presenter
            QVariantMap proccess(QString method, QVariantMap commands, QStringList package);

            // Validate package
            bool expectedPackage(QVariantMap commands, QStringList package, int expectedLength);
            QVariantMap generateError(QString method, QVariantMap commands, QStringList package);

            // Presenters
            QVariantMap configureIO(QVariantMap commands, QStringList package);

            QVariantMap setDigitalPort(QVariantMap commands, QStringList package);
            QVariantMap setAnaloguePort(QVariantMap commands, QStringList package);

            QVariantMap readPortDirection(QVariantMap commands, QStringList package);
            QVariantMap readDigitalPort(QVariantMap commands, QStringList package);
            QVariantMap readAnaloguePort(QVariantMap commands, QStringList package);

        private:
            int error_returnedPackageSize = -1;

    };

}}}}

