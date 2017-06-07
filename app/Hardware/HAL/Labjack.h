#pragma once

// Include external deps
#include <QObject>

// Include labjack controller making universal cross OS platform
#include "../../Services/LabJackController.h"


namespace App { namespace Hardware { namespace HAL
{
    class LabJack: public App::Services::LabJackController
    {
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

    private:
        //explicit LabJack(const LabJack& rhs) = delete;
        //LabJack& operator= (const LabJack& rhs) = delete;
    };
}}}


