#pragma once
#include <QWidget>

namespace App { namespace ViewManager
{
    class View
    {
    public:
        View();

    private:
        explicit View(const View& rhs) = delete;
        View& operator= (const View& rhs) = delete;
    };
}}


