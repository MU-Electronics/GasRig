// Qt includes
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include <QtDebug>
#include <QTextStream>

// Include the template framework
#include "iconsimageprovider.h"
#include "iconthemeimageprovider.h"

// Require app
#include "bootstrap/Startup.h"


/**
 * Message handle to log to file for functions like qDebug when released
 *
 * @brief myMessageHandler
 * @param type
 * @param msg
 */
void fileMessageHandler(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
    QString txt;
    switch (type) {
        case QtDebugMsg:
            txt = QString("Debug: %1").arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
        break;
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
        break;
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
        break;
    }
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

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
    // Start app
    QGuiApplication app(argc, argv);

    // Attach message handler
    //qInstallMessageHandler(fileMessageHandler);

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

