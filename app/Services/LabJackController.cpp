#include "LabJackController.h"

// Include external deps
#include <QByteArray>

// Import correct labjack driver
#ifdef _WIN32
    // Using labjackUD library for windows as it uses WinUsb
    #include "vendor/labjack/windowsUD/LabJackUD.h"
#else
    // Using the exodriver for everything else as this uses usblib-1.0
    #include "vendor/labjack/exodriver/labjackusb.h"
#endif

namespace App { namespace Services
{

    LabJackController::LabJackController()
    {

    }


    /**
     * Open the first lab jack U3 on the PC
     *
     * @brief LabJackController::open
     * @return
     */
    bool LabJackController::open()
    {

        // Run the correct method using the correct library
        #ifdef _WIN32
            // Using labjackUD library
            OpenLabJack (LJ_dtU3, LJ_ctUSB, "1", 1, &device);
        #else
            // Using the exodriver
            device = LJUSB_OpenDevice(1, 0, U3_PRODUCT_ID);
        #endif
    }


    /**
     * Close the current labjack connection
     *
     * @brief LabJackController::close
     */
    void LabJackController::close()
    {
        // Run the correct close connection method using the correct library
        #ifdef _WIN32
            // Using labjackUD library
            Close();
        #else
            // Using the exodriver
            LJUSB_CloseDevice(device);
        #endif
    }


    /**
     * Read a packet of data from the labjack
     *
     * @brief LabJackController::read
     * @param length
     * @return
     */
    QByteArray LabJackController::read(int length)
    {
        // Hold the reads data
        unsigned char data;

        // Run the correct method using the correct library
        #ifdef _WIN32
            // Number of bytes to read
            double byteLength = (double) length;

            // Using labjackUD library
            eGetPtr(device, LJ_ioRAW_IN, 0, &byteLength, &data);
        #else
            // Number of bytes to be read
            unsigned long byteLength = (unsigned int) length;

            // Using the exodriver
            byteLength = LJUSB_Read(device, &data, byteLength);
        #endif

        // Format char into QByteArray
        return QByteArray::fromRawData((char*) data, byteLength);
    }


    /**
     * Write a packet of data to the labjack
     *
     * @brief LabJackController::write
     * @param package
     * @return
     */
    QByteArray LabJackController::write(QByteArray package)
    {
        // Convert QBtyeArray to char
        unsigned char data = *(unsigned char*)(package.data());

        // Get the size of the data
        int length = package.size();

        // Run the correct method using the correct library
        #ifdef _WIN32
            // Number of bytes to read
            double byteLength = (double) length;

            // Using labjackUD library
            eGetPtr(device, LJ_ioRAW_OUT, 0, &byteLength, &data);
        #else
            // Number of bytes to be read
            unsigned long byteLength = (unsigned int) length;

            // Using the exodriver
            byteLength = LJUSB_Write(device, &data, byteLength);
        #endif

        // Format char into QByteArray
        return QByteArray::fromRawData((char*) data, byteLength);
    }


}}
