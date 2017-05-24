#pragma once
#include <QObject>

// Include serial controller
#include "../../Services/SerialController.h"

// Interface for HAL classes
#include "HALSupport.h"


namespace App { namespace Hardware { namespace HAL
{
    class PressureSensor: public App::Services::SerialController, public HALSupport
    {
        Q_OBJECT
        public:
            explicit PressureSensor(QObject *parent=0);

        private:
            void proccessReadData(QString readData);
            bool validate(QString package);

            explicit PressureSensor(const PressureSensor& rhs) = delete;
            PressureSensor& operator= (const PressureSensor& rhs) = delete;
    };
}}}


