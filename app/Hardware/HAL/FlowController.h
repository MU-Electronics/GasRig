#pragma once

#include <QObject>
#include <QString>

// Include serial controller
#include "../../Services/SerialController.h"

// Interface for HAL classes
#include "HALSupport.h"

namespace App { namespace Hardware { namespace HAL
{
    class FlowController: public App::Services::SerialController, public HALSupport
    {
        Q_OBJECT
        public:
            FlowController(QObject *parent);

            void setId(int id);

        private slots:
            // Required slot for full HAL Support compliance
            void testConnection();
            void resetConnection();

        private:
            int m_id;

            void proccessReadData(QStringList readData);
            bool validate(QStringList package);
            QString calculateCheckSum(QStringList package);

            explicit FlowController(const FlowController& rhs) = delete;
            FlowController& operator= (const FlowController& rhs) = delete;
    };
}}}


