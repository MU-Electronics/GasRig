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
#include <QLoggingCategory>

// Include the template framework
#include "iconsimageprovider.h"
#include "iconthemeimageprovider.h"

// Require app
#include "bootstrap/Startup.h"

// Debugger
#include "app/Services/Debugger.h"

// Debugging catergories
#include "app/Services/Debuging.h"


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

    // Install the debugger
    if(App::Services::Debugger::getInstance().wasSuccess())
        qInstallMessageHandler(App::Services::Debugger::handlerAccessor);

    // Boot the applcation
    Bootstrap::Startup loader;

    // If successfull return event loop
    if (loader.success())
    {
        // State app loaded
        qInfo() << "Application loaded";

        // Run event loop
        return app.exec();
    }

    // Log error
    qCritical() << "Application could not load!";

    // End app
    return -1;
}

