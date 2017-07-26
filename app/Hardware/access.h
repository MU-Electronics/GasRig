#pragma once

// Include external libs
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QVariantMap>
#include <QQueue>

// Include settings container
#include "../Settings/Container.h"

// Include thread
#include "../Services/Thread.h"

// Include HALS
#include "HAL/FlowController.h"
#include "HAL/Labjack.h"
#include "HAL/PressureSensor.h"
#include "HAL/VacStation.h"

// Include HAL presenters
#include "HAL/Presenters/PressureSensorPresenter.h"
#include "HAL/Presenters/LabJackPresenter.h"
#include "HAL/Presenters/FlowControllerPresenter.h"
#include "HAL/Presenters/VacStationPresenter.h"

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

            // Serial controller erros
            void emit_serialComUpdated(QVariantMap command);
            void emit_timeoutSerialError(QVariantMap command);
            void emit_critialSerialError(QVariantMap command);

            // Possable signals from pressure sensor presenter
            void emit_pressureSensorInit(QVariantMap command);
            void emit_pressureSensorSerialNumber(QVariantMap command);
            void emit_pressureSensorPressure(QVariantMap command);

            // Possable signals from the flow controller presenter
            void emit_getFlowControllerIdentifier(QVariantMap command);
            void emit_getFlowControllerSoftStartTime(QVariantMap command);
            void emit_getFlowControllerSoftStart(QVariantMap command);
            void emit_getFlowControllerSourceControl(QVariantMap command);
            void emit_getFlowControllerFlowRate(QVariantMap command);
            void emit_getFlowControllerSetFlowRate(QVariantMap command);
            void emit_getFlowControllerValveOverride(QVariantMap command);
            void emit_getFlowControllerTemperature(QVariantMap command);
            void emit_setFlowControllerSourceControl(QVariantMap command);
            void emit_setFlowControllerFlowRate(QVariantMap command);
            void emit_setFlowControllerValveOverride(QVariantMap command);
            void emit_setFlowControllerSoftStart(QVariantMap command);
            void emit_setFlowControllerSoftStartTime(QVariantMap command);
            void emit_setFlowControllerFlowUnit(QVariantMap command);
            void emit_setFlowControllerTemperatureUnit(QVariantMap command);

            // Possable signals from the vac station presenter
            void emit_getTemperature(QVariantMap command);
            void emit_getTurboSpeed(QVariantMap command);
            void emit_getError(QVariantMap command);
            void emit_getGasMode(QVariantMap command);
            void emit_getBackingPumpMode(QVariantMap command);
            void emit_getTurboPumpState(QVariantMap command);
            void emit_getPumpingState(QVariantMap command);
            void emit_setGasMode(QVariantMap command);
            void emit_setBackingPumpMode(QVariantMap command);
            void emit_setTurboPumpState(QVariantMap command);
            void emit_setPumpingState(QVariantMap command);

            // Possable signals from the labjack presenter
            void emit_configureIO(QVariantMap commands);
            void emit_setDigitalPort(QVariantMap commands);
            void emit_setAnaloguePort(QVariantMap commands);
            void emit_readPortDirection(QVariantMap commands);
            void emit_readDigitalPort(QVariantMap commands);
            void emit_readAnaloguePort(QVariantMap commands);


        public slots:
            void hardwareAccess(QVariantMap command);

            void listen_serialComUpdates(QVariantMap command);
            void listen_critialSerialError(QVariantMap command);
            void listen_timeoutSerialError(QVariantMap command);

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
            HAL::Presenters::LabJackPresenter& m_labJackPresenter;
            HAL::Presenters::FlowControllerPresenter& m_flowControllerPresenter;
            HAL::Presenters::VacStationPresenter& m_vacStationPresenter;

            // Methods in this class that can be ran externally
            QList<QString> m_avaliableMethods;

            // Store command data for access invoked commands
            QVariantMap m_lastcommands;

            // Runs the correct HAL function
            void executeHalMethods(QVariantMap command);

            void proccessDataFromHals(QString responable, QString method, QVariantMap commands, QStringList halData);

            // Check com port connections
            void connectDevices();

            // Ran when configuring thread
            void configure(QThread &thread);

            // The worker to run in forever loop
            void worker();

    };
}}


