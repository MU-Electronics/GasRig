#pragma once

// Include external deps
#include <QByteArray>

// Import correct labjack driver
#ifdef _WIN32
    #include "vendor/labjack/windowsUD/LabJackUD.h"
#else
    #include "vendor/labjack/exodriver/labjackusb.h"
#endif


namespace App { namespace Services
{

    class LabJackController
    {

        public:
            LabJackController();

            #ifdef _WIN32
                // Using labjackUD library
                LJ_HANDLE device;
            #else
                // Using the exodriver
                HANDLE device;
            #endif


            bool open();
            void close();
            QByteArray read(int length);
            QByteArray write(QByteArray package);
    };

}}
