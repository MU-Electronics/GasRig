#include "SystemStatus.h"
#include <QVariantMap>
#include <QString>
#include <QtGlobal>

// Include settings container
#include "../Settings/container.h"

namespace App { namespace ViewManager
{
    SystemStatus::SystemStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        // Init default value state
        for(int i = 0; i <= 9; i++){
            m_valves.insert(QString(i), false);
        }

        // Request current values for valves
        //emit valveStatus(0);

        // Set default pressure values
        for(int i = 0; i <= 9; i++){
            m_pressure.append(0);
        }

    }

    SystemStatus::~SystemStatus()
    {

    }

}}


