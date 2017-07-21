#include "Machines.h"

// Include external libs
#include <QDebug>

// Include state machine
#include "VacDown.h"
#include "SafeValve.h"
#include "SensorReadings.h"

namespace App { namespace Experiment { namespace Machines
{


    Machines::Machines(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   QObject(parent)
        ,   m_vacDown(*new VacDown(parent, settings, hardware, safety))
        ,   m_safeValve(*new SafeValve(parent, settings, hardware, safety))
        ,   m_sensorReadings(*new SensorReadings(parent, settings, hardware, safety))
    {
        // Connect the finished signals for the machine vac down
        connect(&m_vacDown, &VacDown::emit_vacDownFinished, this, &Machines::vacDownFinished);
        connect(&m_vacDown, &VacDown::emit_vacDownFailed, this, &Machines::vacDownFailed);

        // Connect the finished signals for the machine safe valve
        connect(&m_safeValve, &SafeValve::emit_safeValveFinished, this, &Machines::sensorReadingsFinished);
        connect(&m_safeValve, &SafeValve::emit_safeValveFailed, this, &Machines::sensorReadingsFailed);

        // Connect the finished signals for the machine sensor readings
        connect(&m_sensorReadings, &SensorReadings::emit_sensorReadingsStopped, this, &Machines::valveStateFinished);
        connect(&m_sensorReadings, &SensorReadings::emit_sensorReadingsFailed, this, &Machines::valveStateFailed);

        // Connect the finished signals for the machine purge system

        // Connect the finished signals for the machine exhuast system

        // Connect the finished signals for the machine set flow rate

        // Connect the finished signals for the machine output pressure

        // Connect the finished signals for the machine set high pressure
    }



    /**
     * Start a new sensor readings state machine running
     *
     * @brief Machines::sensorReadings
     * @param vacSensorTimeInter
     * @param pressureSensorTimeInter
     * @param flowControllerTimeInter
     */
    void Machines::sensorReadings(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter)
    {
        // Set the params
        m_sensorReadings.setParams(vacSensorTimeInter, pressureSensorTimeInter, flowControllerTimeInter);

        // Build the machine
        m_sensorReadings.buildMachine();

        // Start the machine
        m_sensorReadings.start();

        // Emit machine started
        emit emit_sensorReadingsMachineStarted(vacSensorTimeInter, pressureSensorTimeInter, flowControllerTimeInter);
    }


    /**
     * Stops a running instance of sensor readings state machine
     *
     * @brief Machines::stopSensorReadings
     */
    void Machines::stopSensorReadings()
    {
        // Stop the machine
        m_sensorReadings.stop();

        // Tell everyone we've stopped
        emit emit_sensorReadingsMachineStopped();
    }


    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machines::sensorReadingsFinished
     * @param params
     */
    void Machines::sensorReadingsFinished(QVariantMap params)
    {

    }


    /**
     * This method is trigged if the state machine failed
     *
     * @brief Machines::sensorReadingsFailed
     * @param params
     */
    void Machines::sensorReadingsFailed(QVariantMap params)
    {

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
     * This method is trigged if the state machine failed
     *
     * @brief Machine::vacDownFailed
     */
    void Machines::vacDownFailed(QVariantMap params)
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
    void Machines::exhuast(double pressure, int frequency, int speed)
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
        // Set the params
        m_safeValve.setParams(id, true);

        // Build the machine
        m_safeValve.buildMachine();

        // Start the machine
        m_safeValve.start();

        // Emit machine started
        emit emit_safeValveMachineStarted(id, true);
    }

    /**
     * Stops a running instance of close valve state machine
     *
     * @brief Machines::valveClose
     * @param id
     */
    void Machines::valveClose(int id)
    {
        // Set the params
        m_safeValve.setParams(id, false);

        // Build the machine
        m_safeValve.buildMachine();

        // Start the machine
        m_safeValve.start();

        // Emit machine started
        emit emit_safeValveMachineStarted(id, true);
    }


    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::valveStateFinished
     */
    void Machines::valveStateFinished(QVariantMap params)
    {

    }

    /**
     * This method is trigged if the state machine failed
     *
     * @brief Machine::valveStateFailed
     */
    void Machines::valveStateFailed(QVariantMap params)
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
