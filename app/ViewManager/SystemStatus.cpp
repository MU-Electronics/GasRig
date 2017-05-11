#include "SystemStatus.h"
#include <QVariantMap>
#include <QString>

namespace App { namespace ViewManager
{
    SystemStatus::SystemStatus(QObject *parent, QQmlApplicationEngine *root)
        : QObject(parent),
          m_root(root)
    {
        // Init default value state
        for(int i = 0; i <= 9; i++){
            m_valves.insert(QString(i), false);
        }
    }

    SystemStatus::~SystemStatus()
    {

    }

}}


