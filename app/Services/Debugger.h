#pragma once

#include <QFile>
#include <QtDebug>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QLoggingCategory>

namespace App { namespace Services
{
    class Debugger
    {
        public:
            // Return the instance of the singleton Debugger
            static Debugger& getInstance();

            void static handlerAccessor(QtMsgType type, const QMessageLogContext &context, const QString &msg);
            bool static wasSuccess();

        private:
            // Make constructer and destructure private as this class is singlton
            Debugger();
            ~Debugger() {}

            bool success = false;

            // Stream to the log file
            QTextStream *file;
            QTextStream *console;

            // Path locations to the log file
            QString logFileName;
            QString logLocation;

            // Setup the path locations
            void logPaths();

            // Delete old log files

            // Sets up the messaage
            void setup();

            // The handler used for Qts debugger framework
            void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

            // Ensure the class is not copied
            Debugger(const Debugger&);
            const Debugger& operator=(const Debugger&);
    };
}}


