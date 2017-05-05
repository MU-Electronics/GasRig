#pragma once
#include <QWidget>

namespace App { namespace Experiment
{
    class Engine
    {
    public:
        Engine();

    private:
        explicit Engine(const Engine& rhs) = delete;
        Engine& operator= (const Engine& rhs) = delete;
    };
}}


