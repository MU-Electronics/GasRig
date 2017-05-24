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

        private slots:
            // Required slot for full HAL Support compliance
            void testConnection();
            void resetConnection();

        private:
            void proccessReadData(QString readData);
            bool validate(QString package);

            explicit FlowController(const FlowController& rhs) = delete;
            FlowController& operator= (const FlowController& rhs) = delete;
    };
}}}


