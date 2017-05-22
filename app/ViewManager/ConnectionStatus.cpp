#include "ConnectionStatus.h"
#include <iostream>

// Include settings container
#include "../Settings/container.h"
// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

namespace App { namespace ViewManager
{
    ConnectionStatus::ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        exampleVar("Hello from C++");
    }

    void ConnectionStatus::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {

    }

    void ConnectionStatus::helloWorld()
    {
        std::cout << "Hey" << std::endl;
    }
}}

