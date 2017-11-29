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
#include "Vent.h"
#include "VacDown.h"

namespace App { namespace Experiment { namespace Machines
{
    Purge::Purge(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Pressurise state machine
        ,   m_vent(*new Vent(parent, settings, hardware, safety))
        ,   m_pressurise(*new Pressurise(parent, settings, hardware, safety))
        ,   m_vacDown(*new VacDown(parent, settings, hardware, safety))

            // States
        ,   sml_vent(&machine)
        ,   sml_finishingVent(&machine)
        ,   sml_setLowPressure(&machine)
        ,   sml_setHighPressure(&machine)
        ,   sml_setAtmospheric(&machine)
        ,   sml_checkCycles(&machine)
        ,   sml_vacDown(&machine)
    {
        // Vent state machine
        connect(&sml_vent, &QState::entered, this, &Purge::ventOutput);
        connect(&sml_finishingVent, &QState::entered, this, &Purge::ventOutput);

        // Vac down state machine
        connect(&sml_vacDown, &QState::entered, this, &Purge::setVacuum);

        // Set pressure state machine
        connect(&sml_setHighPressure, &QState::entered, this, &Purge::setHighPressure);

        // Check how many cycles left state
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
     * @brief Purge::stopped
     */
    void Purge::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief Purge::buildMachine
     */
    void Purge::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(&sml_vent);

        // Vent
        sml_vent.addTransition(&m_vent, &Vent::emit_machineFailed, &sm_stopAsFailed);
        sml_vent.addTransition(&m_vent, &Vent::emit_machineFinished, &sml_vacDown);



        // Vac down to X
        sml_vacDown.addTransition(&m_vacDown, &VacDown::emit_machineFailed, &sm_stopAsFailed);
        sml_vacDown.addTransition(&m_vacDown, &VacDown::emit_machineFinished, &sml_setHighPressure);



        // Set high pressure
        sml_setHighPressure.addTransition(&m_pressurise, &Pressurise::emit_machineFailed, &sm_stopAsFailed);
        sml_setHighPressure.addTransition(&m_pressurise, &Pressurise::emit_machineFinished, &sml_checkCycles);



        // Check cycles
        sml_checkCycles.addTransition(this, &Purge::emit_continueCycling, &sml_vent);
        sml_checkCycles.addTransition(this, &Purge::emit_stopCycling, &sml_finishingVent);

            // Vent pressure
            sml_finishingVent.addTransition(&m_vent, &Vent::emit_machineFailed, &sm_stopAsFailed);
            sml_finishingVent.addTransition(&m_vent, &Vent::emit_machineFinished, &sm_stop);

    }

    void Purge::checkCycles()
    {
        qDebug() << "Checking cycles";


        // Continue or stop?
        if(cycles == 0)
        {
            qDebug() << "Stop cycling";
            emit emit_stopCycling();
        }
        else
        {
            // Once cycle just completed
            cycles--;

            qDebug() << "Continue cycling: " << cycles;

            emit emit_continueCycling();
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
        m_pressurise.setParams(params.value("nitrogen_pressure").toDouble(), true, 2000, false, params.value("open_output_valve").toBool());

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
        override.insert("tolerance_final", 500);
        override.insert("vac_down_to", params.value("vac_pressure").toDouble());
        m_pressurise.paramsOverride(override);

        // Build the machine
        m_pressurise.buildMachine();

        // Start the machine
        m_pressurise.start();
        qDebug() << "High presure" << params.value("nitrogen_pressure").toDouble();
    }


    /**
     * Vents pressure
     *
     * @brief Purge::ventOutput
     */
    void Purge::ventOutput()
    {
        // Set the params
        m_vent.setParams(true, false, false, false, false, false, false);

        // Build the machine
        m_vent.buildMachine();

        // Start the machine
        m_vent.start();
    }


    /**
     * Check if the system has exhusted low enough to be vaced down
     *
     * @brief Purge::setVacuum
     */
    void Purge::setVacuum()
    {
        // Set the params
        m_vacDown.setParams(params.value("vac_pressure").toDouble(), false, false, 2, 1);

        // Build the machine
        m_vacDown.buildMachine();

        // Start the machine
        m_vacDown.start();
    }

}}}





