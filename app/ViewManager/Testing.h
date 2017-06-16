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

namespace App { namespace ViewManager
{
    class Testing : public QObject, public App::ViewManager::Manager
    {
        Q_OBJECT
        AUTO_PROPERTY (QString, exampleVar)

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

            void requestPressureConfirmation();
            void requestPressureReading();
            void requestPressureSerialNumber();

            void requestLabJackConfig();
            void requestValveState(int port, bool state);
            void requestVacuumPressure();

            void requestValveOverride(QString controller, int state);


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


        private:
            QQmlApplicationEngine* m_root;
            Settings::Container m_settings;

    };
}}


