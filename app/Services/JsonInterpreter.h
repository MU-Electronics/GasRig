#pragma once

namespace App { namespace Services{
    class JsonInterpreter
    {
    public:
        JsonInterpreter();

    private:
        explicit JsonInterpreter(const JsonInterpreter& rhs) = delete;
        JsonInterpreter& operator= (const JsonInterpreter& rhs) = delete;
    };
}}


