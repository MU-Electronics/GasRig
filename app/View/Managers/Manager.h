#pragma once

// Debugging catergories
#include "../../Services/Debuging.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Disable compiler warining for un-used vars
#define UNUSED( x ) ( &reinterpret_cast< const int& >( x ) )


namespace App { namespace View { namespace Managers
{
    class Manager
    {

        virtual void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety) = 0;

    };
}}}


