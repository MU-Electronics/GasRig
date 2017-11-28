#include "Purge.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include valaitor state
#include "Functions/CommandValidatorState.h"

// Include possable machine states
#include "Functions/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    Purge::Purge(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)


            // Timers
    {

    }

    Purge::~Purge()
    {

    }




    /**
     * Set state machine params
     *
     * @brief Purge::setParams
     * @param outputValve
     * @param numberCycles
     * @param nitrogenPressure
     * @param vacTo
     */
    void Purge::setParams(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo)
    {       
        // Copnfiguration settings
        // params.insert("output", output);

        // Setup timers

    }


    /**
     * Start the state machine
     *
     * @brief Purge::start
     */
    void Purge::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief Purge::start
     */
    void Purge::stop()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_purgeFinished(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief Purge::stopAsFailed
     */
    void Purge::stopAsFailed()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_purgeFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief Purge::buildMachine
     */
    void Purge::buildMachine()
    {
        // Where to start the machine
        //machine.setInitialState(&sml_stageFinder);

    }

}}}






