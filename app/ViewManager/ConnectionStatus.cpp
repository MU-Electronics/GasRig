#include "ConnectionStatus.h"
#include <iostream>

namespace App { namespace ViewManager
{
    ConnectionStatus::ConnectionStatus(QObject *parent, QQmlApplicationEngine *root)
        : QObject(parent),
          m_root(root)
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

