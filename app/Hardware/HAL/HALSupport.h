#pragma once

#include <QObject>
#include <QVariantMap>

// Debugging catergories
#include "../../Services/Debuging.h"

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
            union FourByteFloatConvertion
            {
                float number;
                unsigned char buf[4];
            }FourByteFloatConvertion;

        public slots:
            virtual void testConnection() = 0;
            virtual void resetConnection() = 0;
    };

}}}
