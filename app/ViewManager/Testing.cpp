#include "Testing.h"
#include <QDebug>
#include <QQuickView>
#include <QUrl>
#include <QString>

// Include settings container
#include "../Settings/container.h"

namespace App { namespace ViewManager
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        exampleVar("Hello from C++");
    }

    void Testing::requestVacuum(bool onOff)
    {
        qDebug() << "Turn Vacuum is: " << onOff;
        exampleVar(m_settings.general.admin["username"].toString());
    }
}}

