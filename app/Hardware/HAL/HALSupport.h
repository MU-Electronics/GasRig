#pragma once

#include <QObject>
#include <QVariantMap>

namespace App { namespace Hardware { namespace HAL
{

    class HALSupport
    {
        public:
            HALSupport(QObject *parent = 0);

            QVariantMap m_command;

            bool setParams(QVariantMap command);
    };

}}}
