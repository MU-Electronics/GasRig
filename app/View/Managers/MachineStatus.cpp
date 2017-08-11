#include "MachineStatus.h"

// Include external libs
#include <QMap>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

// Include machine container
#include "../../Experiment/Machines/Machines.h"

namespace App { namespace View { namespace Managers
{
    MachineStatus::MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
        ,   m_experimentEngine(experimentEngine)
    {
        // Default values for vac state machine
        m_vacDownMachine.insert("status", false);
        m_vacDownMachine.insert("mintues", 0);
        m_vacDownMachine.insert("turbo", false);
        m_vacDownMachine.insert("gasMode", 0);
        m_vacDownMachine.insert("mode", 0);

        // Default values for vac state machine
        m_pulseValveMachine.insert("status", false);
        m_pulseValveMachine.insert("valve", -1);
        m_pulseValveMachine.insert("cycles", -1);
        m_pulseValveMachine.insert("timeOpen", -1);
        m_pulseValveMachine.insert("timeClosed", -1);

    }



    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief MachineStatus::makeConnections
     * @param hardware
     * @param safety
     */
    void MachineStatus::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Requests for hardware reconnects
        connect(this, &MachineStatus::emit_hardwareAccess, &hardware, &Hardware::Access::hardwareAccess);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_vacDownMachineStarted, this, &MachineStatus::vacDownStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_vacDownMachineStopped, this, &MachineStatus::vacDownStopped);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pulseValveStarted, this, &MachineStatus::pulseValveStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pulseValveStopped, this, &MachineStatus::pulseValveStopped);
    }



    void MachineStatus::pulseValveStarted(int valve, int cycles, int timeOpen, int timeClosed)
    {
        m_pulseValveMachine.insert("status", true);
        m_pulseValveMachine.insert("valve", valve);
        m_pulseValveMachine.insert("cycles", cycles);
        m_pulseValveMachine.insert("timeOpen", timeOpen);
        m_pulseValveMachine.insert("timeClosed", timeClosed);

        // Emit that there was an update
        emit emit_pulseValveMachineChanged(m_pulseValveMachine);
    }

    void MachineStatus::pulseValveStopped()
    {
        // Update the vac machine status
        m_pulseValveMachine.insert("status", false);

        // Emit that there was an update
        emit emit_pulseValveMachineChanged(m_pulseValveMachine);
    }


    /**
     * Triggered when the vac down state machine is triggered
     *
     * @brief MachineStatus::vacDownStarted
     * @param mintues
     * @param turbo
     * @param gasMode
     * @param mode
     */
    void MachineStatus::vacDownStarted(int mintues, bool turbo, int gasMode, int mode)
    {
        // Update the vac machine status
        m_vacDownMachine.insert("status", true);
        m_vacDownMachine.insert("mintues", mintues);
        m_vacDownMachine.insert("turbo", turbo);
        m_vacDownMachine.insert("gasMode", gasMode);
        m_vacDownMachine.insert("mode", mode);

        // Emit that there was an update
        emit emit_vacDownMachineChanged(m_vacDownMachine);
    }


    /**
     * Triggered when the vac down state machine is stopped
     *
     * @brief MachineStatus::vacDownStopped
     */
    void MachineStatus::vacDownStopped()
    {
        // Update the vac machine status
        m_vacDownMachine.insert("status", false);

        // Emit that there was an update
        emit emit_vacDownMachineChanged(m_vacDownMachine);
    }


}}}

