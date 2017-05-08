#pragma once

namespace App { namespace Hardware { namespace HAL
{
    class LabJack
    {
    public:
        LabJack();

    private:
        explicit LabJack(const LabJack& rhs) = delete;
        LabJack& operator= (const LabJack& rhs) = delete;
    };
}}}


