#pragma once

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

namespace App { namespace View { namespace Managers
{
    class Manager
    {

        virtual void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety) = 0;

    };
}}}


