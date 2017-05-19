#pragma once

#include <QObject>

namespace App { namespace Hardware { namespace HAL
{
    class FlowController
    {
    public:
        FlowController(QObject *parent);

    private:
        explicit FlowController(const FlowController& rhs) = delete;
        FlowController& operator= (const FlowController& rhs) = delete;
    };
}}}


