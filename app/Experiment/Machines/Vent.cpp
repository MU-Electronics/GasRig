#include "Vent.h"

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
    Vent::Vent(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)

            // Stage finder
        ,   sml_stageFinder(&machine)

            // Wait for pressure reading
        ,   sml_waitForPressureAfterSlowExhuast(&machine)
        ,   sml_waitForPressureAfterOutput(&machine)
        ,   sml_waitForPressureAfterVacOutput(&machine)
        ,   sml_waitForPressureAfterFlowCavity(&machine)
        ,   sml_waitForPressureAfterNitrogenPipe(&machine)
        ,   sml_waitForPressureAfterMultiPipe(&machine)
        ,   sml_waitForPressureAfterFlowOnePipe(&machine)
        ,   sml_waitForPressureAfterFlowTwoPipe(&machine)

        ,   sml_validatePressureAfterSlowExhuast(&machine)
        ,   sml_validatePressureAfterOutput(&machine)
        ,   sml_validatePressureAfterVacOutput(&machine)
        ,   sml_validatePressureAfterFlowCavity(&machine)
        ,   sml_validatePressureAfterNitrogenPipe(&machine)
        ,   sml_validatePressureAfterMultiPipe(&machine)
        ,   sml_validatePressureAfterFlowOnePipe(&machine)
        ,   sml_validatePressureAfterFlowTwoPipe(&machine)


            // Close valve related states
        ,   sml_closeHighPressureInput(&machine)
        ,   sml_closeHighPressureNitrogen(&machine)
        ,   sml_closeFlowController(&machine)
        ,   sml_closeExhuast(&machine)
        ,   sml_closeOutput(&machine)
        ,   sml_closeSlowExhuastPath(&machine)
        ,   sml_closeFastExhuastPath(&machine)
        ,   sml_closeVacuumIn(&machine)
        ,   sml_closeVacuumOut(&machine)

            // Open valve related states
        ,   sml_openHighPressureInput(&machine)
        ,   sml_openHighPressureNitrogen(&machine)
        ,   sml_openFlowController(&machine)
        ,   sml_openExhuast(&machine)
        ,   sml_openOutput(&machine)
        ,   sml_openSlowExhuastPath(&machine)
        ,   sml_openFastExhuastPath(&machine)
        ,   sml_openVacuumIn(&machine)
        ,   sml_openVacuumOut(&machine)

            // Validate closed valve
        ,   sml_validateCloseHighPressureInput(&machine)
        ,   sml_validateCloseHighPressureNitrogen(&machine)
        ,   sml_validateCloseFlowController(&machine)
        ,   sml_validateCloseExhuast(&machine)
        ,   sml_validateCloseOutput(&machine)
        ,   sml_validateCloseSlowExhuastPath(&machine)
        ,   sml_validateCloseFastExhuastPath(&machine)
        ,   sml_validateCloseVacuumIn(&machine)
        ,   sml_validateCloseVacuumOut(&machine)

            // Validate open valve
        ,   sml_validateOpenHighPressureInput(&machine)
        ,   sml_validateOpenHighPressureNitrogen(&machine)
        ,   sml_validateOpenFlowController(&machine)
        ,   sml_validateOpenExhuast(&machine)
        ,   sml_validateOpenOutput(&machine)
        ,   sml_validateOpenSlowExhuastPath(&machine)
        ,   sml_validateOpenFastExhuastPath(&machine)
        ,   sml_validateOpenVacuumIn(&machine)
        ,   sml_validateOpenVacuumOut(&machine)


            // Timers

    {
        // General
        connect(&sml_stageFinder, &QState::entered, this, &Vent::stageFinder);
        connect(&sml_validatePressureAfterSlowExhuast, &QState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterOutput, &QState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterVacOutput, &QState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterFlowCavity, &QState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterNitrogenPipe, &QState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterMultiPipe, &QState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterFlowOnePipe, &QState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterFlowTwoPipe, &QState::entered, this, &Vent::validatePressureForAtmospheric);


        // Link close valve states
        connect(&sml_closeHighPressureInput, &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(&sml_closeHighPressureNitrogen, &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(&sml_closeFlowController, &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(&sml_closeExhuast, &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(&sml_closeOutput, &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(&sml_closeSlowExhuastPath, &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(&sml_closeFastExhuastPath, &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(&sml_closeVacuumIn, &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(&sml_closeVacuumOut, &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // Link close valve validator states
        connect(&sml_validateCloseHighPressureInput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(&sml_validateCloseHighPressureNitrogen, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(&sml_validateCloseFlowController, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(&sml_validateCloseExhuast, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(&sml_validateCloseOutput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(&sml_validateCloseSlowExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(&sml_validateCloseFastExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(&sml_validateCloseVacuumIn, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(&sml_validateCloseVacuumOut, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

        // Link open valve states
        connect(&sml_openHighPressureInput, &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(&sml_openHighPressureNitrogen, &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(&sml_openFlowController, &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(&sml_openExhuast, &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(&sml_openOutput, &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(&sml_openSlowExhuastPath, &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(&sml_openFastExhuastPath, &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(&sml_openVacuumIn, &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(&sml_openVacuumOut, &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);

        // Link open valve validator states
        connect(&sml_validateOpenHighPressureInput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);
        connect(&sml_validateOpenHighPressureNitrogen, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);
        connect(&sml_validateOpenFlowController, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(&sml_validateOpenExhuast, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(&sml_validateOpenOutput, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(&sml_validateOpenSlowExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(&sml_validateOpenFastExhuastPath, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(&sml_validateOpenVacuumIn, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(&sml_validateOpenVacuumOut, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);
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
        machine.setInitialState(&sml_stageFinder);


        // What the next stage should be?
        sml_stageFinder.addTransition(this, &Vent::emit_openExhuast, &sml_openExhuast);
        sml_stageFinder.addTransition(this, &Vent::emit_openSlowExhuast, &sml_openSlowExhuastPath);
        sml_stageFinder.addTransition(this, &Vent::emit_ventOuput, &sml_openOutput);

        sml_stageFinder.addTransition(this, &Vent::emit_ventVacuumOutput, &sml_openExhuast);
        sml_stageFinder.addTransition(this, &Vent::emit_ventFlowCavity, &sml_openExhuast);
        sml_stageFinder.addTransition(this, &Vent::emit_ventNitrogenPipes, &sml_openExhuast);
        sml_stageFinder.addTransition(this, &Vent::emit_ventMultiPipes, &sml_openExhuast);
        sml_stageFinder.addTransition(this, &Vent::emit_ventFlowOnePipes, &sml_openExhuast);
        sml_stageFinder.addTransition(this, &Vent::emit_ventFlowTwoPipes, &sml_openExhuast);

        sml_stageFinder.addTransition(this, &Vent::emit_finished, &sm_stop);


        // Open the exhuast valve
        sml_openExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenExhuast);
            // Success finish here
            sml_validateOpenExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_stageFinder);
            // Failed close all valves
            sml_validateOpenExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


        // Open the slow exhuast valve
        sml_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenSlowExhuastPath);
            // Failed close all valves
            sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_waitForPressureAfterSlowExhuast);
                // Wait for pressure reading?
                sml_waitForPressureAfterSlowExhuast.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterSlowExhuast);
                    // Are we at atmopheric
                    sml_validatePressureAfterSlowExhuast.addTransition(this, &Vent::emit_validationSuccess, sml_stageFinder);
                    sml_validatePressureAfterSlowExhuast.addTransition(this, &Vent::emit_validationFailed, sml_waitForPressureAfterSlowExhuast);


        // Open the output valve
        sml_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenOutput);
            // Failed close all valves
            sml_validateOpenOutput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            sml_validateOpenOutput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_waitForPressureAfterOutput);
                // Wait for pressure reading?
                sml_waitForPressureAfterOutput.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterOutput);
                    // Are we at atmopheric
                    sml_validatePressureAfterOutput.addTransition(this, &Vent::emit_validationSuccess, sml_stageFinder);
                    sml_validatePressureAfterOutput.addTransition(this, &Vent::emit_validationFailed, sml_waitForPressureAfterOutput);
    }




    void Vent::stageFinder()
    {
        // Stage is 0 then we need to open exhuast
        if(stage == 0)
        {
            stage = 1;
            emit emit_openExhuast();
            return;
        }

        // Stage is 1 then we need to vent internal rig
        if(stage == 1)
        {
            stage = 2;
            emit emit_openSlowExhuast();
            return;
        }

        // Stage is 2 then we need to vent the output
        if(stage == 2)
        {
            if(params.value("output").Bool == true)
            {
                emit emit_ventOuput();
                return;
            }
            stage = 3;
        }

        // Finished stage << Temp while test
        if(stage == 3)
        {
            stage = 0;
            emit emit_finished();
            return;
        }

    }


    /**
     * Validate whether the current pressure is atmospheric
     *
     * @brief Vent::validatePressureForAtmospheric
     */
    void Vent::validatePressureForAtmospheric()
    {
        // Get the validator state instance
        Functions::CommandValidatorState* state = (Functions::CommandValidatorState*)sender();

        // Get the package data from the instance
        QVariantMap package = state->package;

        // Get the pressure
        float pressureIn = package.value("pressure").toFloat();

        // Get the max pressure allowed
        float maxPressure = 1250;

        // Check the pressure is safe to vac down
        if(pressureIn < maxPressure)
        {
            // Store the stage info
            QVariantMap data;
            data.insert("pressure", pressureIn);

            // Emit safe to proceed
            emit emit_validationSuccess(data);

            return;
        }

        // Store the error
        QVariantMap error;
        error.insert("message", "pressure in system is too high for the vac station; Exhuast the system first.");
        error.insert("system_pressure", pressureIn);
        error.insert("system_pressure_max", maxPressure);

        // Emit not safe to proceed
        emit emit_validationFailed(error);
    }

}}}






