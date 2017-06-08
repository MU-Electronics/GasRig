#include "Access.h"

// Include external libs
#include <QObject>
#include <QThread>
#include <QDebug>
#include <QMetaObject>
#include <QVariantMap>
#include <QVariant>
#include <QMapIterator>

// Include settings container
#include "../Settings/container.h"

// Include Hals
#include "HAL/VacStation.h"
#include "HAL/FlowController.h"
#include "HAL/PressureSensor.h"
#include "HAL/Labjack.h"

// Include presenters
#include "HAL/Presenters/PressureSensorPresenter.h"

namespace App { namespace Hardware
{

    /**
     * Class constructure performs:
     *      - Set parent thread class
     *      - Set settings member variable
     *
     * @brief Monitor::Monitor
     * @param parent
     */
    Access::Access(QObject *parent, Settings::Container settings)
        :   Thread(parent, false, false),

            // System settings container
            m_settings(settings),

            // Timer for time based events
            m_timer(*new QTimer(this)),

            // HAL objects
            m_vacStation(*new HAL::VacStation(this, 1)),
            m_flowController(*new HAL::FlowController(this)),
            m_pressureSensor(*new HAL::PressureSensor(this, 1)),
            m_labjack(*new HAL::LabJack(this)),

           // HAL Presenters
           m_pressurePresenter(*new HAL::Presenters::PressureSensorPresenter(this))
    {
        // Set possable methods to be ran within this class via the queue
        // None atm set like this: m_avaliableMethods.append("<method name>");
    }


    /**
     * Class detructure performs:
     *      - Nothing atm
     *
     * @brief Monitor::~Monitor
     */
    Access::~Access()
    {

    }


    /**
     * Allows for any configurations to be ran before the worker is set going
     *
     * @brief Access::configure
     */
    void Access::configure(QThread &thread)
    {
        // Connect vac station HAL connections
        connect(&m_vacStation, &HAL::VacStation::emit_vacStationData, this, &Access::proccessDataFromHals);
        connect(&m_vacStation, &HAL::VacStation::emit_comConnectionStatus, this, &Access::listen_serialComUpdates);
        connect(&m_vacStation, &HAL::VacStation::emit_critialSerialError, this, &Access::listen_critialSerialError);
        connect(&m_vacStation, &HAL::VacStation::emit_timeoutSerialError, this, &Access::listen_timeoutSerialError);

        // Connect pressure sensor HAL connections
        connect(&m_pressureSensor, &HAL::PressureSensor::emit_pressureSensorData, this, &Access::proccessDataFromHals);
        connect(&m_pressureSensor, &HAL::PressureSensor::emit_comConnectionStatus, this, &Access::listen_serialComUpdates);
        connect(&m_pressureSensor, &HAL::PressureSensor::emit_critialSerialError, this, &Access::listen_critialSerialError);
        connect(&m_pressureSensor, &HAL::PressureSensor::emit_timeoutSerialError, this, &Access::listen_timeoutSerialError);
        connect(&m_pressureSensor, &HAL::PressureSensor::emit_comConnectionStatus, &m_pressureSensor, &HAL::PressureSensor::connectInitSensor);


        // Connect flow controller HAL connections
        connect(&m_flowController, &HAL::FlowController::emit_flowControllerData, this, &Access::proccessDataFromHals);
        connect(&m_flowController, &HAL::FlowController::emit_comConnectionStatus, this, &Access::listen_serialComUpdates);
        connect(&m_flowController, &HAL::FlowController::emit_critialSerialError, this, &Access::listen_critialSerialError);
        connect(&m_flowController, &HAL::FlowController::emit_timeoutSerialError, this, &Access::listen_timeoutSerialError);

        // Connect labjack HAL connections
        connect(&m_labjack, &HAL::LabJack::emit_labJackData, this, &Access::proccessDataFromHals);
        connect(&m_labjack, &HAL::LabJack::emit_comConnectionStatus, this, &Access::listen_serialComUpdates);
        connect(&m_labjack, &HAL::LabJack::emit_critialLabJackError, this, &Access::listen_critialSerialError);
        connect(&m_labjack, &HAL::LabJack::emit_timeoutLabJackError, this, &Access::listen_timeoutSerialError);

        // Open com port for all devices
        connectDevices();
    }


