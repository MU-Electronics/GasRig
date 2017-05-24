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

            void setParams(QVariantMap command);

        public slots:
            virtual void testConnection() = 0;
            virtual void resetConnection() = 0;
    };

}}}
