#pragma once

#include <QObject>

namespace App { namespace Hardware { namespace HAL
{
    class LabJack
    {
    public:
        LabJack(QObject *parent);

    private:
        explicit LabJack(const LabJack& rhs) = delete;
        LabJack& operator= (const LabJack& rhs) = delete;
    };
}}}


