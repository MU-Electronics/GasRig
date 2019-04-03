#include "ContinuousPressure.h"

// Include extenral deps
#include <QObject>
#include <QDateTime>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "Functions/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    ContinuousPressure::ContinuousPressure(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Timers
        ,   t_maxTime(parent)
        ,   t_monitorTime(parent)
        ,   t_pressureDelayTime(parent)

            // State machines
        ,   m_pressurise(*new Pressurise(parent, settings, hardware, safety))
    {
        // We have stop state machines
        shutDownMachines = true;



        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());



        // Timers
        connect(state("startMaxTime", true), &QState::entered, this, &ContinuousPressure::startMaxTime);
        connect(state("startMonitorTime", true), &QState::entered, this, &ContinuousPressure::startMonitorTime);
        connect(state("startDelayTime", true), &QState::entered, this, &ContinuousPressure::startDelayTime);


        // Open vavle 1
        connect(state("openValve1", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(validator("openValve1", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);


        // Close valve 1
        connect(state("closeValve1_pressure", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(validator("closeValve1_pressure", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);

        connect(state("closeValve1_nothing", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(validator("closeValve1_nothing", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);

        connect(state("closeValve1_elapsed", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(validator("closeValve1_elapsed", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);

        connect(state("closeValve1_leak", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(validator("closeValve1_leak", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);


        // Set pressure
        connect(state("setPressure", true), &QState::entered, this, &ContinuousPressure::setHighPressure);


        // Read pressure
        connect(state("systemPressure", true), &QState::entered, this->pressure(), &Functions::Pressure::systemPressure);
        connect(validator("systemPressure", true), &Functions::CommandValidatorState::entered, this->pressure(), &Functions::Pressure::validatePressure);


        // Check pressure
        connect(validator("checkPressure", true), &Functions::CommandValidatorState::entered, this, &ContinuousPressure::checkPressure);




        // Shutdown states
        connect(state("pressurise", false), &QState::entered, this, &ContinuousPressure::stopPressurise);


        // SHUTDOWN STATE MACHINE: Link close valve states
        connect(state("closeHighPressureIn", false), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("closeNitrogenIn", false), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("closeFlowController", false), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("closeExhuast", false), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("closeOuput", false), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("closeSlowExhuast", false), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("closeFastExhuast", false), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeVacuumIn", false), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("closeVacuumOut", false), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // SHUTDOWN STATE MACHINE: Link close valve validator states
        connect(validator("closeHighPressureIn", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("closeNitrogenIn", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("closeFlowController", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("closeExhuast", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("closeOutput", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("closeSlowExhuast", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("closeFastExhuast", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("closeVacuumIn", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("closeVacuumOut", false), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

    }

    ContinuousPressure::~ContinuousPressure()
    {

    }


    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void ContinuousPressure::setParams(int maxTime, int monitorTime, double topUp, double leak, double pressure, int stepSize, bool inputValve, bool outputValve, bool exhuastValveOnly)
    {       
        // Save params
        params.insert("maxTime", maxTime); // minutes
        params.insert("monitorTime", monitorTime); // seconds
        params.insert("topUp", topUp); // millbar
        params.insert("leak", leak); // millbar
        params.insert("pressure", pressure); // millbar
        params.insert("stepSize", stepSize); // millbar
        params.insert("inputValve", inputValve);
        params.insert("outputValve", outputValve); // valve 1 at step size
        params.insert("exhuastValveOnly", exhuastValveOnly);

        // Setup timers
        t_maxTime.setInterval(maxTime);
        t_monitorTime.setInterval(monitorTime);
        t_pressureDelayTime.setInterval(3000);
    }


    /**
     * Start the state machine
     *
     * @brief ReadPressure::start
     */
    void ContinuousPressure::beforeStart()
    {

    }


    /**
     * Start the state machine
     *
     * @brief ContinuousPressure::stopped
     */
    void ContinuousPressure::stopped()
    {
    }



    /**
     * Builds the shutdown state machine
     *
     * @brief ContinuousPressure::buildShutDownMachine
     */
    void ContinuousPressure::buildShutDownMachine()
    {
        // Stop set high pressure
        if(m_pressurise.machine.isRunning())
        {
            shutDownMachine.setInitialState(state("pressurise", false));
            state("pressurise", false)->addTransition(&m_pressurise, &Pressurise::emit_machineFinished, state("closeOuput", false));
            state("pressurise", false)->addTransition(&m_pressurise, &Pressurise::emit_machineFailed, state("closeOuput", false));
        }
        else
        {
            shutDownMachine.setInitialState(state("closeOuput", false));
        }


        // Ensure all valves are closed
        transitionsBuilder()->closeAllValves(
            state("closeOuput", false),
            validator("closeOutput", false),
            state("closeSlowExhuast", false),
            validator("closeSlowExhuast", false),
            state("closeFastExhuast", false),
            validator("closeFastExhuast", false),
            state("closeVacuumIn", false),
            validator("closeVacuumIn", false),
            state("closeVacuumOut", false),
            validator("closeVacuumOut", false),
            state("closeExhuast", false),
            validator("closeExhuast", false),
            state("closeHighPressureIn", false),
            validator("closeHighPressureIn", false),
            state("closeNitrogenIn", false),
            validator("closeNitrogenIn", false),
            state("closeFlowController", false),
            validator("closeFlowController", false),
            &ssm_stop,
            &ssm_stopAsFailed
        );
    }



    /**
     * Builds the machine connections
     *
     * @brief ContinuousPressure::buildMachine
     */
    void ContinuousPressure::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("startMaxTime", true));


        // Start max time
        state("startMaxTime", true)->addTransition(this, &ContinuousPressure::emit_maxTimerStarted, state("setPressure", true));


        // Set pressure
        state("setPressure", true)->addTransition(&m_pressurise, &Pressurise::emit_machineFailed, &sm_stopAsFailed);
        state("setPressure", true)->addTransition(&m_pressurise, &Pressurise::emit_machineFinished, state("startMonitorTime", true));

        // At monitor interval
        state("startMonitorTime", true)->addTransition(&t_pressureDelayTime, &QTimer::timeout, state("openValve1", true));

        // Open valve 1
        transitionsBuilder()->openValve(state("openValve1", true), validator("openValve1", true), state("startDelayTime", true), &sm_stopAsFailed);

        // Wait
        state("startDelayTime", true)->addTransition(&t_pressureDelayTime, &QTimer::timeout, state("systemPressure", true));

        // Read the pressure sensor
        state("systemPressure", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("systemPressure", true));
            // Validate
            validator("systemPressure", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, state("systemPressure", true));
            validator("systemPressure", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, validator("checkPressure", true));
                    // LEAK drop of over X stop machine
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_leakDetected, state("closeValve1_leak", true));
                    // Top up using pressure state machine below Y
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_topUpRequired, state("closeValve1_pressure", true));
                    // Max time ran out
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_maxTimeElapsed, state("closeValve1_elapsed", true));
                    // Carry on checking
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_nothingRequired, state("closeValve1_nothing", true));

        // Close valve 1 (lazy method really!)
        transitionsBuilder()->closeValve(state("closeValve1_pressure", true), validator("closeValve1_pressure", true), state("setPressure", true), &sm_stopAsFailed);
        transitionsBuilder()->closeValve(state("closeValve1_nothing", true), validator("closeValve1_nothing", true), state("startMonitorTime", true), &sm_stopAsFailed);
        transitionsBuilder()->closeValve(state("closeValve1_elapsed", true), validator("closeValve1_elapsed", true), &sm_stop, &sm_stopAsFailed);
        transitionsBuilder()->closeValve(state("closeValve1_leak", true), validator("closeValve1_leak", true), &sm_stopAsFailed, &sm_stopAsFailed);

        // Account for com issues
        transitionsBuilder()->stateComErrors(state("systemPressure", true), state("systemPressure", true));
        transitionsBuilder()->stateComErrors(validator("systemPressure", true), state("systemPressure", true));
    }


    void ContinuousPressure::checkPressure()
    {
        // Get the validator state instance
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Have we reach max time
        if(!t_maxTime.isActive())
        {
            emit emit_maxTimeElapsed();
            return;
        }

        // Current pressure value and convert to mbar
        double currentPressure = package.value("pressure").toDouble() * 1000;

        // Mark first time time
        if(m_time == -1)
        {
            m_pastPressure = currentPressure;
        }

        // Mark current time
        long long time = QDateTime::currentSecsSinceEpoch();

        if((time - m_time) > params["monitorTime"].toInt())
        {
            // Mark new time
            m_time = time;

            // Check for leak
            if((currentPressure - m_pastPressure) > params["leak"].toDouble())
            {
                emit emit_leakDetected();
                return;
            }

            // Check for topup
            if((currentPressure - m_pastPressure) > params["topUp"].toDouble())
            {
                emit emit_topUpRequired();
                return;
            }
        }

        // Continue along some path
        emit emit_nothingRequired();
    }


    /**
     * Timer to use to trigger end of session
     *
     * @brief ContinuousPressure::startMaxTime
     */
    void ContinuousPressure::startMaxTime()
    {
        // Setup timer
        t_maxTime.setSingleShot(true);
        t_maxTime.start();
        emit emit_maxTimerStarted();
    }

    /**
     * Timer to use to trigger reading the pressure sensor
     *
     * @brief ContinuousPressure::startMonitorTime
     */
    void ContinuousPressure::startMonitorTime()
    {
        // Setup timer
        t_monitorTime.setSingleShot(true);
        t_monitorTime.start();
    }

    /**
     * Timer to use to trigger gas propergated
     *
     * @brief ContinuousPressure::startDelayTime
     */
    void ContinuousPressure::startDelayTime()
    {
        // Setup timer
        t_pressureDelayTime.setSingleShot(true);
        t_pressureDelayTime.start();
    }

    /**
     * Set the high pressure
     *
     * @brief ContinuousPressure::setHighPressure
     */
    void ContinuousPressure::setHighPressure()
    {
        // Set params
        m_pressurise.setParams(params.value("pressure").toDouble(), true, params["stepSize"].toInt(), params.value("inputValve").toBool(), params.value("outputValve").toBool(), params.value("exhuastValveOnly").toBool());

        // Build the machine
        m_pressurise.buildMachine();

        // Start the machine
        m_pressurise.start();
    }

    /**
     * Stop the pressurise machine
     *
     * @brief ContinuousPressure::stopPressurise
     */
    void ContinuousPressure::stopPressurise()
    {
        // Start the machine
        m_pressurise.cancelStateMachine();
    }

}}}






