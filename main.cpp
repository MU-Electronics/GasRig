// Qt includes
#include <QGuiApplication>
#include <QQmlApplicationEngine>

// Include the template framework
#include "iconsimageprovider.h"
#include "iconthemeimageprovider.h"


/**
 * Main applcation function
 *
 * @brief qMain
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    // Load the template framework
    engine.addImportPath(QLatin1String("qrc:/"));
    engine.addImageProvider(QLatin1String("fluidicons"), new IconsImageProvider());
    engine.addImageProvider(QLatin1String("fluidicontheme"), new IconThemeImageProvider());

    // Load the main application qml file
    engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));

    return app.exec();
}
