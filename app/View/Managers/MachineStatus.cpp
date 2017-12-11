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
        // Global if machine is running


        // Default values for vac state machine
        m_vacDownMachine.insert("status", 0);
        m_vacDownMachine.insert("mintues", 0);
        m_vacDownMachine.insert("turbo", false);
        m_vacDownMachine.insert("gasMode", 0);
        m_vacDownMachine.insert("mode", 0);

        // Default values for pulse state machine
        m_pulseValveMachine.insert("status", 0);
        m_pulseValveMachine.insert("valve", -1);
        m_pulseValveMachine.insert("cycles", -1);
        m_pulseValveMachine.insert("timeOpen", -1);
        m_pulseValveMachine.insert("timeClosed", -1);

        // Default values for pressurise state machine
        m_pressuriseMachine.insert("status", 0);
        m_pressuriseMachine.insert("pressure", -1);
        m_pressuriseMachine.insert("initVacDown", -1);
        m_pressuriseMachine.insert("stepSize", -1);
        m_pressuriseMachine.insert("inputValve", -1);

        // Default valves for vent state machine
        m_ventMachine.insert("status", 0);
        m_ventMachine.insert("output", false);
        m_ventMachine.insert("vacuumOutput", false);
        m_ventMachine.insert("flowCavity", false);
        m_ventMachine.insert("nitrogenPipes", false);
        m_ventMachine.insert("multiPipes", false);
        m_ventMachine.insert("flowOnePipes", false);
        m_ventMachine.insert("flowTwoPipes", false);

        // Default values for purge state machine
        m_purgeMachine.insert("status", 0);
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
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_ventMachineStopping, this, &MachineStatus::ventStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_vacDownMachineStarted, this, &MachineStatus::vacDownStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_vacDownMachineStopped, this, &MachineStatus::vacDownStopped);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_vacDownMachineStopping, this, &MachineStatus::vacDownStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pulseValveStarted, this, &MachineStatus::pulseValveStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pulseValveStopped, this, &MachineStatus::pulseValveStopped);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pulseValveStopping, this, &MachineStatus::pulseValveStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pressuriseStarted, this, &MachineStatus::pressuriseStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pressuriseStopped, this, &MachineStatus::pressuriseStopped);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_pressuriseStopping, this, &MachineStatus::pressuriseStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_purgeStarted, this, &MachineStatus::purgeStarted);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_purgeStopped, this, &MachineStatus::purgeStopped);
        connect(&m_experimentEngine.machines(), &Experiment::Machines::Machines::emit_purgeStopping, this, &MachineStatus::purgeStopping);


        // Global register if any machines are running
        connect(this, &MachineStatus::emit_purgeMachineChanged, this, &MachineStatus::machinesStatues);
        connect(this, &MachineStatus::emit_vacDownMachineChanged, this, &MachineStatus::machinesStatues);
        connect(this, &MachineStatus::emit_pulseValveMachineChanged, this, &MachineStatus::machinesStatues);
        connect(this, &MachineStatus::emit_pressuriseMachineChanged, this, &MachineStatus::machinesStatues);
        connect(this, &MachineStatus::emit_ventMachineChanged, this, &MachineStatus::machinesStatues);
    }




    /**
     * Allows easy way for views to determin if any state machine is running
     *
     * @brief MachineStatus::machinesStatues
     * @param info
     */
    void MachineStatus::machinesStatues(QVariantMap info)
    {
        // Is the state machine running?
        if(info.value("status").toInt() == 0 && stateMachineRunning() != 0)
        {
            // State that a machine is not running
            stateMachineRunning(a_stateMachineRunning - 1);
        }
        else
        {
            // State that a machine is  running
            stateMachineRunning(a_stateMachineRunning + 1);
        }
    }


    /**
     * Should a block in the UI be diabled depending if state machines are running or
     * if the current statte machine running is the current block
     *
     * @brief MachineStatus::shouldEnable
     * @param id
     * @return
     */
    bool MachineStatus::shouldEnable(QString id)
    {
        // Check if the id state machine is the one running?
        bool status = false;
        if(id == "ventMachine")
            status = m_ventMachine.value("status").toBool();
        if(id == "pressuriseMachine")
            status = m_pressuriseMachine.value("status").toBool();
        if(id == "vacDownMachine")
            status = m_vacDownMachine.value("status").toBool();
        if(id == "pulseValveMachine")
            status = m_pulseValveMachine.value("status").toBool();
        if(id == "purgeMachine")
            status = m_purgeMachine.value("status").toBool();


        // Should block be enabled?
        if( (stateMachineRunning() != 0) && !status)
        {
            return false;
        }

        return true;
    }





    void MachineStatus::purgeStarted(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo)
    {
        // Update array
        m_purgeMachine.insert("status", 1);
        m_purgeMachine.insert("outputValve", outputValve);
        m_purgeMachine.insert("numberCycles", numberCycles);
        m_purgeMachine.insert("nitrogenPressure", nitrogenPressure);
        m_purgeMachine.insert("vacTo", vacTo);

        // Tell everyone
        emit emit_purgeMachineChanged(m_purgeMachine);
    }

    void MachineStatus::purgeStopping()
    {
        // Update array
        m_purgeMachine.insert("status", 2);

        // Tell everyone
        emit emit_purgeMachineChanged(m_purgeMachine);
    }

    void MachineStatus::purgeStopped()
    {
        // Update array
        m_purgeMachine.insert("status", 0);

        // Tell everyone
        emit emit_purgeMachineChanged(m_purgeMachine);
    }





    void MachineStatus::ventStarted(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes)
    {
        // Update array
        m_ventMachine.insert("status", 1);
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

    void MachineStatus::ventStopping()
    {
        // Update array
        m_ventMachine.insert("status", 2);

        // Tell everyone
        emit emit_ventMachineChanged(m_ventMachine);
    }

    void MachineStatus::ventStopped()
    {
        // Update array
        m_ventMachine.insert("status", 0);

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
        m_pressuriseMachine.insert("status", 1);
        m_pressuriseMachine.insert("pressure", pressure);
        m_pressuriseMachine.insert("initVacDown", initVacDown);
        m_pressuriseMachine.insert("stepSize", stepSize);
        m_pressuriseMachine.insert("inputValve", inputValve);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }

    void MachineStatus::pressuriseStopping()
    {
        m_pressuriseMachine.insert("status", 2);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }

    void MachineStatus::pressuriseStopped()
    {
        m_pressuriseMachine.insert("status", 0);

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
        m_pulseValveMachine.insert("status", 1);
        m_pulseValveMachine.insert("valve", valve);
        m_pulseValveMachine.insert("cycles", cycles);
        m_pulseValveMachine.insert("timeOpen", timeOpen);
        m_pulseValveMachine.insert("timeClosed", timeClosed);

        // Emit that there was an update
        emit emit_pulseValveMachineChanged(m_pulseValveMachine);
    }

    void MachineStatus::pulseValveStopping()
    {
        // Update the vac machine status
        m_pulseValveMachine.insert("status", 2);

        // Emit that there was an update
        emit emit_pulseValveMachineChanged(m_pulseValveMachine);
    }

    void MachineStatus::pulseValveStopped()
    {
        // Update the vac machine status
        m_pulseValveMachine.insert("status", 0);

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
        m_vacDownMachine.insert("status", 1);
        m_vacDownMachine.insert("value", value);
        m_vacDownMachine.insert("valueType", valueType);
        m_vacDownMachine.insert("turbo", turbo);
        m_vacDownMachine.insert("gasMode", gasMode);
        m_vacDownMachine.insert("mode", mode);

        // Emit that there was an update
        emit emit_vacDownMachineChanged(m_vacDownMachine);
    }


    void MachineStatus::vacDownStopping()
    {
        // Update the vac machine status
        m_vacDownMachine.insert("status", 2);

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
        m_vacDownMachine.insert("status", 0);

        // Emit that there was an update
        emit emit_vacDownMachineChanged(m_vacDownMachine);
    }


}}}

