#include "Access.h"

// Include external libs
#include <QObject>
#include <QThread>
#include <QDebug>

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
            m_settings(settings),
            m_timer(*new QTimer(this)),
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
        qDebug() << "Hardware thread child setup method ran";

        // Open the com port for the vac station
        m_vacStation.open(m_vacStation.findPortName(24577, 1027), 9600, 1000); // Will open on tty.usbserial-AH02FNCX"

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
        // Get temperature as a one off
        if(!firstRun && m_vacStation.busFree())
        {
            // Mark as ran
            firstRun = true;

            // Turn the pump on and off
            m_timer.singleShot(500, [=] {
                // Turn pump on
                m_vacStation.SetPumpingState(1);
            });
            m_timer.singleShot(5000, [=] {
                // Turn pump off
                m_vacStation.SetPumpingState(0);
            });
        }

        // Sleep the thread for a while
        thread()->usleep(5);
    }

}}
