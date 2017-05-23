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
            m_vacStation(*new HAL::VacStation(this, 123)),
            m_flowController(*new HAL::FlowController(this)),
            m_pressureSensor(*new HAL::PressureSensor(this)),
            m_labjack(*new HAL::LabJack(this))
    {

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


        // Open com port for the flow controllers


        // Open the com port for the labjack
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
        connectDevices();

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
        while (i.hasNext())
        {
            i.next();
            status[i.key()] = i.value();
        }

        // Debug message
        // qDebug() << "Harware set to: " << hardware << " Method to run: " << method;

        // Find the correct HAL
        if(hardware == "VacStation")
        {
            // If the bus is not free we cant procceed
            if(!m_vacStation.busFree() || !m_vacStation.isOpen())
            {
                // Re add the method to the queue as this one will be removed
                m_queue.enqueue(command);

                // Return back to worker for next method
                return;
            }

            // Set the method params
            m_vacStation.setParams(command);

            // Run the method and cache the status
            status["resulting_status"] = (QMetaObject::invokeMethod(&m_vacStation, method.toLatin1().data(), Qt::DirectConnection)) ? true : false;

        }
        else if(hardware == "PressureSensor")
        {

        }
        else if (hardware  == "FlowController")
        {

        }
        else if (hardware == "LabJack")
        {

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

}}