    void Access::connectDevices()
    {
        // Open the com port for the vac station
        if(!m_vacStation.isOpen())
        {
            // Get connection data
            QVariantMap vacData = m_settings.hardware.usb_connections.value("vacuum_station").toMap();

            // Set vac pump id from settings
            m_vacStation.setId(vacData["id"].toInt());

            // Is com port provided?
            QString comport = vacData["com"].toString();
            if(comport.isNull())
            {
                // Find the com port
                comport = m_vacStation.findPortName(vacData["productId"].toInt(), vacData["vendorId"].toInt());
            }

            // Connect to port
            m_vacStation.open(comport, vacData["braud"].toInt(), vacData["timeout"].toInt()); // Will open on tty.usbserial-AH02FNCX for my mac
        }


        // Open com port for the pressure sensor
        if(!m_pressureSensor.isOpen())
        {
            // Get connection data
            QVariantMap pressureData = m_settings.hardware.usb_connections.value("pressure_sensor").toMap();

            // Set vac pump id from settings
            m_pressureSensor.setId(pressureData["id"].toInt());

            // Is com port provided?
            QString pressureComPort = pressureData["com"].toString();
            if(pressureComPort.isNull())
            {
                // Find the com port
                pressureComPort = m_pressureSensor.findPortName(pressureData["productId"].toInt(), pressureData["vendorId"].toInt());
            }

            // Connect to port
            m_pressureSensor.open(pressureComPort, pressureData["braud"].toInt(), pressureData["timeout"].toInt()); // Will open on tty.usbserial-AH02FNCX for my mac
        }

        // Open com port for the flow controllers
        if(!m_flowController.isOpen())
        {
            // Get connection data
            QVariantMap flowData = m_settings.hardware.usb_connections.value("flow_controller").toMap();

            // Is com port provided?
            QString flowComPort = flowData["com"].toString();
            if(flowComPort.isNull())
            {
                // Find the com port
                flowComPort = m_flowController.findPortName(flowData["productId"].toInt(), flowData["vendorId"].toInt());
            }

            // Connect to port
            m_flowController.open(flowComPort, flowData["braud"].toInt(), flowData["timeout"].toInt(), 3); // Will open on tty.usbserial-AH02FNCX for my mac

            // Register flow controller one with the class
            m_flowController.registerController("FlowControllerOne", flowData["1_manufacturer"].toString(), flowData["1_type"].toString(), flowData["1_id_1"].toString(), flowData["1_id_2"].toString(), flowData["1_id_3"].toString());

            // Register flow controller two with the class
            m_flowController.registerController("FlowControllerTwo", flowData["2_manufacturer"].toString(), flowData["2_type"].toString(), flowData["2_id_1"].toString(), flowData["2_id_2"].toString(), flowData["2_id_3"].toString());
        }


        // Open the com port for the labjack
        m_labjack.open();
    }


    /**
     * The thread worker that will be in a forever loop untill stopped
     * via the thread_abort parent member variable
     *
     * @brief Monitor::worker
     */
    void Access::worker()
    {
        // Check connections are open
        // This cause a never ending loop when cant connect
        // Was dicussed at work and autorecover is not desired initally
        // So is the wrong thing to do any way for now
        // In future just time stamp to check ever X seconds
        //connectDevices();

        // Check if any commands need to be ran
        if(m_queue.isEmpty())
            return;

        // Run the command
        executeHalMethods(m_queue.head());

        // Remove item from queue
        m_queue.dequeue();

        // Sleep the thread for a while
        thread()->usleep(1);
    }


