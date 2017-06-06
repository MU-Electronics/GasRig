#pragma once

#include <QObject>
#include <QString>
#include <QMap>

// Include serial controller
#include "../../Services/SerialController.h"

// Interface for HAL classes
#include "HALSupport.h"

namespace App { namespace Hardware { namespace HAL
{
    class FlowController: public App::Services::SerialController, public HALSupport
    {
        Q_OBJECT
        public:
            FlowController(QObject *parent);

            //void setId(int id);
            void registerController(QString referance, QString manuf, QString type, QString ID1, QString ID2, QString ID3);

        signals:
            void emit_flowControllerData(QString responable, QString m_method, QStringList halData);

        private slots:
            void getIdentifier();
            void getFlowRate();
            void getSetFlowRate();
            void getValveOverride();
            void getControllerTemperature();

            void setSourceControll();
            void setFlowRate();
            void setValveOverride();
            void setSoftStart();
            void setSoftStartTime();
            void setFlowUnit();
            void setTemperatureUnit();


            // Required slot for full HAL Support compliance
            void testConnection();
            void resetConnection();

        private:
            //int m_id;
            QMap<QString, QMap<QString, QString>> m_controllers;

            void proccessReadData(QStringList readData);
            bool validate(QStringList package);
            QString calculateCheckSum(QStringList package);
            QByteArray createPackage(QString selectedController, QString command, QStringList data);
            bool send(QString selectedController, QString command, QStringList data);

            explicit FlowController(const FlowController& rhs) = delete;
            FlowController& operator= (const FlowController& rhs) = delete;
    };
}}}


