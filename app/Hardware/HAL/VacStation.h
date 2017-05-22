#pragma once

// Include external libs
#include <QObject>
#include <QDebug>
#include <QString>
#include <QByteArray>


// Include serial controller
#include "../../Services/SerialController.h"

namespace App { namespace Hardware { namespace HAL
{
    class VacStation: public App::Services::SerialController
    {
        Q_OBJECT
        public:
            VacStation(QObject *parent, int id);
            ~VacStation();

            // Get vac pump data
            void GetTemperature(int location);
            void GetTurboSpeed(int type);
            void GetError(int id);
            void GetGasMode();
            void GetBackingPumpMode();
            void GetTurboPumpState();
            void GetPumpingState();

            // Set vac pump params
            void SetGasMode(int mode);
            void SetBackingPumpMode(int mode);
            void SetTurboPumpState(int state);
            void SetPumpingState(int state);

       signals:
            void vacStationData(QString id, QString param, QString data);

        private:
            int m_id;

            void proccessReadData(QString readData);

            QByteArray CreatePackage(QString action, QString parameterValue, QString data);
            bool send(QString action, QString parameterValue, QString data);
            bool validate(QString package);

    };
}}}


