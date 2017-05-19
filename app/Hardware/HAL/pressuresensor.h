#pragma once
#include <QObject>

#include "../../Services/SerialController.h"


namespace App { namespace Hardware { namespace HAL
{
    class PressureSensor
    {
    public:
        explicit PressureSensor(QObject *parent=0);

    private:
        explicit PressureSensor(const PressureSensor& rhs) = delete;
        PressureSensor& operator= (const PressureSensor& rhs) = delete;
    };
}}}