    /**
     * Runs the correct method in the correct HAL object for a given command object
     *
     * @brief Access::executeHalMethods
     * @param command
     */
    void Access::executeHalMethods(QVariantMap command)
    {
        // Get the hardware and method
        QString hardware = command.value("hardware").toString();
        QString method = command.value("method").toString();

        // Status package
        QVariantMap status;

        // Attach current commands to status package
        QMapIterator<QString, QVariant> i(command);

        // While iterator has data loop
        while (i.hasNext())
        {
            // Move to next record
            i.next();

            // Store the current record data
            status[i.key()] = i.value();
        }

        // Set results to be false as we have none yet
        status["resulting_status"] = false;

        // Find the correct HAL
        if(hardware == "VacStation")
        {
            // If the bus is not free we cant procceed
            if((!m_vacStation.busFree() || !m_vacStation.isOpen()) && method != "resetConnection")
            {
                // Re add the method to the queue as this one will be removed
                m_queue.enqueue(command);

                // Log the issue
                qDebug() << "Command was relisted due to bus issue: " << command;

                // Return back to worker for next method
                return;
            }

            // Set the method params
            m_vacStation.setParams(command);

            // Run the method in the HAL and cache the status
            status["resulting_status"] = (QMetaObject::invokeMethod(&m_vacStation, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;

        }
        else if(hardware == "PressureSensor")
        {
            // If the bus is not free we cant procceed
            if((!m_pressureSensor.busFree() || !m_pressureSensor.isOpen()) && method != "resetConnection")
            {
                // Re add the method to the queue as this one will be removed
                m_queue.enqueue(command);

                // Log the issue
                qDebug() << "Command was relisted due to bus issue: " << command;

                // Return back to worker for next method
                return;
            }

            // Set the method params
            m_pressureSensor.setParams(command);

            // Run the method in the HAL and cache the status
            status["resulting_status"] = (QMetaObject::invokeMethod(&m_pressureSensor, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;
        }
        else if (hardware  == "FlowController")
        {
            // If the bus is not free we cant procceed
            if((!m_flowController.busFree() || !m_flowController.isOpen()) && method != "resetConnection")
            {
                // Re add the method to the queue as this one will be removed
                m_queue.enqueue(command);

                // Log the issue
                qDebug() << "Command was relisted due to bus issue: " << command;

                // Return back to worker for next method
                return;
            }

            // Set the method params
            m_flowController.setParams(command);

            // Run the method in the HAL and cache the status
            status["resulting_status"] = (QMetaObject::invokeMethod(&m_flowController, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;
        }
        else if (hardware == "LabJack")
        {
            // If the bus is not free we cant procceed
            if((!m_labjack.isOpen) && method != "resetConnection")
            {
                // Re add the method to the queue as this one will be removed
                m_queue.enqueue(command);

                // Log the issue
                qDebug() << "Command was relisted due to bus issue: " << command;

                // Return back to worker for next method
                return;
            }

            // Set the method params
            m_labjack.setParams(command);

            // Run the method in the HAL and cache the status
            status["resulting_status"] = (QMetaObject::invokeMethod(&m_labjack, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;
        }
        else if (hardware == "AccessLayer")
        {
            // If command is allowed to be ran
            if(m_avaliableMethods.contains(method))
            {
                // Store command data
                m_lastcommands = command;

                // Allow certain methods in this call to be called, main for management of USB connections
                status["resulting_status"] = (QMetaObject::invokeMethod(this, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;
            }
            else
            {
                // Error
                qDebug() << "Method not allow or does not exsist in Access class: " << command;

                // Failed to run method not allowed
                status["resulting_status"]  = false;
            }
        }

        // Emit the result
        emit_methodAttemptResults(status);
    }


    /**
     * This slot allows other proccess to add commands to be ran into the queue
     *
     * @brief Access::hardwareAccess
     * @param method
     * @param commands
     */
    void Access::hardwareAccess(QVariantMap command)
    {
        if(command.contains("hardware") && command.contains("method"))
        {
            // Add to the queue
            m_queue.enqueue(command);
        }
    }


    /**
     * Takes data from the HALs converts it to useable data via the HAL presenters and
     * then inkokes the correct slot+signal for the rest of the application to connect to
     *
     * @brief Access::proccessDataFromHals
     * @param responable
     * @param method
     * @param halData
     */
    void Access::proccessDataFromHals(QString responable, QString method, QStringList halData)
    {
        // Hold the presenters data
        QVariantMap package;

        // Format the data from the HAL package to useable data for the rest of the application
        // Pressure sensor presenter
        if(responable == "PressureSensor")
            package = m_pressurePresenter.proccess(method, halData);

        // Vac station presenter


        // Flow controller presenter


        // Labjack presenter


        // Once the data is formatted run the correct signal
        if(!package.isEmpty())
            // Trigger the correct access class signal
            QMetaObject::invokeMethod(this, package["method"].toString().toLatin1().data(), Qt::DirectConnection, Q_ARG( QVariantMap, package ));
    }







    /**
     * Listen for new pressure readings and emits the new data
     *
     * @brief Access::listen_pressureSensorOne
     * @param command
     */
    void Access::listen_pressureSensorOne(QVariantMap command)
    {
        qDebug() << "I'll be sending that signal for you";
    }







    /**
     * Listen to all com updates and emit relivent onces
     *
     * @brief Access::listen_serialComUpdates
     * @param command
     */
    void Access::listen_serialComUpdates(QVariantMap command)
    {
        emit emit_serialComUpdated(command);
    }


    /**
     * @brief Access::listen_critialSerialError
     * @param command
     */
    void Access::listen_critialSerialError(QVariantMap command)
    {
        emit emit_critialSerialError(command);
    }


    /**
     * @brief Access::listen_timeoutSerialError
     * @param command
     */
    void Access::listen_timeoutSerialError(QVariantMap command)
    {
        emit emit_timeoutSerialError(command);
    }


}}
