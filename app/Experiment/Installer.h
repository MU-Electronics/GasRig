#pragma once

namespace App { namespace Experiment{
    class Installer
    {
    public:
        Installer();

    private:
        explicit Installer(const Installer& rhs) = delete;
        Installer& operator= (const Installer& rhs) = delete;
    };
}}


