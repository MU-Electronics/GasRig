#pragma once

// Include external libs
#include <QObject>
#include <QDebug>
#include <QString>


// Include serial controller
#include "../../Services/SerialController.h"

namespace App { namespace Hardware { namespace HAL
{
    class VacStation: public App::Services::SerialController
    {
        public:
            VacStation(QObject *parent, int id);
            ~VacStation();

        private:
            int m_id;

            void proccessReadData(QString readData);

            QString CreatePackage(QString action, QString parameterValue, QString data);


            //explicit VacStation(const VacStation& rhs) = delete;
            //VacStation& operator= (const VacStation& rhs) = delete;
    };
}}}


