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

            void setId(int id);

        signals:
            void emit_vacStationData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        private slots:
            // Required slot for full HAL Support compliance
            void testConnection();
            void resetConnection();

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

            QByteArray createPackage(QString action, QString parameterValue, QString data);
            bool send(QString action, QString parameterValue, QString data);

            bool validate(QStringList package);
            void proccessReadData(QStringList readData);
            QString calculateCheckSum(QStringList stringList);

    };
}}}


