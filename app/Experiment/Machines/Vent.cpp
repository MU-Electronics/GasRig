#include "Vent.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "Functions/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    Vent::Vent(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // States


            // Timers

    {

    }

    Vent::~Vent()
    {

    }




    /**
     * Configure the state machine params
     *
     * @brief Vent::setParams
     * @param output
     * @param vacuumOutput
     * @param flowCavity
     * @param nitrogenPipes
     * @param multiPipes
     * @param flowOnePipes
     * @param flowTwoPipes
     */
    void Vent::setParams(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes)
    {       
        // Copnfiguration settings
        params.insert("output", output);
        params.insert("vacuum_output", vacuumOutput);
        params.insert("flow_cavity", flowCavity);
        params.insert("nitrogen_pipes", nitrogenPipes);
        params.insert("multi_pipes", multiPipes);
        params.insert("flow_one_pipes", flowOnePipes);
        params.insert("flow_two_pipes", flowTwoPipes);

        // Setup timers

    }


    /**
     * Start the state machine
     *
     * @brief Vent::start
     */
    void Vent::start()
    {
        machine.start();
        qDebug() << "started";

    }


    /**
     * Start the state machine
     *
     * @brief Vent::start
     */
    void Vent::stop()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_ventFinished(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief Vent::stopAsFailed
     */
    void Vent::stopAsFailed()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_ventFailed(params);
    }


    /**
     * Builds the machine connections
     *
     * @brief Vent::buildMachine
     */
    void Vent::buildMachine()
    {
        // Where to start the machine
        //machine.setInitialState(&sml_startTurboSpeedTimer);

    }

}}}






