#pragma once
#include <QWidget>

namespace App { namespace Experiment{
    class Loader
    {
    public:
        Loader();

    private:
        explicit Loader(const Loader& rhs) = delete;
        Loader& operator= (const Loader& rhs) = delete;
    };
}}


