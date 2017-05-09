#include "ConnectionStatus.h"

namespace App { namespace ViewManager
{
    ConnectionStatus::ConnectionStatus(QObject *parent, QObject *root)
        : QObject(parent),
          m_root(root)
    {

    }
}}
