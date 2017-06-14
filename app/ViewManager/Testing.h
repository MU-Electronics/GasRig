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

        public slots:
            // Requests comming from the views
            void requestBackingPump(bool onOff);
            void requestTurboPump(bool onOff);

            void requestPressureConfirmation();
            void requestPressureReading();


            // Returned actions
            void pressureSensorInit(QVariantMap command);
            void pressureSensorReading(QVariantMap command);

        private:
            QQmlApplicationEngine* m_root;
            Settings::Container m_settings;

    };
}}


