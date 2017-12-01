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


            // Control flow controller override valves
        ,   sml_flowControllerOneValveOverrideOpen(&machine)
        ,   sml_flowControllerTwoValveOverrideOpen(&machine)
        ,   sml_flowControllerOneValveOverrideOff(&machine)
        ,   sml_flowControllerTwoValveOverrideOff(&machine)
        ,   sml_flowControllerOneValveOverrideClose(&machine)
        ,   sml_flowControllerTwoValveOverrideClose(&machine)
        ,   sml_validateFlowControllerOneValveOverrideOpen(&machine)
        ,   sml_validateFlowControllerTwoValveOverrideOpen(&machine)
        ,   sml_validateFlowControllerOneValveOverrideClose(&machine)
        ,   sml_validateFlowControllerTwoValveOverrideClose(&machine)
        ,   sml_validateFlowControllerOneValveOverrideOff(&machine)
        ,   sml_validateFlowControllerTwoValveOverrideOff(&machine)


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
        ,   sml_closeFlowController_2(&machine)
        ,   sml_closeFlowController_3(&machine)
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
        ,   sml_openFlowController_2(&machine)
        ,   sml_openFlowController_3(&machine)

            // Validate closed valve
        ,   sml_validateCloseHighPressureInput(&machine)
        ,   sml_validateCloseHighPressureNitrogen(&machine)
        ,   sml_validateCloseFlowController(&machine)
        ,   sml_validateCloseFlowController_2(&machine)
        ,   sml_validateCloseFlowController_3(&machine)
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
        ,   sml_validateOpenFlowController_2(&machine)
        ,   sml_validateOpenFlowController_3(&machine)
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

        // Validate pressure for atmospheric
        connect(&sml_validatePressureAfterSlowExhuast, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterOutput, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterVacOutput, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterFlowCavity, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterNitrogenPipe, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterMultiPipe, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterFlowOnePipe, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(&sml_validatePressureAfterFlowTwoPipe, &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);


        // Control flow cotnrollers override valve
        connect(&sml_flowControllerOneValveOverrideOpen, &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideOpen);
        connect(&sml_flowControllerTwoValveOverrideOpen, &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideOpen);
        connect(&sml_flowControllerOneValveOverrideOff, &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideOff);
        connect(&sml_flowControllerTwoValveOverrideOff, &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideOff);
        connect(&sml_flowControllerOneValveOverrideClose, &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideClose);
        connect(&sml_flowControllerTwoValveOverrideClose, &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideClose);
        connect(&sml_validateFlowControllerOneValveOverrideOpen, &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideOpen);
        connect(&sml_validateFlowControllerTwoValveOverrideOpen, &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideOpen);
        connect(&sml_validateFlowControllerOneValveOverrideClose, &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideClose);
        connect(&sml_validateFlowControllerTwoValveOverrideClose, &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideClose);
        connect(&sml_validateFlowControllerOneValveOverrideOff, &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideOff);
        connect(&sml_validateFlowControllerTwoValveOverrideOff, &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideOff);


        // Link close valve states
        connect(&sml_closeHighPressureInput, &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(&sml_closeHighPressureNitrogen, &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(&sml_closeFlowController, &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(&sml_closeFlowController_2, &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(&sml_closeFlowController_3, &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
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
        connect(&sml_validateCloseFlowController_2, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(&sml_validateCloseFlowController_3, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
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
        connect(&sml_openFlowController_2, &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(&sml_openFlowController_3, &QState::entered, this->valves(), &Functions::Valves::openFlowController);
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
        connect(&sml_validateOpenFlowController_2, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(&sml_validateOpenFlowController_3, &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
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
     * @brief Vent::stopped
     */
    void Vent::stopped()
    {
        // Close valves
        valves()->closeSlowExhuastPath();
        valves()->closeExhuast();
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
        sml_stageFinder.addTransition(this, &Vent::emit_ventVacuumOutput, &sml_openVacuumOut);
        sml_stageFinder.addTransition(this, &Vent::emit_ventFlowCavity, &sml_openFlowController);
        sml_stageFinder.addTransition(this, &Vent::emit_ventNitrogenPipes, &sml_openHighPressureNitrogen);
        sml_stageFinder.addTransition(this, &Vent::emit_ventMultiPipes, &sml_openHighPressureInput);
        sml_stageFinder.addTransition(this, &Vent::emit_ventFlowOnePipes, &sml_flowControllerOneValveOverrideOpen);
        sml_stageFinder.addTransition(this, &Vent::emit_ventFlowTwoPipes, &sml_flowControllerTwoValveOverrideOpen);
        sml_stageFinder.addTransition(this, &Vent::emit_finished, &sm_stop);


        // Open exhuast wait for pressure drop
        sml_openExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenExhuast);
            // Failed close all valves
            sml_validateOpenExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            sml_validateOpenExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openSlowExhuastPath);

        // Open the slow exhuast valve and wait for pressure drop
        sml_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenSlowExhuastPath);
            // Failed close all valves
            sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_waitForPressureAfterSlowExhuast);
                // Wait for pressure reading?
                sml_waitForPressureAfterSlowExhuast.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &sml_validatePressureAfterSlowExhuast);
                    // Are we at atmopheric
                    sml_validatePressureAfterSlowExhuast.addTransition(this, &Vent::emit_validationSuccess, &sml_stageFinder);
                    sml_validatePressureAfterSlowExhuast.addTransition(this, &Vent::emit_validationFailed, &sml_waitForPressureAfterSlowExhuast);


        // Open the output valve, wait for pressure drop and then close
        openPressureClose(sml_openOutput, sml_validateOpenOutput, sml_closeOutput, sml_validateCloseOutput, sml_waitForPressureAfterOutput, sml_validatePressureAfterOutput, sml_stageFinder, sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(sml_openVacuumOut, sml_validateOpenVacuumOut, sml_closeVacuumOut, sml_validateCloseVacuumOut, sml_waitForPressureAfterVacOutput, sml_validatePressureAfterVacOutput, sml_stageFinder, sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(sml_openFlowController, sml_validateOpenFlowController, sml_closeFlowController, sml_validateCloseFlowController, sml_waitForPressureAfterFlowCavity, sml_validatePressureAfterFlowCavity, sml_stageFinder, sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(sml_openHighPressureNitrogen, sml_validateOpenHighPressureNitrogen, sml_closeHighPressureNitrogen, sml_validateCloseHighPressureNitrogen, sml_waitForPressureAfterNitrogenPipe, sml_validatePressureAfterNitrogenPipe, sml_stageFinder, sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(sml_openHighPressureInput, sml_validateOpenHighPressureInput, sml_closeHighPressureInput, sml_validateCloseHighPressureInput, sml_waitForPressureAfterMultiPipe, sml_validatePressureAfterMultiPipe, sml_stageFinder, sm_stopAsFailed);

        // Open the flow controller internal valve
        sml_flowControllerOneValveOverrideOpen.addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, &sml_validateFlowControllerOneValveOverrideOpen);
            // Check valve open
            sml_validateFlowControllerOneValveOverrideOpen.addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
            sml_validateFlowControllerOneValveOverrideOpen.addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, &sml_openFlowController_2);
                // Open flow controller cavity valve, wait for pressure drop, open intenral flow controller valve, and then close
                openPressureClose(sml_openFlowController_2, sml_validateOpenFlowController_2, sml_closeFlowController_2, sml_validateCloseFlowController_2, sml_waitForPressureAfterFlowOnePipe, sml_validatePressureAfterFlowOnePipe, sml_flowControllerOneValveOverrideOff, sm_stopAsFailed);
                    // Turn off valve override
                    sml_flowControllerOneValveOverrideOff.addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, &sml_validateFlowControllerOneValveOverrideOff);
                        // Check valve override off
                        sml_validateFlowControllerOneValveOverrideOff.addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
                        sml_validateFlowControllerOneValveOverrideOff.addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, &sml_stageFinder);


        // Open the flow controller internal valve
        sml_flowControllerTwoValveOverrideOpen.addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, &sml_validateFlowControllerTwoValveOverrideOpen);
            // Check valve open
            sml_validateFlowControllerTwoValveOverrideOpen.addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
            sml_validateFlowControllerTwoValveOverrideOpen.addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, &sml_openFlowController_3);
                // Open the  valve, wait for pressure drop, open intenral flow controller valve, and then close
                openPressureClose(sml_openFlowController_3, sml_validateOpenFlowController_3, sml_closeFlowController_3, sml_validateCloseFlowController_3, sml_waitForPressureAfterFlowTwoPipe, sml_validatePressureAfterFlowTwoPipe, sml_flowControllerTwoValveOverrideOff, sm_stopAsFailed);
                    // Turn off valve override
                    sml_flowControllerTwoValveOverrideOff.addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, &sml_validateFlowControllerTwoValveOverrideOff);
                        // Check valve override off
                        sml_validateFlowControllerTwoValveOverrideOff.addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
                        sml_validateFlowControllerTwoValveOverrideOff.addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, &sml_stageFinder);
    }



    /**
     * Select which stage we are up to
     *
     * @brief Vent::stageFinder
     */
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
            stage = 3;
            if(params.value("output").toBool() == true)
            {
                emit emit_ventOuput();
                return;
            }

        }

        // Stage is 3 then we need to vent vacuum out
        if(stage == 3)
        {
            stage = 4;
            if(params.value("vacuum_output").toBool() == true)
            {
                emit emit_ventVacuumOutput();
                return;
            }

        }

        // Stage is 4 then we need to vent flow cavity
        if(stage == 4)
        {
            stage = 5;
            if(params.value("flow_cavity").toBool() == true)
            {
                emit emit_ventFlowCavity();
                return;
            }

        }

        // Stage is 5 then we need to vent nitrogen pipe
        if(stage == 5)
        {
            stage = 6;
            if(params.value("nitrogen_pipes").toBool() == true)
            {
                emit emit_ventNitrogenPipes();
                return;
            }

        }

        // Stage is 6 then we need to vent multi gas pipe
        if(stage == 6)
        {
            stage = 7;
            if(params.value("multi_pipes").toBool() == true)
            {
                emit emit_ventMultiPipes();
                return;
            }

        }

        // Stage is 7 then we need to vent flow controller one pipe
        if(stage == 7)
        {
            stage = 8;
            if(params.value("flow_one_pipes").toBool() == true)
            {
                emit emit_ventFlowOnePipes();
                return;
            }

        }

        // Stage is 8 then we need to vent flow controller two pipe
        if(stage == 8)
        {
            stage = 9;
            if(params.value("flow_two_pipes").toBool() == true)
            {
                emit emit_ventFlowTwoPipes();
                return;
            }

        }

        // Finished stage << Temp while test
        if(stage == 9)
        {
            stage = 0;
            emit emit_finished();
            return;
        }

    }

    /**
     * Builder for open valve, wait for pressure to decrease, close valve
     *
     * @brief Vent::openPressureClose
     * @param open
     * @param openValidate
     * @param close
     * @param closeValidate
     * @param pressureWait
     * @param pressureValidate
     */
    void Vent::openPressureClose(QState& open,
                                 Functions::CommandValidatorState& openValidate,
                                 QState& close,
                                 Functions::CommandValidatorState& closeValidate,
                                 QState& pressureWait,
                                 Functions::CommandValidatorState& pressureValidate,
                                 QState& finished,
                                 QState& failed)
    {
        // Open the output valve
        open.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &openValidate);
            // Failed close all valves
            openValidate.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &failed);
            // Success finish here
            openValidate.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &pressureWait);
                // Wait for pressure reading?
                pressureWait.addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, &pressureValidate);
                    // Are we at atmopheric
                    pressureValidate.addTransition(this, &Vent::emit_validationFailed, &pressureWait);
                    pressureValidate.addTransition(this, &Vent::emit_validationSuccess, &close);
                        // Close valve
                        close.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &closeValidate);
                            // Validate close
                            closeValidate.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &failed);
                            closeValidate.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &finished);
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
        double pressureIn = package.value("pressure").toDouble() * 1000;

        // Get the max pressure allowed
        double maxPressure = 1100;

        // Get the min pressure allowed
        double minPressure = 950;

        // Check the pressure is safe to vac down
        if(pressureIn < maxPressure && pressureIn > minPressure)
        {
            // Store the stage info
            QVariantMap data;
            data.insert("pressure", pressureIn);

            // Emit safe to proceed
            emit emit_validationSuccess();

            return;
        }

        // Store the error
        QVariantMap error;
        error.insert("message", "pressure in system is too high for the vac station; Exhuast the system first.");
        error.insert("system_pressure", pressureIn);
        error.insert("system_pressure_max", maxPressure);

        // Emit not safe to proceed
        emit emit_validationFailed();
    }

}}}






