#pragma once

// Include external deps
#include <QObject>

// Include labjack controller making universal cross OS platform
#include "../../Services/LabJackController.h"

// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class LabJack: public App::Services::LabJackController, public HALSupport
    {

        Q_OBJECT

        public:
            LabJack(QObject *parent);


        signals:
            void emit_labJackData(QString responable, QString m_method, QStringList halData);

        public slots:
            void configureIO();

            void setPortDirection();
            void setDigitalPort();
            void setAnaloguePort();

            void readPortDirection();
            void readDigitalPort();
            void readAnaloguePort();

            // Required slots for full HAL Support compliance
            void testConnection();
            void resetConnection();

        private:
            bool validate(QString type, QStringList package);

            QByteArray createPackageFeedback(QStringList data);
            QStringList sendReceivePackage(QString type, QStringList data, int receivedBytes);
        //explicit LabJack(const LabJack& rhs) = delete;
        //LabJack& operator= (const LabJack& rhs) = delete;
    };
}}}


