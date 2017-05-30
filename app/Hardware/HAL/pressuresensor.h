#pragma once
#include <QObject>
#include <QByteArray>


// Include serial controller
#include "../../Services/SerialController.h"

// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class PressureSensor: public App::Services::SerialController, public HALSupport
    {
        Q_OBJECT
        public:
            PressureSensor(QObject *parent, int id);

            void setId(int id);

        public slots:
            void confirmInit();
            void readPressure();

            // Required slot for full HAL Support compliance
            void testConnection();
            void resetConnection();

        private:
            int m_id;

            QByteArray createPackage(QString action, QString parameterValue, QString data);

            void proccessReadData(QStringList readData);
            bool validate(QStringList package);

            //explicit PressureSensor(const PressureSensor& rhs) = delete;
            //PressureSensor& operator= (const PressureSensor& rhs) = delete;
    };
}}}


