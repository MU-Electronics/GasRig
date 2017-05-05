#pragma once
#include <QWidget>

namespace App { namespace Hardware
{
    class Access
    {
    public:
        Access();

    private:
        explicit Access(const Access& rhs) = delete;
        Access& operator= (const Access& rhs) = delete;
    };
}}


