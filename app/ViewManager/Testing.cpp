#include "Testing.h"
#include <QDebug>
#include <QQuickView>
#include <QUrl>
#include <QString>

namespace App { namespace ViewManager
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root)
        : QObject(parent),
          m_root(root)
    {
        exampleVar("Hello from C++");
    }

    void Testing::requestVacuum(bool onOff)
    {
        qDebug() << "Turn Vacuum is: " << onOff;
        exampleVar("updated");
    }
}}

