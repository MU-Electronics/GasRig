#pragma once

#include <QStringList>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class PresenterSupport
    {
        public:
            PresenterSupport();

            bool validatePackageSize(QStringList package, int size);

            // Convert 4 btyes to float
            union FourByteFloatConvertion
            {
                float number;
                unsigned char buf[4];
            }FourByteFloatConvertion;

            // Convert 3 btyes to float
            union ThreeByteFloatConvertion
            {
                float number;
                unsigned char buf[3];
            }ThreeByteFloatConvertion;

    };

}}}}
