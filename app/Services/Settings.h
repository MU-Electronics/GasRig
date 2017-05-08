#pragma once

namespace App { namespace Services{
    class Settings
    {
    public:
        Settings();

    private:
        explicit Settings(const Settings& rhs) = delete;
        Settings& operator= (const Settings& rhs) = delete;
    };
}}


