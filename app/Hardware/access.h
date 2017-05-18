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

// Include settings container
#include "../Settings/container.h"

namespace App { namespace Hardware
{
    class Access: public App::Services::Thread
    {
        Q_OBJECT
        public:
            Access(QObject *parent, Settings::Container settings);
            ~Access();

        private:
            Settings::Container m_settings;

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


