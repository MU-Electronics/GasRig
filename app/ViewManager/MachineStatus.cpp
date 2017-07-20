#include "MachineStatus.h"

// Include external libs
#include <QMap>

// Include settings container
#include "../Settings/container.h"

// Include threads
#include "../Hardware/Access.h"
#include "../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../Experiment/Engine.h"

// Include machine container
#include "../Experiment/Machines/Machines.h"

namespace App { namespace ViewManager
{
    MachineStatus::MachineStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        :   QObject(parent)
        ,   m_root(root)
        ,   m_settings(settings)
        ,   m_experimentEngine(experimentEngine)
    {


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
        qDebug() << "Vac down machine started for: " << mintues << " mins";
    }


    /**
     * Triggered when the vac down state machine is stopped
     *
     * @brief MachineStatus::vacDownStopped
     */
    void MachineStatus::vacDownStopped()
    {
        qDebug() << "Vac down machine stopped";
    }


}}

