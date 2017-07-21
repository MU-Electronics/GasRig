#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class SensorReadings    :   public MachineStates
    {
        Q_OBJECT

        public:
            SensorReadings(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~SensorReadings();

            void setParams(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_sensorReadingsStopped(QVariantMap params);
            void emit_sensorReadingsFailed(QVariantMap params);

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

    };
}}}


