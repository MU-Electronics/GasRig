#include "SystemStatus.h"

#include <cmath>

#include <QVariantMap>
#include <QString>
#include <QtGlobal>
#include <QTimer>


namespace App { namespace View { namespace Managers
{
    SystemStatus::SystemStatus(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings, Experiment::Engine* experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings),
          m_experimentEngine(experimentEngine),
          m_commandConstructor(*new Hardware::CommandConstructor)
    {
        // Default valves values
        m_valveState.insert("1", 0);
        m_valveState.insert("2", 0);
        m_valveState.insert("3", 0);
        m_valveState.insert("4", 0);
        m_valveState.insert("5", 0);
        m_valveState.insert("6", 0);
        m_valveState.insert("7", 0);
        m_valveState.insert("8", 0);
        m_valveState.insert("9", 0);

        // Pressure reading
        m_pressureSensor.insert("pressure", 0);
        m_pressureSensor.insert("pressure_round_3", 0);

        // Default vacuum states
        m_vacuumState.insert("backing_pump", 0);
        m_vacuumState.insert("turbo_pump", 0);
        m_vacuumState.insert("turbo_pump_speed", 0);
        m_vacuumState.insert("turbo_pump_speed_unit", 0);
        m_vacuumState.insert("backing_pump_mode", 0);
        m_vacuumState.insert("gas_type_mode", 1);
        m_vacuumState.insert("vacuum", 0);

        // Default flow controller values
        m_flowController.insert("controller_1_set_flowrate", 0);
        m_flowController.insert("controller_2_set_flowrate", 0);
        m_flowController.insert("controller_1_flow", 0);
        m_flowController.insert("controller_2_flow", 0);
        m_flowController.insert("controller_1_temperature", 0);
        m_flowController.insert("controller_2_temperature", 0);
        m_flowController.insert("controller_1_override", 0);
        m_flowController.insert("controller_2_override", 0);
        m_flowController.insert("controller_1_source", 0);
        m_flowController.insert("controller_2_source", 0);
        m_flowController.insert("controller_1_softstart", 0);
        m_flowController.insert("controller_2_softstart", 0);
        m_flowController.insert("controller_1_softstart_time", 0);
        m_flowController.insert("controller_2_softstart_time", 0);

        // Initial commands have not been sent yet
        initalCommands.insert("SensorMonitor", false);
        initalCommands.insert("PressureSensor", false);
        initalCommands.insert("FlowController", false);
        initalCommands.insert("VacStation", false);
        initalCommands.insert("LabJack", false);

    }

    void SystemStatus::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Connect object signals to hardware slots and visa versa
        connect(this, &SystemStatus::emit_hardwareRequest, &hardware, &Hardware::Access::hardwareAccess);



        // Labjack statuses
        connect(&hardware, &Hardware::Access::emit_setValveState, this, &SystemStatus::receiveValveStatus);
        connect(&hardware, &Hardware::Access::emit_readVacuumPressure, this, &SystemStatus::receiveVacuumPressure);



        // Pressure sensor
        m_pressureSensor.insert("vacuum", 0);
        connect(&hardware, &Hardware::Access::emit_pressureSensorPressure, this, &SystemStatus::receivePressureSensorPressure);


        // Vacuum status
        connect(&hardware, &Hardware::Access::emit_setPumpingState, this, &SystemStatus::receiveVacSetPump);
        connect(&hardware, &Hardware::Access::emit_setTurboPumpState, this, &SystemStatus::receiveVacSetTurbo);
        connect(&hardware, &Hardware::Access::emit_setGasMode, this, &SystemStatus::receiveVacSetGasMode);
        connect(&hardware, &Hardware::Access::emit_setBackingPumpMode, this, &SystemStatus::receiveVacSetPumpMode);

        connect(&hardware, &Hardware::Access::emit_getGasMode, this, &SystemStatus::receiveVacSetGasMode);
        connect(&hardware, &Hardware::Access::emit_getBackingPumpMode, this, &SystemStatus::receiveVacSetPumpMode);
        connect(&hardware, &Hardware::Access::emit_getTurboSpeed, this, &SystemStatus::receiveVacGetTurboSpeed);



