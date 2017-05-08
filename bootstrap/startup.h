#pragma once

namespace Bootstrap{
    class Startup
    {
    public:
        Startup();

    private:
        explicit Startup(const Startup& rhs) = delete;
        Startup& operator= (const Startup& rhs) = delete;
    };
}


