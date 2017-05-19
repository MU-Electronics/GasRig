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
            m_vacStation(*new HAL::VacStation(this, 1)),
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
    }


    /**
     * The thread worker that will be in a forever loop untill stopped
     * via the thread_abort parent member variable
     *
     * @brief Monitor::worker
     */
    void Access::worker()
    {
        //qDebug() << "Hardware thread";

        //qDebug() << m_vacStation.findPortName(24577, 1027);




        m_vacStation.open("tty.usbserial-AH02FNCX", 9600, 1000);

        m_vacStation.GetTemperature(1);

        m_vacStation.close();


        thread()->sleep(20);
    }

}}
