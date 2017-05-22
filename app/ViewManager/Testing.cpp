#include "Testing.h"

#include <QObject>
#include <QDebug>
#include <QVariantMap>
#include <QString>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace ViewManager
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        exampleVar("Hello from C++");
    }


    void Testing::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Testing::vacStationPump, &hardware, &Hardware::Access::hardwareAccess);
    }


    void Testing::requestVacuum(bool onOff)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "SetPumpingState");
        command.insert("state", onOff);

        // Emit siganl to HAL
        emit vacStationPump(command);
    }
}}

