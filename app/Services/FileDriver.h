#pragma once

namespace App { namespace Services
{
    class FileDriver
    {
    public:
        FileDriver();

    private:
        explicit FileDriver(const FileDriver& rhs) = delete;
        FileDriver& operator= (const FileDriver& rhs) = delete;
    };
}}