        // Flow controller statuses
        connect(&hardware, &Hardware::Access::emit_setFlowControllerValveOverride, this, &SystemStatus::receiveSetFlowControllerValveOverride);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerFlowRate, this, &SystemStatus::receiveSetFlowControllerFlowRate);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStart, this, &SystemStatus::receiveSetFlowControllerSoftStart);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSoftStartTime, this, &SystemStatus::receiveSetFlowControllerSoftStartTime);
        connect(&hardware, &Hardware::Access::emit_setFlowControllerSourceControl, this, &SystemStatus::receiveSetFlowControllerSourceControl);

        connect(&hardware, &Hardware::Access::emit_getFlowControllerFlowRate, this, &SystemStatus::receiveFlowControllerFlowRate);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerTemperature, this, &SystemStatus::receiveFlowControllerTemperature);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerValveOverride, this, &SystemStatus::receiveSetFlowControllerValveOverride);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerSetFlowRate, this, &SystemStatus::receiveSetFlowControllerFlowRate);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerSoftStart, this, &SystemStatus::receiveSetFlowControllerSoftStart);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerSoftStartTime, this, &SystemStatus::receiveSetFlowControllerSoftStartTime);
        connect(&hardware, &Hardware::Access::emit_getFlowControllerSourceControl, this, &SystemStatus::receiveSetFlowControllerSourceControl);




        // Listen for com port connection signals
        connect(&hardware, &Hardware::Access::emit_serialComUpdated, this, &SystemStatus::setInitialValues);




        // Listen for debugging messages
        connect(&Services::Debugger::getInstance(), &Services::Debugger::emit_logChanged, this, &SystemStatus::logChanged);

        // Set log location
        logLocation(Services::Debugger::getInstance().log());

        // Disable unused compiler warnings
        UNUSED(safety);
    }



    /**
     * Generates the debug message from the array of messages
     *
     * @brief SystemStatus::debugMessages
     * @return
     */
    QString SystemStatus::debugMessages()
    {
        QString message = "";
        for (int i = 0; i < m_debugMessages.size(); ++i)
        {
            QString color = ( i % 2 == 0)? "white" : "#9b9b9b" ;

            message += "<div>";
            // Create message string
            QMapIterator<QString, QString> messageIterator(m_debugMessages[i]);
            while (messageIterator.hasNext()) {
                // Get data
                messageIterator.next();

                // Create string
                if(messageIterator.key() != "0")
                {
                    message += "<font color='#ed6110' size='3'>[" + messageIterator.key() +"]</font> <font color='#0054fc' size='3'>" + messageIterator.value() + "</font><br>";
                }
                else if(messageIterator.key() == "0")
                {
                    message += "<b><font color='#fc4300' size='4'>" + QString::number(i + 1) + ": " + messageIterator.value() + "</font></b><br>";
                }
            }

            // Clsoe div and addtion break at end of string
            message += "</div>";
        }

        return message;
    }


    /**
     * Triggered when a new debugging log is added
     *
     * @brief SystemStatus::logChanged
     * @param message
     */
    void SystemStatus::logChanged(QMap<QString, QString> message)
    {
        // Add message to list
        m_debugMessages.append(message);

        // Update every one
        emit emit_debugMessagesChanged(m_debugMessages);
    }








    void SystemStatus::setInitialValues(QVariantMap package)
    {

        // LabJack commands
        if(package.value("status").toBool() && package["responsability"].toString() == "LabJack" && !initalCommands.value("LabJack"))
        {
            // Ensure lab jack configured
            emit emit_hardwareRequest(m_commandConstructor.setLabJackConfig(0,0,0,0,0,0,0,0,0,0,0,0));
            // Get current values for vacuum
            emit emit_hardwareRequest(m_commandConstructor.getVacuumPressure(   m_settings->hardware()->vacuum_guage.value("connection").toString(),
                                                                                m_settings->hardware()->vacuum_guage.value("slope").toDouble(),
                                                                                m_settings->hardware()->vacuum_guage.value("offset").toDouble()));
            // Ensure all valves are closed
            for(int i=1; i<=9; i++)
                emit emit_hardwareRequest(m_commandConstructor.setValveState(m_settings->hardware()->valve_connections.value(QString::number(i)).toString(), false));

            // Inital commands sent
            initalCommands.insert("LabJack", true);
        }

        // Turbo pump commands
        if(package.value("status").toBool() && package["responsability"].toString() == "VacStation" && !initalCommands.value("VacStation"))
        {
            // Disable turbo and backing pump
            emit emit_hardwareRequest(m_commandConstructor.setBackingPump(false));
            emit emit_hardwareRequest(m_commandConstructor.setTurboPump(false));
            // Get the currently backing pump and gas type mode from vac station
            emit emit_hardwareRequest(m_commandConstructor.getGasMode());
            emit emit_hardwareRequest(m_commandConstructor.getBackingPumpMode());

            // Inital commands sent
            initalCommands.insert("VacStation", true);
        }


        // Flow controller one commands
        if(package.value("status").toBool() && package["responsability"].toString() == "FlowController" && !initalCommands.value("FlowController"))
        {
            // Ensure the flow controllers are set to closed
            emit emit_hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerOne", 2));
            emit emit_hardwareRequest(m_commandConstructor.setFlowControllerValveOverride("FlowControllerTwo", 2));
            // Get the flow controllers set flow rates
            emit emit_hardwareRequest(m_commandConstructor.setFlowControllerFlowRate("FlowControllerOne", 0));
            emit emit_hardwareRequest(m_commandConstructor.setFlowControllerFlowRate("FlowControllerTwo", 0));
            // Get the flow controllers soft start enabled / disabled
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerSoftStart("FlowControllerOne"));
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerSoftStart("FlowControllerTwo"));
            // Get the flow controllers soft start times
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerSoftStartTime("FlowControllerOne"));
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerSoftStartTime("FlowControllerTwo"));
            // Get the source of control for the flow controller
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerSourceControl("FlowControllerOne"));
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerSourceControl("FlowControllerTwo"));
            // Get the temperature of the flow controller
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerTemperature("FlowControllerOne"));
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerTemperature("FlowControllerTwo"));
            // Get the actual flow for the flow controllers
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerFlowRate("FlowControllerOne"));
            emit emit_hardwareRequest(m_commandConstructor.getFlowControllerFlowRate("FlowControllerTwo"));

            // Inital commands sent
            initalCommands.insert("FlowController", true);
        }

        // Pressure sensor command
        if(package.value("status").toBool() && package["responsability"].toString() == "PressureSensor" && !initalCommands.value("PressureSensor"))
        {
            // Pressure sensor likes time to settle after initialisation
            QTimer::singleShot(1000, [this]() {
                // Get current values for pressure
                emit emit_hardwareRequest(m_commandConstructor.getPressureReading(1));
            });

            // Inital commands sent
            initalCommands.insert("PressureSensor", true);
        }

        // Start monitoring the sensors
        if(!initalCommands.value("SensorMonitor") && package.value("status").toBool())
        {
            m_experimentEngine->machines().sensorReadings(m_settings->hardware()->polling_times.value("vacuum_sensor").toInt(),
                                                         m_settings->hardware()->polling_times.value("pressure_sensor").toInt(),
                                                         m_settings->hardware()->polling_times.value("flow_controller_flow").toInt(),
                                                         m_settings->hardware()->polling_times.value("turbo_speed").toInt(),
                                                         m_settings->hardware()->polling_times.value("vacuum_station_temperatures").toInt(),
                                                         m_settings->hardware()->polling_times.value("flow_controller_temperatures").toInt());

            // Inital commands sent
            initalCommands.insert("SensorMonitor", true);
        }
    }







    /**
     * Update the most recent pressure reading
     *
     * @brief SystemStatus::receivePressureSensorPressure
     * @param package
     */
    void SystemStatus::receivePressureSensorPressure(QVariantMap package)
    {
        // Update the pressure @todo moving average
        m_pressureSensor["pressure"] = package["pressure"];
        m_pressureSensor["pressure_round_3"] = QString::number(package["pressure"].toFloat(), 'e', 3);

        // Tell the views we've updated
        emit_pressureSensorChanged(m_pressureSensor);
    }







    /**
     * Check if a digital port update is relivent to a valve port and if so update the display
     *
     * @brief ConnectionStatus::receiveValveStatus
     * @param package
     */
    void SystemStatus::receiveValveStatus(QVariantMap package)
    {
        // Get the port name
        QString portNumber  = m_settings->hardware()->valve_connections.key(package.value("port").toString());

        if(!portNumber.isNull())
        {
            // Update valve value
            m_valveState[portNumber] = package.value("value").toInt();

            // Update the displays
            emit_valveStateChanged(m_valveState);
        }

    }







    /**
     * Sets the gas mode varaible and informs views
     *
     * @brief SystemStatus::receiveVacSetGasMode
     * @param command
     */
    void SystemStatus::receiveVacSetGasMode(QVariantMap command)
    {
        // Get gas mode
        int gasMode = command.value("mode").toInt();

        // Update the mode
        m_vacuumState["gas_type_mode"] = gasMode;

        if(gasMode == 0) {
            m_vacuumState["gas_type_mode_verbal"] = ">= 39";
        }
        else if(gasMode == 1)
        {
            m_vacuumState["gas_type_mode_verbal"] = "< 39";
        }
        else if(gasMode == 2)
        {
            m_vacuumState["gas_type_mode_verbal"] = "helium";
        }

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Sets the pump mode variable and informs views
     *
     * @brief SystemStatus::receiveVacSetPumpMode
     * @param command
     */
    void SystemStatus::receiveVacSetPumpMode(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["backing_pump_mode"] = command.value("mode").toInt();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Setsn the pumping state variable and informs views
     *
     * @brief SystemStatus::receiveVacSetPump
     * @param command
     */
    void SystemStatus::receiveVacSetPump(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["backing_pump"] = command.value("state").toInt();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Sets the turbo state variable and informs views
     *
     * @brief SystemStatus::receiveVacSetTurbo
     * @param command
     */
    void SystemStatus::receiveVacSetTurbo(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["turbo_pump"] = command.value("state").toInt();

        if(command.value("state").toInt() == 1)
        {
            m_vacuumState["turbo_pump_verbal"] = "enabled";
        }
        else
        {
            m_vacuumState["turbo_pump_verbal"] = "disabled";
        }

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }


    /**
     * Sets the turbo state variable and informs views
     *
     * @brief SystemStatus::receiveVacGetTurboSpeed
     * @param command
     */
    void SystemStatus::receiveVacGetTurboSpeed(QVariantMap command)
    {
        // Update the mode
        m_vacuumState["turbo_pump_speed"] = command.value("speed").toDouble();
        m_vacuumState["turbo_pump_speed_unit"] = command.value("location_verbal").toString();

        // Update the display
        emit_vacuumStateChanged(m_vacuumState);
    }

    /**
     * Sets the vacuum pressure variable and informs views
     *
     * @brief SystemStatus::receiveVacuumPressure
     * @param command
     */
    void SystemStatus::receiveVacuumPressure(QVariantMap command)
    {
        // If port is the same as the vacuum guage port
        if(command["port"] == m_settings->hardware()->vacuum_guage.value("connection").toString())
        {
            // Update vacuum reading
            double pressure = (std::pow(10, (1.667*command.value("calibrated").toDouble()-9.333)))/100;
            m_vacuumState["vacuum"] = pressure;
            m_vacuumState["vacuum_round_3"] = QString::number(pressure, 'e', 3);

            // Update the display
            emit_vacuumStateChanged(m_vacuumState);
        }
    }







    void SystemStatus::receiveFlowControllerTemperature(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the flowrate
            m_flowController["controller_1_temperature"] = command.value("temperature").toDouble();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the flowrate
            m_flowController["controller_2_temperature"] = command.value("temperature").toDouble();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveFlowControllerFlowRate(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the flowrate
            m_flowController["controller_1_flow"] = command.value("flow").toDouble();
            m_vacuumState["controller_1_flow_round_3"] = QString::number(command.value("flow").toDouble(), 'e', 3);
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the flowrate
            m_flowController["controller_2_flow"] = command.value("flow").toDouble();
            m_vacuumState["controller_2_flow_round_3"] = QString::number(command.value("flow").toDouble(), 'e', 3);
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }
    void SystemStatus::receiveSetFlowControllerValveOverride(QVariantMap command)
    {
        int override = command.value("override").toInt();
        QString controller = "1";
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_override"] = override;
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_override"] = override;
            controller = "2";
        }

        // Add the vebal meaning / result
        if(override == 0)
        {
            m_flowController["controller_"+controller+"_override_verbal"] = "off";
        }
        else if(override == 1)
        {
            m_flowController["controller_"+controller+"_override_verbal"] = "open";
        }
        else if(override == 2)
        {
            m_flowController["controller_"+controller+"_override_verbal"] = "closed";
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerFlowRate(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_set_flowrate"] = command.value("flow").toDouble();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_set_flowrate"] = command.value("flow").toDouble();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerSoftStart(QVariantMap command)
    {

        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_softstart"] = command.value("state").toInt();
            if(command.value("state").toInt() == 4){ m_flowController["controller_1_softstart_verbal"] = "enabled"; }else{ m_flowController["controller_1_softstart_verbal"] = "disabled"; }
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_softstart"] = command.value("state").toInt();
            if(command.value("state").toInt() == 4){ m_flowController["controller_2_softstart_verbal"] = "enabled"; }else{ m_flowController["controller_2_softstart_verbal"] = "disabled"; }

        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerSoftStartTime(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_softstart_time"] = command.value("seconds").toInt();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_softstart_time"] = command.value("seconds").toInt();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }

    void SystemStatus::receiveSetFlowControllerSourceControl(QVariantMap command)
    {
        // Select controller
        if(command["controller"] == "FlowControllerOne")
        {
            // Update the mode
            m_flowController["controller_1_source"] = command.value("source").toInt();
        }
        else if(command["controller"] == "FlowControllerTwo")
        {
            // Update the mode
            m_flowController["controller_2_source"] = command.value("source").toInt();
        }

        // Update the display
        emit_flowControllerStateChanged(m_flowController);
    }


}}}


