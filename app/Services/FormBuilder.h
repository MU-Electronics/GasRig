#pragma once

namespace App { namespace Services{
    class FormBuilder
    {
    public:
        FormBuilder();

    private:
        explicit FormBuilder(const FormBuilder& rhs) = delete;
        FormBuilder& operator= (const FormBuilder& rhs) = delete;
    };
}}


