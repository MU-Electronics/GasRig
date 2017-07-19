#pragma once

// Include external libs
#include <QDebug>

// Include state machine
#include "VacDown.h"

namespace App { namespace Experiment { namespace Machines
{

    class Machines
    {
        public:
            Machines(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety);

            void vacDown(int mintues, bool turbo, int gasMode, int mode);
            void stopVacDown();

        private:
            // Vac down machine
            VacDown& m_vacDown;
    };

}}}

