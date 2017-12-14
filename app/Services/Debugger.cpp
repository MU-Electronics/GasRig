#include "Debugger.h"
#include <QFile>
#include <QtDebug>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QLoggingCategory>
#include <QtGlobal>

namespace App { namespace Services
{

    /**
     * Constructor for debugger
     *    - sets up the class read for handling debugging messages
     *
     * @brief Debugger::Debugger
     */
    Debugger::Debugger()
    {
        // Always assum setup failed
        success = false;

        // Setup instance
        setup();
    }

    /**
     * Return instance of class (singlton)
     *
     * @brief Debugger::getInstance
     * @return
     */
    Debugger& Debugger::getInstance()
    {
        // Make thread safe
        static QMutex mutex;
        QMutexLocker lock(&mutex);

        static Debugger instance;
        return instance;
    }


    /**
     * Returns whether the setup was successfull or not
     *
     * @brief Debugger::wasSuccess
     * @return
     */
    bool Debugger::wasSuccess()
    {
        return Debugger::getInstance().success;
    }


    /**
     * Provides a static inteface for the debug handler
     *
     * @brief Debugger::handlerAccessor
     * @param type
     * @param context
     * @param msg
     */
    void Debugger::handlerAccessor(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        return Debugger::getInstance().handler(type, context, msg);
    }



    /**
     * Debug message handler
     *
     * @brief logOutputHandler
     * @param type
     * @param context
     * @param msg
     */
    void Debugger::handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        // Define message container
        QString message;

        // State debug message
        message = "######## Log Message ########\n";

        // Get the current date and time
        message += "[Timestamp] " + QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") + "\n";

        // Append the catergory
        message += "[Category] " + QString::fromUtf8(context.category) + "\n";

        // Append the version number
        message += "[Version] " + QString::number(context.version) + "\n";

        // Append the file
        message += "[File] " + QString::fromUtf8(context.file) + "\n";

        // Append the function
        message += "[Function] " + QString::fromUtf8(context.function) + "\n";

        // Append the line number
        message += "[Line] " + QString::number(context.line) + "\n";

        // Get error type
        switch (type)
        {
            case QtInfoMsg:
                message += "[Type] Information \n";
                break;
            case QtDebugMsg:
                message += "[Type] Debug \n";
                break;
            case QtWarningMsg:
                message += "[Type] Warning \n";
                break;
            case QtCriticalMsg:
                message += "[Type] Critical \n";
                break;
            case QtFatalMsg:
                message += "[Type] Fatal \n";
        }

        // Append the message
        message += "[Message] " + msg;

        // Make thread safe
        static QMutex mutex;
        QMutexLocker lock(&mutex);

        // Output to log file
        (*file) << message << "\n\n" << endl;

        // Output to console
        (*console) << message << "\n\n" << endl;

        // If fatel error abort the application
        if (QtFatalMsg == type)
        {
            abort();
        }
    }


    void Debugger::logPaths()
    {
        // Get the current date and time and but the log on the end
        logFileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") + ".log";

        // Get the OS app data location
        logLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/logs";
    }


    /**
     * Setup the debugger class
     *
     * @brief Debugger::setup
     */
    void Debugger::setup()
    {
        // Setup log location
        logPaths();

        // Create the app data location is it does not exist
        if(!QDir(logLocation).exists())
        {
            QDir().mkdir(logLocation);
        }

        // Open the file
        QFile *log = new QFile(logLocation + "/" + logFileName);

        // Was the file able to be opened with write access?
        if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            // Create log file streaming instance
            file = new QTextStream(log);

            // Create console streaming instance
            console = new QTextStream( stdout );

            // State success
            success = true;
        }
        else
        {
            // Log file could not be opened, log error to console if one open
            qDebug() << "Error opening log file '" << logFileName << "'" << " In location '" << logLocation << "' .All debug outputs will be redirected to console.";
        }
    }

}}
