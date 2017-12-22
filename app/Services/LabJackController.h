#pragma once

// Include external deps
#include <QByteArray>
#include <QVariantMap>

// Debugging catergories
#include "../Services/Debuging.h"

// Import correct labjack driver
#ifdef _WIN32
    #include "vendor/labjack/windowsUD/LabJackUD.h"
#else
    #include "vendor/labjack/exodriver/labjackusb.h"
#endif


namespace App { namespace Services
{

    class LabJackController: public QObject
    {

        Q_OBJECT

        public:
            explicit LabJackController(QObject *parent = 0);
            ~LabJackController();

            // What is the class responsable for?
            QString m_responsability;

            // Which method is being ran
            QString m_method;

            // Is the bus open
            bool isOpen;

            #ifdef _WIN32
                // Using labjackUD library
                LJ_HANDLE device;
            #else
                // Using the exodriver
                HANDLE device;
            #endif

            QMap<QString, QString> customise;


            bool open();
            void close();
            QStringList read(int length);
            bool write(QByteArray package);


            bool validate(QString type, QStringList package);

            QByteArray createPackageFeedback(QStringList data);
            QStringList sendReceivePackage(QString type, QStringList data, int receivedBytes);

            int portValueFromName(QString name);

            QString checkSumEight(QStringList package);
            QString checkSumSixteen(QStringList package);

            QVariantMap errorPackageGenerator(QString com, QString port, QString error);
            QVariantMap comConnectionPackageGenerator(QString com, bool status);

    signals:
        void emit_critialLabJackError(QVariantMap errorPackage);
        void emit_comConnectionStatus(QVariantMap package);
        void emit_timeoutLabJackError(QVariantMap errorPackage);

    };

}}
