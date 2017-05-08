#pragma once
#include <QObject>
#include "app/Services/SerialWrapper.h"


namespace App { namespace Hardware { namespace HAL
{
    class PressureSensor : public QObject
    {
        Q_OBJECT
    public:
        explicit PressureSensor(QObject *parent=0);

    private:
        explicit PressureSensor(const PressureSensor& rhs) = delete;
        PressureSensor& operator= (const PressureSensor& rhs) = delete;
    };
}}}


