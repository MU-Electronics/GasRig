#pragma once

// Include extenral deps
#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QTimer>

// Include functions helper
#include "FunctionsHelper.h"

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include command validator states
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace Functions
{
    class Vacuum : public QObject, public FunctionsHelper
    {
        Q_OBJECT

        public:
            Vacuum(QObject* parent, Settings::Container* settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap& params, Hardware::CommandConstructor& commandConstructor);
            ~Vacuum();


            // Holds the application settings
            Settings::Container* m_settings;

            // Hold the hardware gateway
            Hardware::Access &m_hardware;

            // Hold the safety monitor
            Safety::Monitor &m_safety;

            // Hold the state machine
            QStateMachine &m_machine;

            // Hold params for states
            QVariantMap &m_params;

            // Hold instance of command constructor
            Hardware::CommandConstructor& m_commandConstructor;

            void connectStatesToMethods();

            // Error container
            QVariantMap errorDetails;

        signals:
            void hardwareRequest(QVariantMap command);

            void emit_validationFailed(QVariantMap error);
            void emit_validationSuccess(QVariantMap data);
            void emit_validationWrongId(QVariantMap data);

            void emit_stateAlreadySet();

            void emit_turboPumpAlreadyEnabled();
            void emit_turboPumpAlreadyDisabled();

        public slots:
            // States relating to controlling the vac station
            void disableTurboPump();
            void enableTurboPump();
            void disableBackingPump();
            void enableBackingPump();
            void setGasModeHeavy();
            void setGasModeMedium();
            void setGasModeHelium();

            // States relating to reading the vac station params
            void getTurboSpeed();
            void getBearingTemperature();
            void getTC110ElectronicsTemperature();
            void getPumpBottomTemperature();
            void getMotorTemperature();
            void validateGetBearingTemperature();
            void validateGetTC110ElectronicsTemperature();
            void validateGetPumpBottomTemperature();
            void validateGetMotorTemperature();

            // States relating to validating the vac station commands
            void validateDisableTurboPump();
            void validateEnableTurboPump();
            void validateDisableBackingPump();
            void validateEnableBackingPump();
            void validateSetGasModeHeavy();
            void validateSetGasModeMedium();
            void validateSetGasModeHelium();
            void validateTurboSpeedZero();

        private:

            // Turbo pump state
            bool m_turboState = false;

    };

}}}}
