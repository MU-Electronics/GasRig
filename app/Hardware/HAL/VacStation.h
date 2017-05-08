#pragma once

namespace App { namespace Hardware { namespace HAL
{
    class VacStation
    {
    public:
        VacStation();

    private:
        explicit VacStation(const VacStation& rhs) = delete;
        VacStation& operator= (const VacStation& rhs) = delete;
    };
}}}


