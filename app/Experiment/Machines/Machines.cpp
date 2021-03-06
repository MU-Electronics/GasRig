#include "Machines.h"

// Include external libs
#include <QDebug>

namespace App { namespace Experiment { namespace Machines
{

    Machines::Machines(QObject *parent, Settings::Container* settings, Hardware::Access& hardware, Safety::Monitor& safety)
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
        ,   m_vent(*new Vent(parent, settings, hardware, safety))
        ,   m_purge(*new Purge(parent, settings, hardware, safety))
        ,   m_continuousPressure(*new ContinuousPressure(parent, settings, hardware, safety))
    {
        // Connect the finishing signals for the continious pressurise state machine
        connect(&m_continuousPressure, &ContinuousPressure::emit_machineStopping, this, &Machines::continuousPressureStopping);
        connect(&m_continuousPressure, &ContinuousPressure::emit_machineFinished, this, &Machines::continuousPressureFinished);
        connect(&m_continuousPressure, &ContinuousPressure::emit_machineFailed, this, &Machines::continuousPressureFailed);

        // Connect the finished signals for the pruge
        connect(&m_purge, &Purge::emit_machineStopping, this, &Machines::purgeStopping);
        connect(&m_purge, &Purge::emit_machineFinished, this, &Machines::purgeFinished);
        connect(&m_purge, &Purge::emit_machineFailed, this, &Machines::purgeFailed);

        // Connect the finished signals for the vent
        connect(&m_vent, &Vent::emit_machineStopping, this, &Machines::ventStopping);
        connect(&m_vent, &Vent::emit_machineFinished, this, &Machines::ventFinished);
        connect(&m_vent, &Vent::emit_machineFailed, this, &Machines::ventFailed);

        // Connect the finished signals for the machine vac down
        connect(&m_vacDown, &VacDown::emit_machineStopping, this, &Machines::vacDownStopping);
        connect(&m_vacDown, &VacDown::emit_machineFinished, this, &Machines::vacDownFinished);
        connect(&m_vacDown, &VacDown::emit_machineFailed, this, &Machines::vacDownFailed);

        // Connect the finished signals for the machine safe valve
        connect(&m_safeValve, &SafeValve::emit_machineFinished, this, &Machines::safeValveFinished);
        connect(&m_safeValve, &SafeValve::emit_machineFailed, this, &Machines::safeValveFailed);
        connect(&m_safeValve, &SafeValve::emit_machineStopping, this, &Machines::safeValveStopping);

        // Connect the finished signals for the pulse valve
        connect(&m_pulseValve, &PulseValve::emit_machineStopping, this, &Machines::pulseValveStopping);
        connect(&m_pulseValve, &PulseValve::emit_machineFinished, this, &Machines::pulseValveFinished);
        connect(&m_pulseValve, &PulseValve::emit_machineFailed, this, &Machines::pulseValveFailed);

        // Connect the finished signals for the machine sensor readings
        connect(&m_readFlowControllerFlow, &ReadFlowControllerFlow::emit_machineFinished, this, &Machines::sensorReadingsFinished);
        connect(&m_readFlowControllerFlow, &ReadFlowControllerFlow::emit_machineFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readPressure, &ReadPressure::emit_machineFinished, this, &Machines::sensorReadingsFinished);
        connect(&m_readPressure, &ReadPressure::emit_machineFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readVacuum, &ReadVacuum::emit_machineFinished, this, &Machines::sensorReadingsFinished);
        connect(&m_readVacuum, &ReadVacuum::emit_machineFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readFlowControllerTemperatures, &ReadFlowControllerTemperatures::emit_machineFinished, this, &Machines::sensorReadingsFinished);
        connect(&m_readFlowControllerTemperatures, &ReadFlowControllerTemperatures::emit_machineFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readTurboSpeed, &ReadTurboSpeed::emit_machineFinished, this, &Machines::sensorReadingsFinished);
        connect(&m_readTurboSpeed, &ReadTurboSpeed::emit_machineFailed, this, &Machines::sensorReadingsFailed);
        connect(&m_readVacStationTemperatures, &ReadVacStationTemperatures::emit_machineFinished, this, &Machines::sensorReadingsFinished);
        connect(&m_readVacStationTemperatures, &ReadVacStationTemperatures::emit_machineFailed, this, &Machines::sensorReadingsFailed);

        // Connect the finished signals for the machine set pressure emit_pressuriseStopped
        connect(&m_pressurise, &Pressurise::emit_machineStopping, this, &Machines::pressuriseStopping);
        connect(&m_pressurise, &Pressurise::emit_machineFinished, this, &Machines::pressuriseFinished);
        connect(&m_pressurise, &Pressurise::emit_machineFailed, this, &Machines::pressuriseFailed);

        // Connect the finished signals for the machine set flow rate

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
        m_readFlowControllerFlow.cancelStateMachine();
        m_readPressure.cancelStateMachine();
        m_readVacuum.cancelStateMachine();
        m_readFlowControllerTemperatures.cancelStateMachine();
        m_readTurboSpeed.cancelStateMachine();
        m_readVacStationTemperatures.cancelStateMachine();

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
    int Machines::vacDown(double value, int valueType, bool turbo, int gasMode, int mode)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_vacDown.setParams(value, valueType, turbo, gasMode, mode);

        // Start the machine
        m_vacDown.start();

        // Emit machine started
        emit emit_vacDownMachineStarted(value, valueType, turbo, gasMode, mode);

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
        m_vacDown.cancelStateMachine();
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::vacDownFinished
     */
    void Machines::vacDownFinished(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_vacDownMachineStopped(params);
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::vacDownStopping
     */
    void Machines::vacDownStopping(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_vacDownMachineStopping(params);
    }

    /**
     * This method is trigged if the state machine failed
     *
     * @brief Machine::vacDownFailed
     */
    void Machines::vacDownFailed(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_vacDownMachineFailed(params);
    }








    /**
     * Starts the purge state machine
     *
     * @brief Machines::purge
     * @param outputValve
     * @param numberCycles
     * @param nitrogenPressure
     * @param vacTo
     * @return
     */
    int Machines::purge(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_purge.setParams(outputValve, numberCycles, nitrogenPressure, vacTo);

        // Start the machine
        m_purge.start();

        // Tell everyone
        emit emit_purgeStarted(outputValve, numberCycles, nitrogenPressure, vacTo);

        // Return success
        return 1;
    }

    /**
     * Stops a running instance of purge state machine
     *
     * @brief Machines::stopPurge
     */
    void Machines::stopPurge()
    {
        // Stop the machine
        m_purge.cancelStateMachine();
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::purgeFinished
     */
    void Machines::purgeFinished(QVariantMap params)
    {
        qDebug() << "Purge finished";
        // Emit machine stopped
        emit emit_purgeStopped(params);
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::purgeStopping
     */
    void Machines::purgeStopping(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_purgeStopping(params);
    }

    /**
     * This method is trigged if the state machine failed
     *
     * @brief Machine::purgeFailed
     */
    void Machines::purgeFailed(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_purgeFailed(params);
    }







    int Machines::setContinuousPressure(int maxTime, int monitorTime, double topUp, double leak, double pressure, int stepSize, bool inputValve, bool outputValve, bool exhuastValveOnly)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_continuousPressure.setParams(maxTime, monitorTime, topUp, leak, pressure, stepSize, inputValve, outputValve, exhuastValveOnly);

        // Start the machine
        m_continuousPressure.start();

        // Tell everyone
        emit emit_continuousPressureStarted(maxTime, monitorTime, topUp, leak, pressure, stepSize, inputValve, outputValve, exhuastValveOnly);

        // Return success
        return 1;
    }

    void Machines::stopSetContinuousPressure()
    {
        // Stop the machine
        m_continuousPressure.cancelStateMachine();
    }

    void Machines::continuousPressureStopping(QVariantMap params)
    {
        emit emit_continuousPressureStopping(params);
    }

    void Machines::continuousPressureFinished(QVariantMap params)
    {
        emit emit_continuousPressureStopped(params);
    }

    void Machines::continuousPressureFailed(QVariantMap params)
    {
        emit emit_continuousPressureFailed(params);
    }







    /**
     * Start a new vent state machine running
     *
     * @brief Machines::vent
     * @param output
     * @param vacuumOutput
     * @param flowCavity
     * @param nitrogenPipes
     * @param multiPipes
     * @param flowOnePipes
     * @param flowTwoPipes
     * @return
     */
    int Machines::vent(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_vent.setParams(output, vacuumOutput, flowCavity, nitrogenPipes, multiPipes, flowOnePipes, flowTwoPipes);

        // Start the machine
        m_vent.start();

        // Emit machine started
        emit emit_ventMachineStarted(output, vacuumOutput, flowCavity, nitrogenPipes, multiPipes, flowOnePipes, flowTwoPipes);

        // Return success
        return 1;
    }

    /**
     * Stops a running instance of vent state machine
     *
     * @brief Machines::stopVent
     */
    void Machines::stopVent()
    {
        // Stop the machine
        m_vent.cancelStateMachine();
    }

    void Machines::ventFinished(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_ventMachineStopped(params);
    }

    void Machines::ventStopping(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_ventMachineStopping(params);
    }

    void Machines::ventFailed(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_ventMachineFailed(params);
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
        m_pulseValve.cancelStateMachine();
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::vacDownFinished
     */
    void Machines::pulseValveFinished(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_pulseValveStopped(params);
    }

    /**
     * This method is trigged if the state machine finished
     *
     * @brief Machine::vacDownFinished
     */
    void Machines::pulseValveStopping(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_pulseValveStopping(params);
    }

    /**
     * This method is trigged if the state machine failed
     *
     * @brief Machine::vacDownFailed
     */
    void Machines::pulseValveFailed(QVariantMap params)
    {
        // Emit machine stopped
        emit emit_pulseValveFailed(params);
    }








    /**
     * Start a new set high pressure state machine running
     *
     * @brief Machines::setPressure
     * @param pressure
     * @param input
     * @param frequency
     */
    int Machines::setPressure(double pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve, bool exhuastValveOnly)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set params
        m_pressurise.setParams(pressure, initVacDown, stepSize, inputValve, openOutputValve, exhuastValveOnly);

        // Start the machine
        m_pressurise.start();

        // Emit machine started
        emit emit_pressuriseStarted(pressure, initVacDown, stepSize, inputValve, openOutputValve, exhuastValveOnly);

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
        // Stop the pulse valve machine
        m_pressurise.cancelStateMachine();
    }


    /**
     * Ran when the pressurise state machine has finished
     *
     * @brief Machines::pressuriseFinished
     * @param params
     */
    void Machines::pressuriseFinished(QVariantMap params)
    {
        emit emit_pressuriseStopped(params);
    }

    /**
     * Ran when the pressurise state machine has finished
     *
     * @brief Machines::pressuriseFinished
     * @param params
     */
    void Machines::pressuriseStopping(QVariantMap params)
    {
        emit emit_pressuriseStopping(params);
    }

    /**
     * Ran when the pressurise state machine has failed
     *
     * @brief Machines::pressuriseFailed
     * @param params
     */
    void Machines::pressuriseFailed(QVariantMap params)
    {
        emit emit_pressuriseFailed(params);
    }




    /**
     * Start a new open valve state machine running
     *
     * @brief Machines::valveOpen
     * @param id
     */
    int Machines::safeValveOpen(int id)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_safeValve.setParams(id, true);

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
    int Machines::safeValveClose(int id)
    {
        // This state machine requires to sensors to be monitored
        if(!sensorMonitors)
            return machineFailedToStart(-1);

        // Set the params
        m_safeValve.setParams(id, false);

        // Start the machine
        m_safeValve.start();

        // Emit machine started
        emit emit_safeValveMachineStarted(id, false);

        // Return success
        return 1;
    }

    /**
     * Ran when the pressurise state machine has finished
     *
     * @brief Machines::valveStateFinished
     * @param params
     */
    void Machines::safeValveFinished(QVariantMap params)
    {
        qDebug() << "Finished";
        emit emit_safeValveMachineStopped(params);
    }

    /**
     * Ran when the pressurise state machine has finished
     *
     * @brief Machines::pressuriseFinished
     * @param params
     */
    void Machines::safeValveStopping(QVariantMap params)
    {
        emit emit_safeValveMachineStopping(params);
    }

    /**
     * Ran when the pressurise state machine has failed
     *
     * @brief Machines::valveStateFailed
     * @param params
     */
    void Machines::safeValveFailed(QVariantMap params)
    {
        emit emit_safeValveMachineFailed(params);
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
