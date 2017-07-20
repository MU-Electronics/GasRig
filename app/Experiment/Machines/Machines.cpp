#include "Machines.h"

// Include external libs
#include <QDebug>

// Include state machine
#include "VacDown.h"

namespace App { namespace Experiment { namespace Machines
{


    Machines::Machines(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   QObject(parent)
        ,   m_vacDown(*new VacDown(parent, settings, hardware, safety))
    {
        // Connect the finished signals for the machines
        connect(&m_vacDown, &VacDown::emit_vacDownFinished, this, &Machines::vacDownFinished);
    }


    /**
     * Start a new vac down state machine running
     *
     * @brief Machines::VacDown
     * @param mintues
     * @param turbo
     * @param gasMode
     * @param mode
     */
    void Machines::vacDown(int mintues, bool turbo, int gasMode, int mode)
    {
        // Set the params
        m_vacDown.setParams(mintues, turbo, gasMode, mode);

        // Build the machine
        m_vacDown.buildMachine();

        // Start the machine
        m_vacDown.start();

        // Emit machine started
        emit emit_vacDownMachineStarted(mintues, turbo, gasMode, mode);
    }

    /**
     * Stops a running instance of vac down state machine
     *
     * @brief Machines::StopVacDown
     */
    void Machines::stopVacDown()
    {
        // Stop the machine
        m_vacDown.stop();

        // Emit machine stopped
        emit emit_vacDownMachineStopped();
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::vacDownFinished
     */
    void Machines::vacDownFinished(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_vacDownMachineStopped();
    }




    /**
     * Start a new purge system state machine running using method one
     *
     * @brief Machines::purgeSystem
     * @param method
     * @param outputValve
     */
    void Machines::purgeSystemMethodOne(bool outputValve, int cycles, QString pressure)
    {

    }


    /**
     * Start a new purge system state machine running with method two
     *
     * @brief Machines::purgeSystem
     * @param method
     * @param outputValve
     */
    void Machines::purgeSystemMethodTwo(int minutes, QString pressure)
    {

    }


    /**
     * Stops a running instance of purge system state machine
     *
     * @brief Machines::stopPurgeSystem
     */
    void Machines::stopPurgeSystem()
    {

    }



    /**
     * Start a new exhuast state machine running
     *
     * @brief Machines::exhuast
     * @param frequency
     * @param speed
     */
    void Machines::exhuast(int frequency, int speed)
    {

    }

    /**
     * Stops a running instance of stop exhaust state machine
     *
     * @brief Machines::stopExhuast
     */
    void Machines::stopExhuast()
    {

    }



    /**
     * Start a new negative tune state machine running
     *
     * @brief Machines::negativeTune
     * @param pressure
     */
    void Machines::negativeTune(double pressure)
    {

    }

    /**
     * Stops a running instance of negative tune state machine
     *
     * @brief Machines::stopNegativeTune
     */
    void Machines::stopNegativeTune()
    {

    }



    /**
     * Start a new set high pressure state machine running
     *
     * @brief Machines::setHighPressure
     * @param pressure
     * @param input
     * @param frequency
     */
    void Machines::setHighPressure(double pressure, int input, int frequency)
    {

    }


    /**
     * Stops a running instance of  state machine
     *
     * @brief Machines::stopSetHighPressure
     */
    void Machines::stopSetHighPressure()
    {

    }




    /**
     * Start a new output pressure state machine running
     *
     * @brief Machines::outputPressure
     * @param frequency
     */
    void Machines::outputPressure(int frequency)
    {

    }

    /**
     * Stops a running instance of  state machine
     *
     * @brief Machines::stopOutputPressure
     */
    void Machines::stopOutputPressure()
    {

    }




    /**
     * Start a new open valve state machine running
     *
     * @brief Machines::valveOpen
     * @param id
     */
    void Machines::valveOpen(int id)
    {

    }

    /**
     * Stops a running instance of close valve state machine
     *
     * @brief Machines::valveClose
     * @param id
     */
    void Machines::valveClose(int id)
    {

    }





    /**
     * Start a new flow rate state machine running
     *
     * @brief Machines::setFlowRate
     * @param flowController
     * @param rate
     */
    void Machines::setFlowRate(int flowController, double rate)
    {

    }


    /**
     * Stops a running instance of flow rate state machine
     *
     * @brief Machines::stopFlowRate
     * @param flowController
     * @param rate
     */
    void Machines::stopFlowRate(int flowController, double rate)
    {

    }

}}}
