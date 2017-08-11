#include "Timers.h"

#include <QString>

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Include the command constructor for hardware gateway
#include "../../../Hardware/CommandConstructor.h"

// Include valaitor state
#include "CommandValidatorState.h"

namespace App { namespace Experiment { namespace Machines { namespace States
{

    Timers::Timers(QObject *parent, Settings::Container settings, Hardware::Access &hardware, Safety::Monitor &safety, QStateMachine& machine, QVariantMap &params, Hardware::CommandConstructor &commandConstructor)
        :   QObject(parent)

        ,   m_settings(settings)
        ,   m_hardware(hardware)
        ,   m_safety(safety)
        ,   m_machine(machine)
        ,   m_params(params)
        ,   m_commandConstructor(commandConstructor)

            // Timer instances
        ,   t_vacPressureMonitor(parent)
        ,   t_pressureMonitor(parent)
        ,   t_flowControllerFlowMonitor(parent)
        ,   t_flowControllerTemperatureMonitor(parent)
        ,   t_vacStationTemperatureMonitor(parent)
        ,   t_turboSpeedMonitor(parent)
        ,   t_vacTime(parent)

            // Timers
        ,   sm_initalWait(&machine)
        ,   sm_timerWait(&machine)
        ,   sm_startVacuumPressureMonitor(&machine)
        ,   sm_startPressureMonitor(&machine)
        ,   sm_startFlowControllerFlowMonitor(&machine)
        ,   sm_startFlowControllerTemperatureMonitor(&machine)
        ,   sm_startVacStationTemperatureMonitor(&machine)
        ,   sm_startTurboSpeedMonitor(&machine)
        ,   sm_startVacuumTimer(&machine)

    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Timers::hardwareRequest, &m_hardware, &Hardware::Access::hardwareAccess);

        // Connect the states to functions
        connectStatesToMethods();
    }

    Timers::~Timers()
    {

    }

    void Timers::connectStatesToMethods()
    {
        // Link the timer states
        connect(&sm_initalWait, &QState::entered, this, &Timers::timerWait);
        connect(&sm_timerWait, &QState::entered, this, &Timers::timerWait);
        connect(&sm_startVacuumPressureMonitor, &QState::entered, this, &Timers::startVacuumPressureMonitor);
        connect(&sm_startPressureMonitor, &QState::entered, this, &Timers::startPressureMonitor);
        connect(&sm_startFlowControllerFlowMonitor, &QState::entered, this, &Timers::startFlowControllerFlowMonitor);
        connect(&sm_startVacuumTimer, &QState::entered, this, &Timers::startVacuumTimer);

        connect(&sm_startFlowControllerTemperatureMonitor, &QState::entered, this, &Timers::startFlowControllerTemperatureMonitor);
        connect(&sm_startVacStationTemperatureMonitor, &QState::entered, this, &Timers::startVacStationTemperatureMonitor);
        connect(&sm_startTurboSpeedMonitor, &QState::entered, this, &Timers::startTurboSpeedMonitor);
    }


    /**
     * Empty state to wait for timer events
     *
     * @brief MachineStates::timerWait
     */
    void Timers::timerWait()
    {
    }



    /**
     * The timer for how long to vac down for
     *
     * @brief Timers::startVacuumTimer
     */
    void Timers::startVacuumTimer()
    {
        if(!t_vacTime.isActive())
        {
            // Setup timer
            t_vacTime.setSingleShot(true);
            t_vacTime.start();
        }

        emit emit_timerActive();
    }

    /**
     * Stop the vac down timer
     *
     * @brief Timers::stopVacuumTimer
     */
    void Timers::stopVacuumTimer()
    {
        t_vacTime.stop();
    }




    /**
     * Timer to use to trigger reading the vacuum sensor
     *
     * @brief Timers::startVacuumPressureMonitor
     */
    void Timers::startVacuumPressureMonitor()
    {
        if(!t_vacPressureMonitor.isActive())
        {
            // Setup timer
            t_vacPressureMonitor.setSingleShot(false);
            t_vacPressureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vacuum sensor
     *
     * @brief Timers::stopVacuumPressureMonitor
     */
    void Timers::stopVacuumPressureMonitor()
    {
        t_vacPressureMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the pressure sensor
     *
     * @brief Timers::startPressureMonitor
     */
    void Timers::startPressureMonitor()
    {
        if(!t_pressureMonitor.isActive())
        {
            // Setup timer
            t_pressureMonitor.setSingleShot(false);
            t_pressureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the pressure sensor
     *
     * @brief Timers::stopPressureMonitor
     */
    void Timers::stopPressureMonitor()
    {
        t_pressureMonitor.stop();
    }



    /**
     * Timer to use to trigger reading the flow controller flow sensor
     *
     * @brief Timers::startPressureMonitor
     */
    void Timers::startFlowControllerFlowMonitor()
    {
        if(!t_flowControllerFlowMonitor.isActive())
        {
            // Setup timer
            t_flowControllerFlowMonitor.setSingleShot(false);
            t_flowControllerFlowMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the flow controller flow sensor
     *
     * @brief Timers::stopPressureMonitor
     */
    void Timers::stopFlowControllerFlowMonitor()
    {
        t_flowControllerFlowMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the vac station turbo speed sensor
     *
     * @brief Timers::startTurboSpeedMonitor
     */
    void Timers::startTurboSpeedMonitor()
    {
        if(!t_turboSpeedMonitor.isActive())
        {
            // Setup timer
            t_turboSpeedMonitor.setSingleShot(false);
            t_turboSpeedMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vac station turbo speed sensor
     *
     * @brief Timers::stopTurboSpeedMonitor
     */
    void Timers::stopTurboSpeedMonitor()
    {
        t_turboSpeedMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the vac station temperature sensor
     *
     * @brief Timers::startVacStationTemperatureMonitor
     */
    void Timers::startVacStationTemperatureMonitor()
    {
        if(!t_vacStationTemperatureMonitor.isActive())
        {
            // Setup timer
            t_vacStationTemperatureMonitor.setSingleShot(false);
            t_vacStationTemperatureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the vac station temperature sensor
     *
     * @brief Timers::stopVacStationTemperatureMonitor
     */
    void Timers::stopVacStationTemperatureMonitor()
    {
        t_vacStationTemperatureMonitor.stop();
    }


    /**
     * Timer to use to trigger reading the flow controller temperature sensor
     *
     * @brief Timers::startFlowControllerTemperatureMonitor
     */
    void Timers::startFlowControllerTemperatureMonitor()
    {
        if(!t_flowControllerTemperatureMonitor.isActive())
        {
            // Setup timer
            t_flowControllerTemperatureMonitor.setSingleShot(false);
            t_flowControllerTemperatureMonitor.start();
        }

        // Emit the timer started
        emit emit_timerActive();
    }


    /**
     * Stop the timer triggering reading of the flow controller temperature sensor
     *
     * @brief Timers::stopFlowControllerTemperatureMonitor
     */
    void Timers::stopFlowControllerTemperatureMonitor()
    {
        t_flowControllerTemperatureMonitor.stop();
    }


}}}}
