#include "Startup.h"

// Qt libraries
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQmlContext>

// Libs prov to be used
#include <QStringListModel>
#include <QQuickItem>
#include <QQuickWindow>
#include <QDebug>
#include <QtGlobal>

// Include the template framework
#include "iconsimageprovider.h"
#include "iconthemeimageprovider.h"

namespace Bootstrap
{
    static auto SETTINGS_FILE = QStringLiteral("settings.json");

    Startup::Startup()
        : QObject(nullptr),
          m_engine (*new QQmlApplicationEngine()),
          m_isValid(true)
    {

        // Setup the styling
        if (QQuickStyle::name().isEmpty())
            QQuickStyle::setStyle(QLatin1String("Material"));

        // Load the template framework
        m_engine.addImportPath(QLatin1String("qrc:/"));
        m_engine.addImageProvider(QLatin1String("fluidicons"), new IconsImageProvider());
        m_engine.addImageProvider(QLatin1String("fluidicontheme"), new IconThemeImageProvider());

        // Load the main application qml file
        m_engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));
    }

    Startup::~Startup(){

    }

}
