#include "Machines.h"

// Include external libs
#include <QDebug>

// Include state machine
#include "VacDown.h"

namespace App { namespace Experiment { namespace Machines
{


    Machines::Machines(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   m_vacDown(*new VacDown(parent, settings, hardware, safety))
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
    }

    /**
     * Stops a running instance of vac down state machine
     *
     * @brief Machines::StopVacDown
     */
    void Machines::stopVacDown()
    {
        m_vacDown.stop();
    }


}}}
