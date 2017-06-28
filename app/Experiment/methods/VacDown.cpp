#include "VacDown.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

namespace App { namespace Experiment
{
    VacDown::VacDown(QObject *parent, Settings::Container settings)
        :   QObject(parent)
        ,   m_settings(settings)
        ,   t_vacMonitor(this)
        ,   t_vacTime(this)
        ,   machine(this),
            // Check the system pressure
            sm_systemPressure(&machine),
            sm_validate_requestSystemPressure(&machine),

            // Close valves that could damage the system and validate response
            sm_closeHighPressureInput(&machine),
            sm_validate_closeHighPressureInput(&machine),
            sm_closeHighPressureNitrogen(&machine),
            sm_validate_closeHighPressureNitrogen(&machine),
            sm_closeFlowController(&machine),
            sm_validate_closeFlowController(&machine),

            // Ensure turbo is disabled
            sm_disableTurbo(&machine),
            sm_validate_disableTurbo(&machine),

            // Start vacuum monitoring
            sm_startVacMonior(&machine),
            sm_validate_startVacMonior(&machine),

            // Start backing pump
            sm_startBackingPump(&machine),
            sm_validate_startBackingPump(&machine),

            // Start the turbo when vacuum low enough and has been selected
            sm_enableTurbo(&machine),
            sm_validate_enableTurbo(&machine),

            // When the timer has finished run the finished sequence
            sm_finished(&machine),

            // System error occured
            sm_error(&machine)
    {
        // The time the vacumm should vac down to
        t_vacTime.setSingleShot(true);
    }


    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void VacDown::buildMachine(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Where to start the machine
        //machine.setInitialState(&sm_initRig);

        //
        //sm_initRig.addTransition(&sslSocket, SIGNAL(encrypted()), &s2);

    }
}}
