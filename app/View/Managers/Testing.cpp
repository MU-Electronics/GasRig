#include "Testing.h"

#include <cmath>

namespace App { namespace View { namespace Managers
{
    Testing::Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings, Experiment::Engine* experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings),
          m_experimentEngine(experimentEngine),
          m_commandConstructor(*new Hardware::CommandConstructor)
    {

    }


    void Testing::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &Testing::hardwareRequest, &hardware, &Hardware::Access::hardwareAccess);

        // Connect incomming signals to actions for the pressure sensor
        connect(&hardware, &Hardware::Access::emit_pressureSensorInit, this, &Testing::receivePressureSensorInit);
        connect(&hardware, &Hardware::Access::emit_pressureSensorPressure, this, &Testing::receivePressureSensorReading);
        connect(&hardware, &Hardware::Access::emit_pressureSensorSerialNumber, this, &Testing::receivePressureSensorSerialNumber);

        // Connect incomming signals to actions for the vac station
        connect(&hardware, &Hardware::Access::emit_setPumpingState, this, &Testing::receiveVacSetPump);
        connect(&hardware, &Hardware::Access::emit_setTurboPumpState, this, &Testing::receiveVacSetTurbo);
        connect(&hardware, &Hardware::Access::emit_setGasMode, this, &Testing::receiveVacSetGasMode);
        connect(&hardware, &Hardware::Access::emit_setBackingPumpMode, this, &Testing::receiveVacSetPumpMode);
        connect(&hardware, &Hardware::Access::emit_getTemperature, this, &Testing::receiveVacTemperature);
        connect(&hardware, &Hardware::Access::emit_getTurboSpeed, this, &Testing::receiveVacTurboSpeed);

        // Connect incomming signals to actions for the LabJack
        connect(&hardware, &Hardware::Access::emit_setValveState, this, &Testing::receiveValveStatus);
        connect(&hardware, &Hardware::Access::emit_configureIO, this, &Testing::receiveLabJackConfig);
        connect(&hardware, &Hardware::Access::emit_readVacuumPressure, this, &Testing::receiveVacuumReading);

