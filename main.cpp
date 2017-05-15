// Qt includes
#include <QApplication>
#include <QQmlApplicationEngine>

// Include the template framework
#include "iconsimageprovider.h"
#include "iconthemeimageprovider.h"

// Require app
#include "bootstrap/Startup.h"

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

    // Boot the applcation
    Bootstrap::Startup loader;

    // If successfull return event loop
    if (loader.success())
    {
       return app.exec();
    }

    // End app
    //return -1;
}

