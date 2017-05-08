#pragma once

namespace App { namespace Hardware { namespace HAL
{
    class FlowController
    {
    public:
        FlowController();

    private:
        explicit FlowController(const FlowController& rhs) = delete;
        FlowController& operator= (const FlowController& rhs) = delete;
    };
}}}


