#include "MessageHandler.h"

// Qt libraries
#include <QtGlobal>
#include <QDebug>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QString>

namespace Bootstrap
{


    /**
     *
     *
     * @brief MessageHandler::MessageHandler
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
//    MessageHandler::MessageHandler()
//    {

//    }


//    /**
//     * Destroy anything still exsisiting
//     *
//     * @brief MessageHandler::~MessageHandler
//     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
//     */
//    MessageHandler::~MessageHandler()
//    {

//    }



    void MessageHandler::handleMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        if (currentHandler != nullptr)
            currentHandler->messageOutputHandler(type, context, msg);
    }


    /**
     * Sets up and install
     *
     * @brief MessageHandler::installMessageHandler
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void MessageHandler::installMessageHandler()
    {
        currentHandler = this;

        // Get the current date and time and but the log on the end
        QString logFileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") + ".log";

        // Get the OS app data location
        QString logLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

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
            // Create the streaming instance
            out = new QTextStream(log);

             *(out) << 1;

            // Attach the custom message handler to Qt
            qInstallMessageHandler(handleMessages);
        }
        else
        {
            // Log file could not be opened, log error to console if one open
            qDebug() << "Error opening log file '" << logFileName << "'" << " In location '" << logLocation << "' .All debug outputs will be redirected to console.";
        }
    }



    /**
     * debug message handler
     *
     * @brief logOutputHandler
     * @param type
     * @param context
     * @param msg
     */
    void MessageHandler::messageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        // Get the current date and time
        QString debugdate = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");

        // Get error type
        switch (type)
        {
            case QtDebugMsg:
                debugdate += " [Debug] ";
                break;
            case QtWarningMsg:
                debugdate += " [Warning] ";
                break;
            case QtCriticalMsg:
                debugdate += " [Critical] ";
                break;
            case QtFatalMsg:
                debugdate += " [Fatal] ";
        }

        // Make thread safe
        static QMutex mutex;
        QMutexLocker lock(&mutex);

        // Compile the error message and output it to the log file
        *(out) << debugdate << " " << msg << endl;

        // If fatel error abort the application
        if (QtFatalMsg == type)
        {
            abort();
        }
    }
}

