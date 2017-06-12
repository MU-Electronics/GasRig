#pragma once
#include <QObject>
#include <QByteArray>


// Include serial controller
#include "../../Services/SerialController.h"

// Interface for HAL classes
#include "HALSupport.h"

// Include presure sensor presenter
#include "Presenters/PressureSensorPresenter.h"


namespace App { namespace Hardware { namespace HAL
{
    class PressureSensor: public App::Services::SerialController, public HALSupport, public Presenters::PressureSensorPresenter
    {
        Q_OBJECT
        public:
            PressureSensor(QObject *parent, int id);

            void setId(int id);

        signals:
            void emit_pressureSensorData(QString responable, QString m_method, QVariantMap m_command, QStringList halData);

        public slots:
            void confirmInit();
            void readPressure();
            void readSerialNumber();
            void connectInitSensor(QVariantMap package);

            // Required slots for full HAL Support compliance
            void testConnection();
            void resetConnection();

        private:
            int m_id;

            QByteArray createPackage(QString action, QString parameterValue);
            bool send(QString action, QString parameterValue);

            void proccessReadData(QStringList readData);
            bool validate(QStringList package);
            QString calculateCheckSum(QStringList dataIn);

            //explicit PressureSensor(const PressureSensor& rhs) = delete;
            //PressureSensor& operator= (const PressureSensor& rhs) = delete;
    };
}}}


