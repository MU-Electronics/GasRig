#pragma once

// Include external libs
#include <QObject>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>

// Include serial controller
#include "../../Services/SerialController.h"

// Interface for HAL classes
#include "HALSupport.h"

namespace App { namespace Hardware { namespace HAL
{
    class VacStation: public App::Services::SerialController, public HALSupport
    {
        Q_OBJECT
        public:
            VacStation(QObject *parent, int id);
            ~VacStation();

        signals:
            void vacStationData(QString id, QString param, QString data);

        private slots:
            // Get vac pump data
            void GetTemperature();
            void GetTurboSpeed();
            void GetError();
            void GetGasMode();
            void GetBackingPumpMode();
            void GetTurboPumpState();
            void GetPumpingState();

            // Set vac pump params
            void SetGasMode();
            void SetBackingPumpMode();
            void SetTurboPumpState();
            void SetPumpingState();

        private:
            int m_id;
            QString m_responsability = "VacStation";


            QByteArray CreatePackage(QString action, QString parameterValue, QString data);
            bool send(QString action, QString parameterValue, QString data);
            bool validate(QString package);


            void proccessReadData(QString readData);

    };
}}}


