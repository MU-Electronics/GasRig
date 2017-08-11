#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "States/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    class VacDown    :   public States::MachineStates
    {
        Q_OBJECT

        public:
            VacDown(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety);

            ~VacDown();

            void setParams(int mintues, bool turbo, int gasMode, int mode);

            void start();

            void stop();

            void stopAsFailed();

            void buildMachine();

        signals:
            void emit_vacDownFinished(QVariantMap params);
            void emit_vacDownFailed(QVariantMap params);

        public slots:
            // Finishing sequence
            void finish();

        private:
            // Referance to QObject
            QObject *parent;

            // Holds the application settings
            Settings::Container m_settings;

    };
}}}


