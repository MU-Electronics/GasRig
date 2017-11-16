#pragma once

// Include external libs
#include <QDebug>
#include <QThread>

// Include state machine
#include "VacDown.h"
#include "SafeValve.h"
#include "ReadFlowControllerFlow.h"
#include "ReadPressure.h"
#include "ReadVacuum.h"
#include "ReadFlowControllerTemperatures.h"
#include "ReadTurboSpeed.h"
#include "ReadVacStationTemperatures.h"
#include "PulseValve.h"
#include "Pressurise.h"

namespace App { namespace Experiment { namespace Machines
{

    class Machines  :   public QObject
    {
        Q_OBJECT

        public:
            Machines(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety);

            void stateMachinesToThread();

            int vacDown(int mintues, bool turbo, int gasMode, int mode);
            void stopVacDown();

            int sensorReadings(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter, int turboSpeedTimeInter, int vacStationTemperTimeInter, int flowControlTempTimeInter);
            void stopSensorReadings();

            int purgeSystemMethodOne(bool outputValve, int cycles, QString pressure);
            int purgeSystemMethodTwo(int minutes, QString pressure);
            void stopPurgeSystem();

            int exhuast(double pressure, int frequency, int speed);
            void stopExhuast();

            int pulseValve(int valve, int cycles, int timeOpen, int timeClosed);
            void stopPulseValve();

            int setPressure(double pressure, bool initVacDown, int stepSize, bool inputValve);
            void stopSetPressure();

            int outputPressure(int frequency);
            void stopOutputPressure();

            int valveOpen(int id);
            int valveClose(int id);

            int setFlowRate(int flowController, double rate);
            void stopFlowRate();

        signals:
            void emit_vacDownMachineStarted(int mintues, bool turbo, int gasMode, int mode);
            void emit_vacDownMachineStopped();

            void emit_safeValveMachineStarted(int id, bool state);

            void emit_pulseValveStarted(int valve, int cycles, int timeOpen, int timeClosed);
            void emit_pulseValveStopped();

            void emit_sensorReadingsMachineStarted(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter, int turboSpeedTimeInter, int vacStationTemperTimeInter, int flowControlTempTimeInter);
            void emit_sensorReadingsMachineStopped();
            void emit_sensorsNotBeingMonitored();

            void emit_purgeSystemMachineState(bool state);

            void emit_pressuriseStarted(double pressure, bool initVacDown, int stepSize, bool inputValve);
            void emit_pressuriseStopped();

        public slots:
            void vacDownFinished(QVariantMap params);
            void vacDownFailed(QVariantMap params);

            void valveStateFinished(QVariantMap params);
            void valveStateFailed(QVariantMap params);

            void pulseValveFinished(QVariantMap params);
            void pulseValveFailed(QVariantMap params);

            void sensorReadingsFinished(QVariantMap params);
            void sensorReadingsFailed(QVariantMap params);

            void pressuriseFinished(QVariantMap params);
            void pressuriseFailed(QVariantMap params);

        private:
            // Are the sensors being monitored?
            bool sensorMonitors = false;

            // Hold the machines
            VacDown& m_vacDown;
            SafeValve& m_safeValve;
            ReadFlowControllerFlow& m_readFlowControllerFlow;
            ReadPressure& m_readPressure;
            ReadVacuum& m_readVacuum;
            ReadFlowControllerTemperatures& m_readFlowControllerTemperatures;
            ReadTurboSpeed& m_readTurboSpeed;
            ReadVacStationTemperatures& m_readVacStationTemperatures;
            PulseValve& m_pulseValve;
            Pressurise& m_pressurise;

            // Sensor reading thread
            QThread thread_sensorReadings;

            // Error
            int machineFailedToStart(int errorCode);

    };

}}}

