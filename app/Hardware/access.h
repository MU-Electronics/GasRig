#pragma once

// Include external libs
#include <QObject>
#include <QThread>
#include <QMutex>

// Include settings container
#include "../Settings/container.h"

// Include thread
#include "../Services/Thread.h"

// Include HALS
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
            Access(QObject *parent, Settings::Container settings);
            ~Access();

        private:
            Settings::Container m_settings;

            HAL::VacStation& m_vacStation;
            HAL::FlowController& m_flowController;
            HAL::PressureSensor& m_pressureSensor;
            HAL::LabJack& m_labjack;

            void configure();
            void worker();

        public slots:
            //void worker();

    };
}}


