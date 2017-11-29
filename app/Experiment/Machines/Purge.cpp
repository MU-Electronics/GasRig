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

// Required state machines
#include "Pressurise.h"

namespace App { namespace Experiment { namespace Machines
{
    Purge::Purge(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Pressurise state machine
        ,   m_pressurise(*new Pressurise(parent, settings, hardware, safety))

            // States
        ,   sml_setLowPressure(&machine)
        ,   sml_setHighPressure(&machine)
        ,   sml_setAtmospheric(&machine)
        ,   sml_checkCycles(&machine)
    {
        connect(&sml_setLowPressure, &QState::entered, this, &Purge::setLowPressure);
        connect(&sml_setHighPressure, &QState::entered, this, &Purge::setHighPressure);
        connect(&sml_setAtmospheric, &QState::entered, this, &Purge::setAtmospheric);
        connect(&sml_checkCycles, &QState::entered, this, &Purge::checkCycles);
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
        params.insert("open_output_valve", outputValve);
        params.insert("number_cycles", numberCycles);
        params.insert("nitrogen_pressure", nitrogenPressure);
        params.insert("vac_pressure", vacTo);

        // Local count
        cycles = numberCycles;
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
        machine.setInitialState(&sml_setLowPressure);



        // Set low pressure
        sml_setLowPressure.addTransition(&m_pressurise, &Pressurise::emit_pressuriseFailed, &sm_stopAsFailed);
        sml_setLowPressure.addTransition(&m_pressurise, &Pressurise::emit_pressuriseFinished, &sml_setHighPressure);



        // Set high pressure
        sml_setHighPressure.addTransition(&m_pressurise, &Pressurise::emit_pressuriseFailed, &sm_stopAsFailed);
        sml_setHighPressure.addTransition(&m_pressurise, &Pressurise::emit_pressuriseFinished, &sml_checkCycles);



        // Check cycles
        sml_checkCycles.addTransition(this, &Purge::emit_continueCycling, &sml_setHighPressure);
        sml_checkCycles.addTransition(this, &Purge::emit_stopCycling, &sml_setAtmospheric);

            // Set atmopheric peressure
            sml_setAtmospheric.addTransition(&m_pressurise, &Pressurise::emit_pressuriseFailed, &sm_stopAsFailed);
            sml_setAtmospheric.addTransition(&m_pressurise, &Pressurise::emit_pressuriseFinished, &sm_stop);

    }

    void Purge::checkCycles()
    {
        // Once cycle just completed
        cycles--;

        // Continue or stop?
        if(cycles == 0)
        {
            emit emit_stopCycling();
        }
        else
        {
            emit emit_stopCycling();
        }
    }


    /**
     * Set the purge high pressure
     *
     * @brief Purge::setHighPressure
     */
    void Purge::setHighPressure()
    {
        // Set params
        m_pressurise.setParams(params.value("nitrogen_pressure").toBool(), true, 2000, false, params.value("open_output_valve").toBool());

        // Override deep params within the pressurise state machine setup
        QVariantMap override;
        override.insert("valve_2_pulse", 200);
        override.insert("tolerance_final", 500);
        override.insert("tolerance_valve_two", 500);
        override.insert("tolerance_valve_two_step", 500);
        override.insert("tolerance_valve_two_final", 500);
        override.insert("tolerance_valve_seven", 500);
        override.insert("tolerance_valve_seven_step", 500);
        override.insert("tolerance_valve_seven_final", 500);
        override.insert("vac_down_to", params.value("vac_pressure").toBool());
        m_pressurise.paramsOverride(override);

        // Build the machine
        m_pressurise.buildMachine();
        qDebug() << 2;
        // Start the machine
        m_pressurise.start();
        qDebug() << 3;
    }


    /**
     * Set the purge low pressure
     *
     * @brief Purge::setLowPressure
     */
    void Purge::setLowPressure()
    {
        bool diableInitVacDown = true;
        if(cycles == params.value("number_cycles").toInt())
            diableInitVacDown = false;

        // Set params
        m_pressurise.setParams(params.value("vac_pressure").toBool(), diableInitVacDown, 2000, false, params.value("open_output_valve").toBool());

        // Override deep params within the pressurise state machine setup
        QVariantMap override;
        override.insert("valve_2_pulse", 200);
        override.insert("tolerance_final", 500);
        override.insert("tolerance_valve_two", 500);
        override.insert("tolerance_valve_two_step", 500);
        override.insert("tolerance_valve_two_final", 500);
        override.insert("tolerance_valve_seven", 500);
        override.insert("tolerance_valve_seven_step", 500);
        override.insert("tolerance_valve_seven_final", 500);
        override.insert("vac_down_to", params.value("vac_pressure").toBool());
        m_pressurise.paramsOverride(override);

        // Build the machine
        m_pressurise.buildMachine();

        // Start the machine
        m_pressurise.start();
    }


    /**
     * Check if the system has exhusted low enough to be vaced down
     *
     * @brief Purge::setAtmospheric
     */
    void Purge::setAtmospheric()
    {
        // Set params
        m_pressurise.setParams(1000, true, 2000, false, params.value("open_output_valve").toBool());

        // Override deep params within the pressurise state machine setup
        QVariantMap override;
        override.insert("valve_2_pulse", 200);
        m_pressurise.paramsOverride(override);

        // Build the machine
        m_pressurise.buildMachine();

        // Start the machine
        m_pressurise.start();
    }

}}}






