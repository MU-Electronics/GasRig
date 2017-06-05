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


            /*
             * Helpers
             */
            // Convert 4 btyes to float
            union FourByteToFloat
            {
               unsigned char buf[4];
               float number;
            }FourByteToFloat;

        public slots:
            virtual void testConnection() = 0;
            virtual void resetConnection() = 0;
    };

}}}
