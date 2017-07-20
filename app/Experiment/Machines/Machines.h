#pragma once

// Include external libs
#include <QDebug>

// Include state machine
#include "VacDown.h"
#include "SafeValve.h"

namespace App { namespace Experiment { namespace Machines
{

    class Machines  :   public QObject
    {
        Q_OBJECT

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

        signals:
            void emit_vacDownMachineStarted(int mintues, bool turbo, int gasMode, int mode);
            void emit_vacDownMachineStopped();

            void emit_safeValveMachineStarted(int id, bool state);

            void emit_purgeSystemMachineState(bool state);

        public slots:
            void vacDownFinished(QVariantMap params);

        private:
            // Hold the machines
            VacDown& m_vacDown;
            SafeValve& m_safeValve;

    };

}}}

