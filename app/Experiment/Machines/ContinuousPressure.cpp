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

            // State machines
        ,   m_pressurise(*new Pressurise(parent, settings, hardware, safety))
    {
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

        connect(state("startMaxTime", true), &QState::entered, this, &ContinuousPressure::startMaxTime);

        connect(state("setPressure", true), &QState::entered, this, &ContinuousPressure::setHighPressure);

        connect(state("systemPressure", true), &QState::entered, this->pressure(), &Functions::Pressure::systemPressure);
        connect(validator("systemPressure", true), &QState::entered, this->pressure(), &Functions::Pressure::validatePressure);

        connect(validator("checkPressure", true), &QState::entered, this, &ContinuousPressure::checkPressure);
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
    void ContinuousPressure::setParams(int maxTime, int monitorTime, double topUp, double leak, double pressure, int stepSize, bool inputValve, bool outputValve)
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

        // Setup timers
        t_maxTime.setInterval(maxTime);
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
     * Builds the machine connections
     *
     * @brief ContinuousPressure::buildMachine
     */
    void ContinuousPressure::buildMachine()
    {
        // Where to start the machine
        sm_master.setInitialState(state("startMaxTime", true));


        // Start max time
        state("startMaxTime", true)->addTransition(this, &ContinuousPressure::emit_maxTimerStarted, state("systemPressure", true));


        // Set pressure
        state("setPressure", true)->addTransition(&m_pressurise, &Pressurise::emit_machineFailed, &sm_stopAsFailed);
        state("setPressure", true)->addTransition(&m_pressurise, &Pressurise::emit_machineFinished, state("systemPressure", true));


        // Read the pressure sensor
        state("systemPressure", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("systemPressure", true));
            // Validate
            validator("systemPressure", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationFailed, state("startPressureMonitor", true));
            validator("systemPressure", true)->addTransition(this->pressure(), &Functions::Pressure::emit_validationSuccess, state("startPressureMonitor", true));
                    // LEAK drop of over X stop machine
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_leakDetected, &sm_stopAsFailed);
                    // Top up using pressure state machine below Y
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_leakDetected, state("setPressure", true));
                    // Max time ran out
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_maxTimeElapsed, &sm_stop);
                    // Carry on checking
                    validator("checkPressure", true)->addTransition(this, &ContinuousPressure::emit_nothingRequired, state("systemPressure", true));

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
     * Timer to use to trigger reading the pressure sensor
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
     * Set the high pressure
     *
     * @brief ContinuousPressure::setHighPressure
     */
    void ContinuousPressure::setHighPressure()
    {
        // Set params
        m_pressurise.setParams(params.value("pressure").toDouble(), true, params["stepSize"].toInt(), params.value("inputValve").toBool(), params.value("outputValve").toBool());

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