        // Connect incomming signals to actions for the flow controllers
        connect(&hardware, &Hardware::Access::emit_setFlowControllerValveOverride, this, &Testing::receiveFlowControllerValveOverride);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerFlowRate, this, &Testing::receiveSetFlowControllerFlowRate);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStart, this, &Testing::receiveSetFlowControllerSoftStart);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStartTime, this, &Testing::receiveSetFlowControllerSoftStartTime);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSourceControl, this, &Testing::receiveSetFlowControllerSourceControl);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerFlowRate, this, &Testing::receiveFlowControllerFlowRate);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerTemperature, this, &Testing::receiveFlowControllerTemperature);

        // Connect the experiment function signals


        // Disable unused compiler warnings
        UNUSED(safety);
    }


    /**
     * Debug method for flow controller temperature
     *
     * @brief Testing::receiveFlowControllerTemperature
     * @param command
     */
    void Testing::receiveFlowControllerTemperature(QVariantMap command)
    {
        if(!isRequestedCommand("requestFlowControllerTemperature"))
            return;

        // Get the flow units
        auto flowUnits = m_settings->hardware()->flow_controller_units.value("temperature").toMap();
        QString unit = " " + flowUnits.value(command.value("temperature_unit").toString()).toString();

        // Display message
        emit emit_testingMaintenanceReply("The temperature is " + command.value("temperature").toString() + unit + " for "+ command.value("controller").toString());
    }

    /**
     * Debug method for flow controller current flow rate
     *
     * @brief Testing::receiveFlowControllerFlowRate
     * @param command
     */
    void Testing::receiveFlowControllerFlowRate(QVariantMap command)
    {
        if(!isRequestedCommand("requestFlowControllerFlowRate"))
            return;

        // Get the flow units
        auto flowUnits = m_settings->hardware()->flow_controller_units.value("flow").toMap();
        QString unit = " " + flowUnits.value(command.value("flow_unit").toString()).toString();

        // Display message
        emit emit_testingMaintenanceReply("The flow rate is "+ command.value("flow").toString() + unit + " for "+ command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set valve override
     *
     * @brief Testing::pressureSensorInit
     * @param command
     */
    void Testing::receiveFlowControllerValveOverride(QVariantMap command)
    {
        if(!isRequestedCommand("requestFlowControllerValveOverride"))
            return;

        emit emit_testingMaintenanceReply("Valve override set to " + command.value("override_verbal").toString() + " for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set flow rate
     *
     * @brief Testing::pressureSensorInit
     * @param command
     */
    void Testing::receiveSetFlowControllerFlowRate(QVariantMap command)
    {
        if(!isRequestedCommand("requestActualFlowControllerFlowRate"))
            return;

        emit emit_testingMaintenanceReply("Flow rate set to " + command.value("flow").toString() + " for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set soft start
     *
     * @brief SystemStatus::receiveSetFlowControllerSoftStart
     * @param command
     */
    void Testing::receiveSetFlowControllerSoftStart(QVariantMap command)
    {
        if(!isRequestedCommand("requestFlowControllerSoftStart"))
            return;

        QString softstart = "disabled";
        if(command.value("state").toInt() == 4)
            softstart = "linear ramp";

        emit emit_testingMaintenanceReply("Flow controller's soft start set to " + softstart + " for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set soft start time
     *
     * @brief SystemStatus::receiveSetFlowControllerSoftStartTime
     * @param command
     */
    void Testing::receiveSetFlowControllerSoftStartTime(QVariantMap command)
    {
        if(!isRequestedCommand("requestFlowControllerSoftStartTime"))
            return;

        emit emit_testingMaintenanceReply("Flow controller's soft start time constant set to " + command.value("seconds").toString() + " seconds for " + command.value("controller").toString());
    }

    /**
     * Debug method for flow controller set source
     *
     * @brief Testing::receiveSetFlowControllerSourceControl
     * @param command
     */
    void Testing::receiveSetFlowControllerSourceControl(QVariantMap command)
    {
        if(!isRequestedCommand("requestSetFlowControllerSourceControl"))
            return;

        QString source = "digital";
        if(command.value("source").toInt() == 1)
            QString source = "analogue";
        emit emit_testingMaintenanceReply("Flow controller's source set to " + source + " for " + command.value("controller").toString());

    }










    /**
     * Debug method for pressure sensor init
     *
     * @brief Testing::pressureSensorInit
     * @param command
     */
    void Testing::receivePressureSensorInit(QVariantMap command)
    {
        if(!isRequestedCommand("requestPressureConfirmation"))
            return;

        emit emit_testingMaintenanceReply("Pressure sensor init: " + command.value("hardware_correct").toString());
    }

    /**
     * Debug method for pressure sensor reading
     *
     * @brief Testing::pressureSensorReading
     * @param command
     */
    void Testing::receivePressureSensorSerialNumber(QVariantMap command)
    {
        if(!isRequestedCommand("requestPressureSerialNumber"))
            return;

        emit emit_testingMaintenanceReply("Pressure sensor serial number is: " + command.value("serial").toString());
    }

    /**
     * Debug method for pressure sensor serial number
     *
     * @brief Testing::pressureSensorReading
     * @param command
     */
    void Testing::receivePressureSensorReading(QVariantMap command)
    {
        if(!isRequestedCommand("requestPressureReading"))
            return;

        emit emit_testingMaintenanceReply("Pressure sensor was reading was: " + command.value("pressure").toString());
    }











    /**
     * Debug method for getting temperature of vac station
     *
     * @brief Testing::receiveVacTurboSpeed
     * @param command
     */
    void Testing::receiveVacTurboSpeed(QVariantMap command)
    {
        if(!isRequestedCommand("requestTurboSpeed"))
            return;

        emit emit_testingMaintenanceReply("The " + command.value("type_desc_verbal").toString() + " speed of the turbo is " + command.value("speed").toString() + " " + command.value("type_unit").toString());
    }

    /**
     * Debug method for getting temperature of vac station
     *
     * @brief Testing::receiveVacTemperature
     * @param command
     */
    void Testing::receiveVacTemperature(QVariantMap command)
    {
        if(!isRequestedCommand("requestTurboBearingTemperature"))
            return;

        emit emit_testingMaintenanceReply("The temperature of " + command.value("location_verbal").toString() + " is " + command.value("temperature").toString() + " Degrees Celsius");
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveVacSetGasMode(QVariantMap command)
    {
        if(!isRequestedCommand("requestGasMode"))
            return;

        emit emit_testingMaintenanceReply("The gas mode was set to: " + command.value("mode_verbal").toString());
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveVacSetPumpMode(QVariantMap command)
    {
        if(!isRequestedCommand("requestBackingPumpMode"))
            return;

        emit emit_testingMaintenanceReply("The pump mode was set to: " + command.value("mode_verbal").toString());
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetPump
     * @param command
     */
    void Testing::receiveVacSetPump(QVariantMap command)
    {
        if(!isRequestedCommand("requestBackingPump"))
            return;

        emit emit_testingMaintenanceReply("The pump state is: " + command.value("state_verbal").toString());
    }

    /**
     * Debug method for setting gas mode
     *
     * @brief Testing::receiveVacSetTurbo
     * @param command
     */
    void Testing::receiveVacSetTurbo(QVariantMap command)
    {
        if(!isRequestedCommand("requestTurboPump"))
            return;

        emit emit_testingMaintenanceReply("The turbo state is: " + command.value("state_verbal").toString());
    }












    /**
     * Debug method for setting the valve status
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveValveStatus(QVariantMap command)
    {
        if(!isRequestedCommand("requestValveState"))
            return;

        // Get the port name
        QString portNumber  = m_settings->hardware()->valve_connections.key(command.value("port").toString());

        if(!portNumber.isNull())
        {
            // Get verbal value
            QString value = "closed";
            if(command.value("value").toBool())
                value = "open";

            // Update the displays
            emit emit_testingMaintenanceReply("The status of valve " + portNumber + " was updated to " + value);
        }
    }

    /**
     * Debug method for setting the config
     *
     * @brief Testing::receiveVacSetGasMode
     * @param command
     */
    void Testing::receiveLabJackConfig(QVariantMap command)
    {
        if(!isRequestedCommand("requestLabJackConfig"))
            return;

        emit emit_testingMaintenanceReply("The LabJack config is: "/* + command.value("state_verbal").toString()*/);
    }

    /**
     * Debug method for reading the vacuum sensor
     *
     * @brief Testing::receiveVacuumReading
     * @param command
     */
    void Testing::receiveVacuumReading(QVariantMap command)
    {
        if(!isRequestedCommand("requestVacuumPressure"))
            return;

        // If port is the same as the vacuum guage port
        if(command["port"] == m_settings->hardware()->vacuum_guage.value("connection").toString())
        {
            double pressure = (std::pow(10, (1.667*command.value("calibrated").toDouble()-9.333)))/100;
            emit emit_testingMaintenanceReply("Vacuum pressure is: " + QString::number(pressure) + " mbar");
        }
    }













    /**
     * Request that the vacuum pump is turned on
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestBackingPump(bool onOff)
    {
        // Set the requested command
        setRequestedCommand("requestBackingPump");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPump(onOff));
    }

    /**
     * Request that the turbo pump is enabled
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestTurboPump(bool onOff)
    {
        // Set the requested command
        setRequestedCommand("requestTurboPump");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setTurboPump(onOff));
    }

    /**
     * Request a new backing pump mode
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestBackingPumpMode(int mode)
    {
        // Set the requested command
        setRequestedCommand("requestBackingPumpMode");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setBackingPumpMode(mode));
    }

    /**
     * Request to set a new gas mode
     *
     * @brief Testing::requestVacuum
     * @param onOff
     */
    void Testing::requestGasMode(int mode)
    {
        // Set the requested command
        setRequestedCommand("requestGasMode");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setGasMode(mode));
    }

    /**
     * Request the turbo speed
     *
     * @brief Testing::requestTurboSpeed
     */
    void Testing::requestTurboSpeed()
    {
        // Set the requested command
        setRequestedCommand("requestTurboSpeed");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboSpeed());
    }

    /**
     * Request the bearning temperature
     *
     * @brief Testing::requestTurboSpeed
     */
    void Testing::requestTurboBearingTemperature()
    {
        // Set the requested command
        setRequestedCommand("requestTurboBearingTemperature");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getTurboBearingTemperature());
    }













    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureConfirmation()
    {
        // Set the requested command
        setRequestedCommand("requestPressureConfirmation");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.pressureConfirmation());
    }

    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureReading()
    {
        // Set the requested command
        setRequestedCommand("requestPressureReading");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getPressureReading(1));
    }

    /**
     * Request that the pressure sensor init
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestPressureSerialNumber()
    {
        // Set the requested command
        setRequestedCommand("requestPressureSerialNumber");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getPressureSerialNumber());
    }















    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestLabJackConfig()
    {
        // Set the requested command
        setRequestedCommand("requestLabJackConfig");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setLabJackConfig(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    }


    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestValveState(int port, bool state)
    {
        // Set the requested command
        setRequestedCommand("requestValveState");

        // Find the correct valve name
        QString valveName = m_settings->hardware()->valve_connections.value(QString::number(port)).toString();

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setValveState(valveName, state));
    }



    /**
     * Request valve status change
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestVacuumPressure()
    {
        // Set the requested command
        setRequestedCommand("requestVacuumPressure");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getVacuumPressure( m_settings->hardware()->vacuum_guage.value("connection").toString(),
                                                                     m_settings->hardware()->vacuum_guage.value("slope").toDouble(),
                                                                     m_settings->hardware()->vacuum_guage.value("offset").toDouble()));
    }














    /**
     * Request current flow rate
     *
     * @brief Testing::requestActualFlowControllerFlowRate
     */
    void Testing::requestActualFlowControllerFlowRate(QString controller)
    {
        // Set the requested command
        setRequestedCommand("requestActualFlowControllerFlowRate");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerFlowRate(controller));
    }

    /**
     * Request current flow rate
     *
     * @brief Testing::requestFlowControllerTemperature
     */
    void Testing::requestFlowControllerTemperature(QString controller)
    {
        // Set the requested command
        setRequestedCommand("requestFlowControllerTemperature");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.getFlowControllerTemperature(controller));
    }

    /**
     * Request a valve override on the flow controllers
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerValveOverride(QString controller, int state)
    {
        // Set the requested command
        setRequestedCommand("requestFlowControllerValveOverride");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerValveOverride(controller, state));
    }

    /**
     * Request flow rate
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerFlowRate(QString controller, double flowrate)
    {
        // Set the requested command
        setRequestedCommand("requestFlowControllerFlowRate");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerFlowRate(controller, flowrate));
    }

    /**
     * Request flow rate soft start enable
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerSoftStart(QString controller, int state)
    {
        // Set the requested command
        setRequestedCommand("requestFlowControllerSoftStart");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerSoftStart(controller, state));
    }

    /**
     * Request flow rate soft start in seconds
     *
     * @brief Testing::requestPressureConfirmation
     */
    void Testing::requestFlowControllerSoftStartTime(QString controller, int seconds)
    {
        // Set the requested command
        setRequestedCommand("requestFlowControllerSoftStartTime");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerSoftStartTime(controller, seconds));
    }


    void Testing::requestSetFlowControllerSourceControl(QString controller, int source)
    {
        // Set the requested command
        setRequestedCommand("requestSetFlowControllerSourceControl");

        // Emit siganl to HAL
        emit hardwareRequest(m_commandConstructor.setFlowControllerSourceControl(controller, source));
    }












    void Testing::requestPurge(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo)
    {
        m_experimentEngine->machines().purge(outputValve, numberCycles, nitrogenPressure, vacTo);
    }

    void Testing::requestPurgeStop()
    {
        m_experimentEngine->machines().stopPurge();
    }





    void Testing::requestVent(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes)
    {
        m_experimentEngine->machines().vent(output, vacuumOutput, flowCavity, nitrogenPipes, multiPipes, flowOnePipes, flowTwoPipes);
    }

    void Testing::requestVentStop()
    {
        m_experimentEngine->machines().stopVent();
    }





    /**
     * Starts the pressurise state machine
     *
     * @brief Testing::requestHighPressure
     * @param pressure
     * @param initVacDown
     * @param stepSize
     * @param inputValve
     */
    void Testing::requestHighPressure(QString pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve, bool exhuastMethod,
                                      bool continiousPressure, int maxTime, int monitorTime, int setTop, int setLeak)
    {
        if(continiousPressure)
        {
            m_experimentEngine->machines().setContinuousPressure((maxTime*60000), (monitorTime*1000), setTop, setLeak, pressure.toInt(), stepSize, inputValve, openOutputValve, exhuastMethod);
            m_pressuriser = 2;
        }
        else
        {
            m_experimentEngine->machines().setPressure(pressure.toInt(), initVacDown, stepSize, inputValve, openOutputValve, exhuastMethod);
            m_pressuriser = 1;
        }
    }

    void Testing::requestHighPressureStop()
    {
        if(m_pressuriser == 2)
        {
            m_experimentEngine->machines().stopSetContinuousPressure();
        }
        else if(m_pressuriser == 1)
        {
            m_experimentEngine->machines().stopSetPressure();
        }

        m_pressuriser = 0;
    }




    /**
     * Starts the valve safe state machine
     *
     * @brief Testing::requestValveStateSafe
     * @param port
     * @param state
     */
    void Testing::requestValveStateSafe(int port, bool state)
    {
        if(state)
        {
            m_experimentEngine->machines().safeValveOpen(port);
        }
        else
        {
            m_experimentEngine->machines().safeValveClose(port);
        }
    }



    /**
     * Starts the vac down state machine
     *
     * @brief Testing::requestVacDown
     * @param mintues
     * @param turbo
     * @param gasMode
     * @param mode
     */
    void Testing::requestVacDown(double value, int valueType, bool turbo,int gasMode, int mode)
    {
        if(valueType == 1)
        {
            // How long the vac should run
            value = (value * 60) * 1000;
        }

        m_experimentEngine->machines().vacDown(value, valueType, turbo, gasMode, mode);
    }

    void Testing::requestVacDownStop()
    {
        m_experimentEngine->machines().stopVacDown();
    }



    /**
     * Starts the pulse valve state machine
     *
     * @brief Testing::requestPulseValve
     * @param valve
     * @param cycles
     * @param timeOpen
     * @param timeClosed
     */
    void Testing::requestPulseValve(int valve, int cycles, int timeOpen, int timeClosed)
    {
        m_experimentEngine->machines().pulseValve(valve, cycles, timeOpen, timeClosed);
    }

    void Testing::requestPulseValveStop()
    {
        m_experimentEngine->machines().stopPulseValve();
    }

}}}

