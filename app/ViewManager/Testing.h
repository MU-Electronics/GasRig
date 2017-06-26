#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QString>

#include "Manager.h"
#include "../Utilities/Properties.h"

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Include command constructor
#include "../Hardware/CommandConstructor.h"

namespace App { namespace ViewManager
{
    class Testing : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT

        public:
            Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings);
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);


        signals:
            void hardwareRequest(QVariantMap command);
            void emit_testingMaintenanceReply(QString reply);

        public slots:
            // Requests comming from the views
            void requestBackingPump(bool onOff);
            void requestTurboPump(bool onOff);
            void requestBackingPumpMode(int mode);
            void requestGasMode(int mode);
            void requestTurboSpeed();
            void requestTurboBearingTemperature();

            void requestPressureConfirmation();
            void requestPressureReading();
            void requestPressureSerialNumber();

            void requestLabJackConfig();
            void requestValveState(int port, bool state);
            void requestVacuumPressure();

            void requestFlowControllerValveOverride(QString controller, int state);
            void requestFlowControllerFlowRate(QString controller, int flowrate);
            void requestFlowControllerSoftStartTime(QString controller, int seconds);
            void requestFlowControllerSoftStart(QString controller, int state);
            void requestSetFlowControllerSourceControl(QString controller, int source);
            void requestActualFlowControllerFlowRate(QString controller);
            void requestFlowControllerTemperature(QString controller);

            // Request experiment methods
            void requestHighPressure(QString pressure);
            void requestExhuast(int filterType, int frequency);
            void requestPurgeSystemMethodOne(bool outputValve, int cycles, QString pressure);
            void requestPurgeSystemMethodTwo(int minutes, QString pressure);
            void requestValveStateSafe(int port, bool state);
            void requestOutputPressure(int frequency);
            void requestVacDown(int mintues, bool turbo);




            // Returned actions
            void receiveVacSetPump(QVariantMap command);
            void receiveVacSetTurbo(QVariantMap command);
            void receiveVacSetGasMode(QVariantMap command);
            void receiveVacSetPumpMode(QVariantMap command);

            void receivePressureSensorInit(QVariantMap command);
            void receivePressureSensorReading(QVariantMap command);
            void receivePressureSensorSerialNumber(QVariantMap command);

            void receiveValveStatus(QVariantMap command);
            void receiveLabJackConfig(QVariantMap command);
            void receiveVacuumReading(QVariantMap command);

            void receiveFlowControllerValveOverride(QVariantMap command);
            void receiveSetFlowControllerFlowRate(QVariantMap command);
            void receiveSetFlowControllerSoftStart(QVariantMap command);
            void receiveSetFlowControllerSoftStartTime(QVariantMap command);
            void receiveSetFlowControllerSourceControl(QVariantMap command);



        private:
            QQmlApplicationEngine* m_root;

            // Holds the settings container
            Settings::Container m_settings;

            // Hold instance of command constructor
            Hardware::CommandConstructor m_commandConstructor;

    };
}}


