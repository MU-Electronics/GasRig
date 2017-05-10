#include "Testing.h"
#include <QDebug>
#include <QQuickView>
#include <QUrl>

namespace App { namespace ViewManager
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root)
        : QObject(parent),
          m_root(root)
    {
        exampleVar("Hello from C++");
    }

    void Testing::vacuumOn()
    {
        qDebug() << "Turn Vacuum On";
        exampleVar("updated");
    }
}}

