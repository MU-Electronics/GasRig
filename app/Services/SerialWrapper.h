#pragma once

namespace App { namespace Services
{
    class SerialController
    {
    public:
        SerialController();

    private:
        explicit SerialController(const SerialController& rhs) = delete;
        SerialController& operator= (const SerialController& rhs) = delete;
    };
}}


