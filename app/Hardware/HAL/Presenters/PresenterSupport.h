#pragma once

#include <QStringList>
#include <QVariantMap>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    class PresenterSupport
    {
        public:
            PresenterSupport();

            bool validatePackageSize(QStringList package, int size);

            QVariantMap extend(QVariantMap commands, QVariantMap package);
            QVariantMap commandId(QVariantMap commands, QVariantMap package);
            QVariantMap methodOverride(QVariantMap commands, QVariantMap package);

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
