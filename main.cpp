// Qt includes
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QtDebug>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>

// Include the template framework
#include "iconsimageprovider.h"
#include "iconthemeimageprovider.h"

// Require app
#include "bootstrap/Startup.h"


/**
 * Message handler for debugging output
 */
#ifndef QT_NO_DEBUG_OUTPUT

    // Stream to text file
    QTextStream *out = 0;

    // Message handler
    void logOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
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
        (*out) << debugdate << " " << msg << endl;

        // If fatel error abort the application
        if (QtFatalMsg == type)
        {
            abort();
        }
    }

#endif



/**
 * Main applcation function
 *
 * @brief qMain
 * @param argc
 * @param argv
 * @return Qt application event loop
 */
int main(int argc, char *argv[])
{
    // Set
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    // Start app
    QApplication app(argc, argv);

    // Attach message handler
    #ifndef QT_NO_DEBUG_OUTPUT
        // Get the current date and time and but the log on the end
        QString logFileName = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss") + ".log";

        // Get the OS app data location
        QString logLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

        // Create the app data location is it does not exist
        if(!QDir(logLocation).exists())
        {
            QDir().mkdir(logLocation);
        }

        qDebug() << logLocation << logFileName;

        // Open the file
        QFile *log = new QFile(logLocation + "/" + logFileName);

        // Was the file able to be opened with write access?
        if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
        {
            // Create the streaming instance
            out = new QTextStream(log);

            // Attach the custom message handler to Qt
            qInstallMessageHandler(logOutput);
        }
        else
        {
            // Log file could not be opened, log error to console if one open
            qDebug() << "Error opening log file '" << logFileName << "'" << " In location '" << logLocation << "' .All debug outputs will be redirected to console.";
        }

    #endif

    // Boot the applcation
    Bootstrap::Startup loader;

    // If successfull return event loop
    if (loader.success())
    {
       return app.exec();
    }
    qDebug() << "Applcation could not load!";
    // End app
    return -1;
}

