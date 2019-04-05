#include "MachineStatus.h"

// Include external libs
#include <QMap>

namespace App { namespace View { namespace Managers
{
    MachineStatus::MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings, Experiment::Engine* experimentEngine)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
        ,   m_experimentEngine(experimentEngine)
    {
        // Global if machine is running
        m_controllable.insert("ventMachine", 1);
        m_controllable.insert("pressuriseMachine", 1);
        m_controllable.insert("vacDownMachine", 1);
        m_controllable.insert("pulseValveMachine", 1);
        m_controllable.insert("purgeMachine", 1);
        m_controllable.insert("safeValveMachine", 1);

        // Default values for vac state machine
        m_safetyValveMachine.insert("status", 0);
        m_safetyValveMachine.insert("valve", -1);
        m_safetyValveMachine.insert("state", false);

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

        // Default values for continuous pressurise state machine
        m_continiousPressuriseMachine.insert("status", 0);
        m_continiousPressuriseMachine.insert("pressure", -1);
        m_continiousPressuriseMachine.insert("initVacDown", -1);
        m_continiousPressuriseMachine.insert("stepSize", -1);
        m_continiousPressuriseMachine.insert("inputValve", -1);
        m_continiousPressuriseMachine.insert("maxTime", -1);
        m_continiousPressuriseMachine.insert("monitorTime", -1);
        m_continiousPressuriseMachine.insert("topUp", -1);
        m_continiousPressuriseMachine.insert("leak", -1);

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
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_ventMachineStarted, this, &MachineStatus::ventStarted);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_ventMachineStopped, this, &MachineStatus::ventStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_ventMachineFailed, this, &MachineStatus::ventStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_ventMachineStopping, this, &MachineStatus::ventStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_vacDownMachineStarted, this, &MachineStatus::vacDownStarted);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_vacDownMachineStopped, this, &MachineStatus::vacDownStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_vacDownMachineFailed, this, &MachineStatus::vacDownStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_vacDownMachineStopping, this, &MachineStatus::vacDownStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pulseValveStarted, this, &MachineStatus::pulseValveStarted);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pulseValveStopped, this, &MachineStatus::pulseValveStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pulseValveFailed, this, &MachineStatus::pulseValveStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pulseValveStopping, this, &MachineStatus::pulseValveStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_continuousPressureStarted, this, &MachineStatus::continuousPressuriseStarted);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_continuousPressureStopped, this, &MachineStatus::continuousPressuriseStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_continuousPressureFailed, this, &MachineStatus::continuousPressuriseStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_continuousPressureStopping, this, &MachineStatus::continuousPressuriseStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pressuriseStarted, this, &MachineStatus::pressuriseStarted);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pressuriseStopped, this, &MachineStatus::pressuriseStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pressuriseFailed, this, &MachineStatus::pressuriseStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_pressuriseStopping, this, &MachineStatus::pressuriseStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_purgeStarted, this, &MachineStatus::purgeStarted);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_purgeFailed, this, &MachineStatus::purgeStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_purgeStopped, this, &MachineStatus::purgeStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_purgeStopping, this, &MachineStatus::purgeStopping);

        // Connect signals to and from experiment engine
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_safeValveMachineStarted, this, &MachineStatus::safetyValveStarted);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_safeValveMachineStopped, this, &MachineStatus::safetyValveStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_safeValveMachineFailed, this, &MachineStatus::safetyValveStopped);
        connect(&m_experimentEngine->machines(), &Experiment::Machines::Machines::emit_safeValveMachineStopping, this, &MachineStatus::safetyValveStopping);

        // Disable unused compiler warnings
        UNUSED(safety);
    }



    /**
     * Set which state machine should be controllable on the gui
     *
     * @brief MachineStatus::setControllable
     * @param id
     * @return
     */
    void MachineStatus::setControllable(QString id, int level)
    {
        // If machine stopping or in progress then all other machines are diabled
        int others = (level == 2 || level == 0) ? 0 : 1 ;

        // Set others
        m_controllable.insert("ventMachine", others);
        m_controllable.insert("pressuriseMachine", others);
        m_controllable.insert("vacDownMachine", others);
        m_controllable.insert("pulseValveMachine", others);
        m_controllable.insert("purgeMachine", others);
        m_controllable.insert("safeValveMachine", others);

        // Set the current to the set level
        m_controllable.insert(id, level);

       // Update everyone
       emit emit_controllableChanged(m_controllable);
    }






    void MachineStatus::safetyValveStarted(int id, bool state)
    {
        // Update array
        m_safetyValveMachine.insert("status", 1);
        m_safetyValveMachine.insert("valve", id);
        m_safetyValveMachine.insert("state", state);

        // Set the controllable
        setControllable("safeValveMachine", 2);

        // Tell everyone
        emit emit_safetyValveMachineChanged(m_safetyValveMachine);
    }

    void MachineStatus::safetyValveStopping(QVariantMap params)
    {
        // Update array
        m_safetyValveMachine.insert("status", 2);

        // Set the controllable
        setControllable("safeValveMachine", 0);

        // Tell everyone
        emit emit_safetyValveMachineChanged(m_safetyValveMachine);
    }

    void MachineStatus::safetyValveStopped(QVariantMap params)
    {
        // Update array
        m_safetyValveMachine.insert("status", 0);

        // Set the controllable
        setControllable("safeValveMachine", 1);

        // Tell everyone
        emit emit_safetyValveMachineChanged(m_safetyValveMachine);
    }





    void MachineStatus::purgeStarted(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo)
    {
        // Update array
        m_purgeMachine.insert("status", 1);
        m_purgeMachine.insert("outputValve", outputValve);
        m_purgeMachine.insert("numberCycles", numberCycles);
        m_purgeMachine.insert("nitrogenPressure", nitrogenPressure);
        m_purgeMachine.insert("vacTo", vacTo);

        // Set the controllable
        setControllable("purgeMachine", 2);

        // Tell everyone
        emit emit_purgeMachineChanged(m_purgeMachine);
    }

    void MachineStatus::purgeStopping(QVariantMap params)
    {
        // Update array
        m_purgeMachine.insert("status", 2);

        // Set the controllable
        setControllable("purgeMachine", 0);

        // Tell everyone
        emit emit_purgeMachineChanged(m_purgeMachine);
    }

    void MachineStatus::purgeStopped(QVariantMap params)
    {
        // Update array
        m_purgeMachine.insert("status", 0);

        // Set the controllable
        setControllable("purgeMachine", 1);

        qDebug() << m_purgeMachine;

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

        // Set the controllable
        setControllable("ventMachine", 2);

        // Tell everyone
        emit emit_ventMachineChanged(m_ventMachine);
    }

    void MachineStatus::ventStopping(QVariantMap params)
    {
        // Update array
        m_ventMachine.insert("status", 2);

        // Set the controllable
        setControllable("ventMachine", 0);

        // Tell everyone
        emit emit_ventMachineChanged(m_ventMachine);
    }

    void MachineStatus::ventStopped(QVariantMap params)
    {
        // Update array
        m_ventMachine.insert("status", 0);

        // Set the controllable
        setControllable("ventMachine", 1);

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

        // Set the controllable
        setControllable("pressuriseMachine", 2);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }

    void MachineStatus::pressuriseStopping(QVariantMap params)
    {
        m_pressuriseMachine.insert("status", 2);

        // Set the controllable
        setControllable("pressuriseMachine", 0);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }

    void MachineStatus::pressuriseStopped(QVariantMap params)
    {
        m_pressuriseMachine.insert("status", 0);

        // Set the controllable
        setControllable("pressuriseMachine", 1);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }







    /**
     * Triggered when continious pressurise state machine is started
     *
     * @brief MachineStatus::continuousPressuriseStarted
     * @param maxTime
     * @param monitorTime
     * @param topUp
     * @param leak
     * @param pressure
     * @param stepSize
     * @param inputValve
     * @param outputValve
     * @param exhuastValveOnly
     */
    void MachineStatus::continuousPressuriseStarted(int maxTime, int monitorTime, double topUp, double leak, double pressure, int stepSize, bool inputValve, bool outputValve, bool exhuastValveOnly)
    {
        m_pressuriseMachine.insert("status", 1);
        m_pressuriseMachine.insert("pressure", pressure);
        m_pressuriseMachine.insert("stepSize", stepSize);
        m_pressuriseMachine.insert("inputValve", inputValve);
        m_pressuriseMachine.insert("maxTime", maxTime);
        m_pressuriseMachine.insert("monitorTime", monitorTime);
        m_pressuriseMachine.insert("topUp", topUp);
        m_pressuriseMachine.insert("leak", leak);

        // Set the controllable
        setControllable("pressuriseMachine", 2);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }

    void MachineStatus::continuousPressuriseStopping(QVariantMap params)
    {
        m_pressuriseMachine.insert("status", 2);

        // Set the controllable
        setControllable("pressuriseMachine", 0);

        emit emit_pressuriseMachineChanged(m_pressuriseMachine);
    }

    void MachineStatus::continuousPressuriseStopped(QVariantMap params)
    {
        m_pressuriseMachine.insert("status", 0);

        // Set the controllable
        setControllable("pressuriseMachine", 1);

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

        // Set the controllable
        setControllable("pulseValveMachine", 2);

        // Emit that there was an update
        emit emit_pulseValveMachineChanged(m_pulseValveMachine);
    }

    void MachineStatus::pulseValveStopping(QVariantMap params)
    {
        // Update the vac machine status
        m_pulseValveMachine.insert("status", 2);

        // Set the controllable
        setControllable("pulseValveMachine", 0);

        // Emit that there was an update
        emit emit_pulseValveMachineChanged(m_pulseValveMachine);
    }

    void MachineStatus::pulseValveStopped(QVariantMap params)
    {
        // Update the vac machine status
        m_pulseValveMachine.insert("status", 0);

        // Set the controllable
        setControllable("pulseValveMachine", 1);

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

        // Set the controllable
        setControllable("vacDownMachine", 2);

        // Emit that there was an update
        emit emit_vacDownMachineChanged(m_vacDownMachine);
    }


    void MachineStatus::vacDownStopping(QVariantMap params)
    {
        // Update the vac machine status
        m_vacDownMachine.insert("status", 2);

        // Set the controllable
        setControllable("vacDownMachine", 0);

        // Emit that there was an update
        emit emit_vacDownMachineChanged(m_vacDownMachine);
    }


    /**
     * Triggered when the vac down state machine is stopped
     *
     * @brief MachineStatus::vacDownStopped
     */
    void MachineStatus::vacDownStopped(QVariantMap params)
    {
        // Update the vac machine status
        m_vacDownMachine.insert("status", 0);

        // Set the controllable
        setControllable("vacDownMachine", 1);

        // Emit that there was an update
        emit emit_vacDownMachineChanged(m_vacDownMachine);
    }


}}}

