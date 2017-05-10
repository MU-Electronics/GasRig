#include "Testing.h"
#include <iostream>

namespace App { namespace ViewManager
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root)
        : QObject(parent),
          m_root(root)
    {
        exampleVar("Hello from C++");
    }

    void Testing::helloWorld()
    {
        std::cout << "Hey" << std::endl;
    }
}}

