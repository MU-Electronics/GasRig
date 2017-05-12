#pragma once

// include external libs
#include <QObject>
#include <QThread>
#include <QMutex>

// include thread
#include "../Services/Thread.h"

// include HALS
#include "HAL/FlowController.h"
#include "HAL/Labjack.h"
#include "HAL/PressureSensor.h"
#include "HAL/VacStation.h"

namespace App { namespace Hardware
{
    class Access: public App::Services::Thread
    {
        Q_OBJECT
        public:
            Access(QObject *parent = 0);
            ~Access();

        private:
            HAL::VacStation m_vacStation;
            HAL::FlowController m_flowController;
            HAL::PressureSensor m_pressureSensor;
            HAL::LabJack m_labjack;

            void setupHAL();
            void worker();

        public slots:
            //void worker();

    };
}}


