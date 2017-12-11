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
#include "Vent.h"
#include "Purge.h"

namespace App { namespace Experiment { namespace Machines
{

    class Machines  :   public QObject
    {
        Q_OBJECT

        public:
            Machines(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety);

            void stateMachinesToThread();

            int vacDown(double value, int valueType, bool turbo, int gasMode, int mode);
            void stopVacDown();

            int sensorReadings(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter, int turboSpeedTimeInter, int vacStationTemperTimeInter, int flowControlTempTimeInter);
            void stopSensorReadings();

            int purge(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo);
            void stopPurge();

            int vent(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);
            void stopVent();

            int pulseValve(int valve, int cycles, int timeOpen, int timeClosed);
            void stopPulseValve();

            int setPressure(double pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve);
            void stopSetPressure();

            int valveOpen(int id);
            int valveClose(int id);

            int setFlowRate(int flowController, double rate);
            void stopFlowRate();

        signals:
            void emit_vacDownMachineStarted(double value, int valueType, bool turbo, int gasMode, int mode);
            void emit_vacDownMachineStopping();
            void emit_vacDownMachineStopped();

            void emit_safeValveMachineStarted(int id, bool state);

            void emit_pulseValveStarted(int valve, int cycles, int timeOpen, int timeClosed);
            void emit_pulseValveStopping();
            void emit_pulseValveStopped();

            void emit_sensorReadingsMachineStarted(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter, int turboSpeedTimeInter, int vacStationTemperTimeInter, int flowControlTempTimeInter);
            void emit_sensorReadingsMachineStopped();
            void emit_sensorsNotBeingMonitored();

            void emit_pressuriseStarted(double pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve);
            void emit_pressuriseStopping();
            void emit_pressuriseStopped();

            void emit_ventMachineStarted(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);
            void emit_ventMachineStopping();
            void emit_ventMachineStopped();

            void emit_purgeStarted(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo);
            void emit_purgeStopping();
            void emit_purgeStopped();

        public slots:
            void vacDownStopping(QVariantMap params);
            void vacDownFinished(QVariantMap params);
            void vacDownFailed(QVariantMap params);

            void valveStateFinished(QVariantMap params);
            void valveStateFailed(QVariantMap params);

            void pulseValveStopping(QVariantMap params);
            void pulseValveFinished(QVariantMap params);
            void pulseValveFailed(QVariantMap params);

            void sensorReadingsFinished(QVariantMap params);
            void sensorReadingsFailed(QVariantMap params);

            void pressuriseStopping(QVariantMap params);
            void pressuriseFinished(QVariantMap params);
            void pressuriseFailed(QVariantMap params);

            void ventStopping(QVariantMap params);
            void ventFinished(QVariantMap params);
            void ventFailed(QVariantMap params);

            void purgeStopping(QVariantMap params);
            void purgeFinished(QVariantMap params);
            void purgeFailed(QVariantMap params);

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
            Vent& m_vent;
            Purge& m_purge;

            // Sensor reading thread
            QThread thread_sensorReadings;

            // Error
            int machineFailedToStart(int errorCode);

    };

}}}

