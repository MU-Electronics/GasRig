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
        public:
            VacStation(QObject *parent, int id);
            ~VacStation();

            void GetTemperature(int location);

        private:
            int m_id;

            void proccessReadData(QString readData);

            QByteArray CreatePackage(QString action, QString parameterValue, QString data);
            bool send(QString action, QString parameterValue, QString data, unsigned int bytesRead);


            //explicit VacStation(const VacStation& rhs) = delete;
            //VacStation& operator= (const VacStation& rhs) = delete;
    };
}}}


