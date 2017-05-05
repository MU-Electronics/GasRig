#pragma once
#include <QWidget>

namespace App { namespace Safety
{
    class Monitor
    {
    public:
        Monitor();

    private:
        explicit Monitor(const Monitor& rhs) = delete;
        Monitor& operator= (const Monitor& rhs) = delete;
    };
}}


