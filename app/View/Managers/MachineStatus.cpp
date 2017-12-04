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

        // Default values for pulse state machine
        m_pulseValveMachine.insert("status", false);
        m_pulseValveMachine.insert("valve", -1);
        m_pulseValveMachine.insert("cycles", -1);
        m_pulseValveMachine.insert("timeOpen", -1);
        m_pulseValveMachine.insert("timeClosed", -1);

        // Default values for pressurise state machine
        m_pressuriseMachine.insert("status", false);
        m_pressuriseMachine.insert("pressure", -1);
        m_pressuriseMachine.insert("initVacDown", -1);
        m_pressuriseMachine.insert("stepSize", -1);
        m_pressuriseMachine.insert("inputValve", -1);

        // Default valves for vent state machine
        m_ventMachine.insert("status", false);
        m_ventMachine.insert("output", false);
        m_ventMachine.insert("vacuumOutput", false);
        m_ventMachine.insert("flowCavity", false);
        m_ventMachine.insert("nitrogenPipes", false);
        m_ventMachine.insert("multiPipes", false);
        m_ventMachine.insert("flowOnePipes", false);
        m_ventMachine.insert("flowTwoPipes", false);

        // Default values for purge state machine
        m_purgeMachine.insert("status", false);
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
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_ventMachineStarted, this, &MachineStatus::ventStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_ventMachineStopped, this, &MachineStatus::ventStopped);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_vacDownMachineStarted, this, &MachineStatus::vacDownStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_vacDownMachineStopped, this, &MachineStatus::vacDownStopped);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pulseValveStarted, this, &MachineStatus::pulseValveStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pulseValveStopped, this, &MachineStatus::pulseValveStopped);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pressuriseStarted, this, &MachineStatus::pressuriseStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pressuriseStopped, this, &MachineStatus::pressuriseStopped);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_purgeStarted, this, &MachineStatus::purgeStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_purgeStopped, this, &MachineStatus::purgeStopped);
    }



    void MachineStatus::purgeStarted(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo)
    {
        // Update array
        m_purgeMachine.insert("status", true);
        m_purgeMachine.insert("outputValve", outputValve);
        m_purgeMachine.insert("numberCycles", numberCycles);
        m_purgeMachine.insert("nitrogenPressure", nitrogenPressure);
        m_purgeMachine.insert("vacTo", vacTo);

        // Tell everyone
        emit emit_purgeMachineChanged(m_purgeMachine);
    }

    void MachineStatus::purgeStopped()
    {
        // Update array
        m_purgeMachine.insert("status", false);

        // Tell everyone
        emit emit_purgeMachineChanged(m_purgeMachine);
    }





    void MachineStatus::ventStarted(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes)
    {
        // Update array
        m_ventMachine.insert("status", true);
        m_ventMachine.insert("output", output);
        m_ventMachine.insert("vacuumOutput", vacuumOutput);
        m_ventMachine.insert("flowCavity", flowCavity);
        m_ventMachine.insert("nitrogenPipes", nitrogenPipes);
        m_ventMachine.insert("multiPipes", multiPipes);
        m_ventMachine.insert("flowOnePipes", flowOnePipes);
        m_ventMachine.insert("flowTwoPipes", flowTwoPipes);

        // Tell everyone
        emit emit_ventMachineChanged(m_ventMachine);
    }

    void MachineStatus::ventStopped()
    {
        // Update array
        m_ventMachine.insert("status", false);

        // Tell everyone
        emit emit_ventMachineChanged(m_ventMachine);
    }





    /**
     * Triggered when pressurise state machine is started
     *
     * @brief MachineStatus::pressuriseStarted
     * @param pressure
     * @param initVacDown
     * @param stepSize
     * @param inputValve
     */
    void MachineStatus::pressuriseStarted(double pressure, bool initVacDown, int stepSize, bool inputValve)
    {
        m_pressuriseMachine.insert("status", true);
        m_pressuriseMachine.insert("pressure", pressure);
        m_pressuriseMachine.insert("initVacDown", initVacDown);
        m_pressuriseMachine.insert("stepSize", stepSize);
        m_pressuriseMachine.insert("inputValve", inputValve);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }

    void MachineStatus::pressuriseStopped()
    {
        m_pressuriseMachine.insert("status", false);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }






    /**
     * Triggered when pulse valve state machine is started
     *
     * @brief MachineStatus::pulseValveStarted
     * @param valve
     * @param cycles
     * @param timeOpen
     * @param timeClosed
     */
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
    void MachineStatus::vacDownStarted(double value, int valueType, bool turbo, int gasMode, int mode)
    {
        // Update the vac machine status
        m_vacDownMachine.insert("status", true);
        m_vacDownMachine.insert("value", value);
        m_vacDownMachine.insert("valueType", valueType);
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

