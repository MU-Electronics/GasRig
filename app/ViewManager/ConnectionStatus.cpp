#include "ConnectionStatus.h"
#include <iostream>

// Include settings container
#include "../Settings/container.h"

namespace App { namespace ViewManager
{
    ConnectionStatus::ConnectionStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
        exampleVar("Hello from C++");
    }

    ConnectionStatus::~ConnectionStatus()
    {

    }

    void ConnectionStatus::helloWorld()
    {
        std::cout << "Hey" << std::endl;
    }
}}

