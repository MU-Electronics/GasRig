#include "Machines.h"

// Include external libs
#include <QDebug>

namespace App { namespace Experiment { namespace Machines
{

    Machines::Machines(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   QObject(parent)
        ,   m_vacDown(*new VacDown(parent, settings, hardware, safety))
        ,   m_safeValve(*new SafeValve(parent, settings, hardware, safety))
        ,   m_readFlowControllerFlow(*new ReadFlowControllerFlow(parent, settings, hardware, safety))
        ,   m_readPressure(*new ReadPressure(parent, settings, hardware, safety))
        ,   m_readVacuum(*new ReadVacuum(parent, settings, hardware, safety))
        ,   m_readFlowControllerTemperatures(*new ReadFlowControllerTemperatures(parent, settings, hardware, safety))
        ,   m_readTurboSpeed(*new ReadTurboSpeed(parent, settings, hardware, safety))
        ,   m_readVacStationTemperatures(*new ReadVacStationTemperatures(parent, settings, hardware, safety))
        ,   m_pulseValve(*new PulseValve(parent, settings, hardware, safety))
        ,   m_pressurise(*new Pressurise(parent, settings, hardware, safety))
    {
        // Connect the finished signals for the machine vac down
        connect(&m_vacDown, &VacDown::emit_vacDownFinished, this, &Machines::vacDownFinished);
        connect(&m_vacDown, &VacDown::emit_vacDownFailed, this, &Machines::vacDownFailed);

        // Connect the finished signals for the machine safe valve
        connect(&m_safeValve, &SafeValve::emit_safeValveFinished, this, &Machines::valveStateFinished);
        connect(&m_safeValve, &SafeValve::emit_safeValveFailed, this, &Machines::valveStateFailed);

        // Connect the finished signals for the pulse valve
        connect(&m_pulseValve, &PulseValve::emit_pulseValveFinished, this, &Machines::pulseValveFinished);
        connect(&m_pulseValve, &PulseValve::emit_pulseValveFinished, this, &Machines::pulseValveFailed);

        // Connect the finished signals for the machine sensor readings
        connect(&m_readFlowControllerFlow, &ReadFlowControllerFlow::emit_readFlowControllerFlowStopped, this, &Machines::sensorReadingsFinished);
        connect(&m_readFlowControllerFlow, &ReadFlowControllerFlow::emit_readFlowControllerFlowFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readPressure, &ReadPressure::emit_readPressureStopped, this, &Machines::sensorReadingsFinished);
        connect(&m_readPressure, &ReadPressure::emit_readPressureFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readVacuum, &ReadVacuum::emit_readVacuumStopped, this, &Machines::sensorReadingsFinished);
        connect(&m_readVacuum, &ReadVacuum::emit_readVacuumStopped, this, &Machines::sensorReadingsFailed);
        connect(&m_readFlowControllerTemperatures, &ReadFlowControllerTemperatures::emit_readFlowControllerTemperaturesStopped, this, &Machines::sensorReadingsFinished);
        connect(&m_readFlowControllerTemperatures, &ReadFlowControllerTemperatures::emit_readFlowControllerTemperaturesFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readTurboSpeed, &ReadTurboSpeed::emit_readTurboSpeedStopped, this, &Machines::sensorReadingsFinished);
        connect(&m_readTurboSpeed, &ReadTurboSpeed::emit_readTurboSpeedFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readVacStationTemperatures, &ReadVacStationTemperatures::emit_readVacStationTemperaturesStopped, this, &Machines::sensorReadingsFinished);
        connect(&m_readVacStationTemperatures, &ReadVacStationTemperatures::emit_readVacStationTemperaturesFailed, this, &Machines::sensorReadingsFailed);

        // Connect the finished signals for the machine purge system

        // Connect the finished signals for the machine exhuast system

        // Connect the finished signals for the machine set flow rate

        // Connect the finished signals for the machine output pressure

        // Connect the finished signals for the machine set high pressure
    }



    /**
     * Move state machines to there own thread
     *
     * @brief Machines::stateMachinesToThread
     */
    void Machines::stateMachinesToThread()
    {

    }


    /**
     * Helper method to produce error responces
     *
     * @brief Machines::machineFailedToStart
     * @param errorCode
     */
    int Machines::machineFailedToStart(int errorCode)
    {
        // Sensors need to be monitored for this state machine
        if(errorCode == -1)
        {
            emit emit_sensorsNotBeingMonitored();

            return -1;
        }

        return 0;
    }









    /**
     * Start a new sensor readings state machine running
     *
     * @brief Machines::sensorReadings
     * @param vacSensorTimeInter
     * @param pressureSensorTimeInter
     * @param flowControllerTimeInter
     */
    int Machines::sensorReadings(int vacSensorTimeInter, int pressureSensorTimeInter, int flowControllerTimeInter, int turboSpeedTimeInter, int vacStationTemperTimeInter, int flowControlTempTimeInter)
    {        
        // Set the params for the three sensor machines
        m_readFlowControllerFlow.setParams(flowControllerTimeInter);
        m_readPressure.setParams(pressureSensorTimeInter);
        m_readVacuum.setParams(vacSensorTimeInter);
        m_readFlowControllerTemperatures.setParams(flowControlTempTimeInter);
        m_readTurboSpeed.setParams(turboSpeedTimeInter);
        m_readVacStationTemperatures.setParams(vacStationTemperTimeInter);

        // Build the machines for the three sensors
        m_readFlowControllerFlow.buildMachine();
        m_readPressure.buildMachine();
        m_readVacuum.buildMachine();
        m_readFlowControllerTemperatures.buildMachine();
        m_readTurboSpeed.buildMachine();
        m_readVacStationTemperatures.buildMachine();

        // Start the machines for the three sensors
        m_readFlowControllerFlow.start();
        m_readPressure.start();
        m_readVacuum.start();
        m_readFlowControllerTemperatures.start();
        m_readTurboSpeed.start();
        m_readVacStationTemperatures.start();

        // Set the sensors to being monitored
        sensorMonitors = true;

        // Emit machines started
        emit emit_sensorReadingsMachineStarted(vacSensorTimeInter, pressureSensorTimeInter, flowControllerTimeInter, turboSpeedTimeInter, vacStationTemperTimeInter, flowControlTempTimeInter);

        // Return success
        return 1;
    }


    /**
     * Stops a running instance of sensor readings state machine
     *
     * @brief Machines::stopSensorReadings
     */
    void Machines::stopSensorReadings()
    {
        // Stop the machine
        m_readFlowControllerFlow.stop();
        m_readPressure.stop();
        m_readVacuum.stop();
        m_readFlowControllerTemperatures.stop();
        m_readTurboSpeed.stop();
        m_readVacStationTemperatures.stop();

        // Set the sensors to being monitored
        sensorMonitors = false;

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
    int Machines::vacDown(int mintues, bool turbo, int gasMode, int mode)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_vacDown.setParams(mintues, turbo, gasMode, mode);

        // Build the machine
        m_vacDown.buildMachine();

        // Start the machine
        m_vacDown.start();

        // Emit machine started
        emit emit_vacDownMachineStarted(mintues, turbo, gasMode, mode);

        // Return success
        return 1;
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
    int Machines::purgeSystemMethodOne(bool outputValve, int cycles, QString pressure)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // @todo Load machine here

        // Return success
        return 1;
    }


    /**
     * Start a new purge system state machine running with method two
     *
     * @brief Machines::purgeSystem
     * @param method
     * @param outputValve
     */
    int Machines::purgeSystemMethodTwo(int minutes, QString pressure)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // @todo Load machine here

        // Return success
        return 1;
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
    int Machines::exhuast(double pressure, int frequency, int speed)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // @todo Load machine here

        // Return success
        return 1;
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
     * Start a new exhuast state machine running
     *
     * @brief Machines::exhuast
     * @param frequency
     * @param speed
     */
    int Machines::pulseValve(int valve, int cycles, int timeOpen, int timeClosed)
    {
        // Set the params
        m_pulseValve.setParams(valve, cycles, timeOpen, timeClosed);

        // Build the machine
        m_pulseValve.buildMachine();

        // Start the machine
        m_pulseValve.start();

        // Emit machine started
        emit emit_pulseValveStarted(valve, cycles, timeOpen, timeClosed);

        // Return success
        return 1;
    }

    /**
     * Stops a running instance of pulse valve state machine
     *
     * @brief Machines::stopExhuast
     */
    void Machines::stopPulseValve()
    {
        // Stop the pulse valve machine
        m_pulseValve.stop();

        // Tell everyone we've stopped the machine
        emit emit_pulseValveStopped();
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::vacDownFinished
     */
    void Machines::pulseValveFinished(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_pulseValveStopped();
    }

    /**
     * This method is trigged if the state machine failed
     *
     * @brief Machine::vacDownFailed
     */
    void Machines::pulseValveFailed(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_pulseValveStopped();
    }








    /**
     * Start a new set high pressure state machine running
     *
     * @brief Machines::setPressure
     * @param pressure
     * @param input
     * @param frequency
     */
    int Machines::setPressure(double pressure, int input, int frequency)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Build the machine
        m_pressurise.buildMachine();

        // Start the machine
        m_pressurise.start();

        // Emit machine started
        emit emit_pressuriseStarted(pressure, input, frequency);

        // Return success
        return 1;
    }


    /**
     * Stops a running instance of  state machine
     *
     * @brief Machines::stopSetPressure
     */
    void Machines::stopSetPressure()
    {

    }




    /**
     * Start a new output pressure state machine running
     *
     * @brief Machines::outputPressure
     * @param frequency
     */
    int Machines::outputPressure(int frequency)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // @todo Load machine here

        // Return success
        return 1;
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
    int Machines::valveOpen(int id)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_safeValve.setParams(id, true);

        // Build the machine
        m_safeValve.buildMachine();

        // Start the machine
        m_safeValve.start();

        // Emit machine started
        emit emit_safeValveMachineStarted(id, true);

        // Return success
        return 1;
    }

    /**
     * Stops a running instance of close valve state machine
     *
     * @brief Machines::valveClose
     * @param id
     */
    int Machines::valveClose(int id)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_safeValve.setParams(id, false);

        // Build the machine
        m_safeValve.buildMachine();

        // Start the machine
        m_safeValve.start();

        // Emit machine started
        emit emit_safeValveMachineStarted(id, true);

        // Return success
        return 1;
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
    int Machines::setFlowRate(int flowController, double rate)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // @todo Load machine here

        // Return success
        return 1;
    }


    /**
     * Stops a running instance of flow rate state machine
     *
     * @brief Machines::stopFlowRate
     * @param flowController
     * @param rate
     */
    void Machines::stopFlowRate()
    {

    }

}}}
