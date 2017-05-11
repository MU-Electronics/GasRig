#pragma once

// include external libs
#include <QObject>
#include <QThread>

// include HALS
#include "HAL/FlowController.h"
#include "HAL/Labjack.h"
#include "HAL/PressureSensor.h"
#include "HAL/VacStation.h"

namespace App { namespace Hardware
{
    class Access: public QObject
    {
        Q_OBJECT
        public:
            explicit Access();
            ~Access();

            void setup(QThread &thread);

        public slots:
            void runner();



        private:
            HAL::VacStation m_vacStation;
            HAL::FlowController m_flowController;
            HAL::PressureSensor m_pressureSensor;
            HAL::LabJack m_labjack;

            void setupHAL();

            //QThread* m_thread;
    };
}}


