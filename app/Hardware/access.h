#pragma once

// Include external libs
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QVariantMap>
#include <QQueue>

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

        public slots:
            void hardwareAccess(QVariantMap command);

        private:
            // Contains for settings container
            Settings::Container m_settings;

            // Timer for time based events
            QTimer& m_timer;

            // Queue for commands to be ran
            QQueue<QVariantMap> m_queue;

            // HAL objects
            HAL::VacStation& m_vacStation;
            HAL::FlowController& m_flowController;
            HAL::PressureSensor& m_pressureSensor;
            HAL::LabJack& m_labjack;

            // Runs the correct HAL function
            void executeHalMethods(QVariantMap command);

            // Ran when configuring thread
            void configure(QThread &thread);

            // The worker to run in forever loop
            void worker();

        public slots:
            //void worker();

    };
}}


