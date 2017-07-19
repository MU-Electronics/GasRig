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

            void purgeSystemMethodOne(bool outputValve, int cycles, QString pressure);
            void purgeSystemMethodTwo(int minutes, QString pressure);
            void stopPurgeSystem();

            void exhuast(int frequency, int speed);
            void stopExhuast();

            void negativeTune(double pressure);
            void stopNegativeTune();

            void setHighPressure(double pressure, int input, int frequency);
            void stopSetHighPressure();

            void outputPressure(int frequency);
            void stopOutputPressure();

            void valveOpen(int id);
            void valveClose(int id);

            void setFlowRate(int flowController, double rate);
            void stopFlowRate(int flowController, double rate);

        private:
            // Vac down machine
            VacDown& m_vacDown;
    };

}}}

