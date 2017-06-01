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

// Include HAL presenters
#include "HAL/Presenters/PressureSensorPresenter.h"


namespace App { namespace Hardware
{
    class Access: public App::Services::Thread
    {
        Q_OBJECT
        public:
            Access(QObject *parent, Settings::Container settings);
            ~Access();

        signals:
            void emit_methodAttemptResults(QVariantMap status);

            void emit_serialComUpdated(QVariantMap command);
            void emit_timeoutSerialError(QVariantMap command);
            void emit_critialSerialError(QVariantMap command);

        public slots:
            void hardwareAccess(QVariantMap command);

            void listen_serialComUpdates(QVariantMap command);
            void listen_critialSerialError(QVariantMap command);
            void listen_timeoutSerialError(QVariantMap command);

            void listen_pressureSensorOne(QVariantMap command);

            // Access function that can be invoked externally via the queue
            // None atm; for example: void resetAllUSBConnection();

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

            // HAL presenters
            HAL::Presenters::PressureSensorPresenter& m_pressurePresenter;

            // Methods in this class that can be ran externally
            QList<QString> m_avaliableMethods;

            // Store command data for access invoked commands
            QVariantMap m_lastcommands;

            // Runs the correct HAL function
            void executeHalMethods(QVariantMap command);

            void proccessDataFromHals(QString responable, QString method, QStringList halData);

            // Check com port connections
            void connectDevices();

            // Ran when configuring thread
            void configure(QThread &thread);

            // The worker to run in forever loop
            void worker();

    };
}}


