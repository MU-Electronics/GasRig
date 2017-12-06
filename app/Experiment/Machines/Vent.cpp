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
    {
        // General
        connect(state("sml_stageFinder", true), &QState::entered, this, &Vent::stageFinder);

        // Validate pressure for atmospheric
        connect(validator("pressureAfterSlowExhuast", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("pressureAfterOutput", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("pressureAfterVacOutput", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("pressureAfterFlowCavity", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("pressureAfterNitrogenPipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("pressureAfterMultiPipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("pressureAfterFlowOnePipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("pressureAfterFlowTwoPipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);


        // Control flow cotnrollers override valve
        connect(state("flowControllerOneValveOverrideOpen", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideOpen);
        connect(state("flowControllerTwoValveOverrideOpen", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideOpen);
        connect(state("flowControllerOneValveOverrideOff", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideOff);
        connect(state("flowControllerTwoValveOverrideOff", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideOff);
        connect(state("flowControllerOneValveOverrideClose", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideClose);
        connect(state("flowControllerTwoValveOverrideClose", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideClose);
        connect(validator("flowControllerOneValveOverrideOpen", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideOpen);
        connect(validator("flowControllerTwoValveOverrideOpen", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideOpen);
        connect(validator("flowControllerOneValveOverrideClose", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideClose);
        connect(validator("flowControllerTwoValveOverrideClose", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideClose);
        connect(validator("flowControllerOneValveOverrideOff", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideOff);
        connect(validator("flowControllerTwoValveOverrideOff", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideOff);



        // Fast exhuast
        connect(state("openFastExhuastPathWaitingInternal", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openFastExhuastPathWaitingOutput", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openFastExhuastPathWaitingVacCavity", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openFastExhuastPathWaitingFlowCavity", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openFastExhuastPathWaitingNitrogenPipe", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openFastExhuastPathWaitingMultiFlow", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openFastExhuastPathWaitingFlowOnePipe", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openFastExhuastPathWaitingFlowTwoPipe", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);

        connect(state("closeFastExhuastPathWaitingInternal", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeFastExhuastPathWaitingOutput", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeFastExhuastPathWaitingVacCavity", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeFastExhuastPathWaitingFlowCavity", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeFastExhuastPathWaitingNitrogenPipe", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeFastExhuastPathWaitingMultiFlow", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeFastExhuastPathWaitingFlowOnePipe", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeFastExhuastPathWaitingFlowTwoPipe", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);





        // Link close valve states
        connect(state("closeHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("closeHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("closeFlowController", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("closeFlowController_2", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("closeFlowController_3", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("closeExhuast", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("closeOutput", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("closeSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("closeFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("closeVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("closeVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // Link close valve validator states
        connect(validator("closeHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("closeHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("closeFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("closeFlowController_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("closeFlowController_3", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("closeExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("closeOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("closeSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("closeFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("closeVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("closeVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

        // Link open valve states
        connect(state("openHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(state("openHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(state("openFlowController", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("openFlowController_2", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("openFlowController_3", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("openExhuast", true), &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(state("openOutput", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(state("openSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(state("openFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("openVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(state("openVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);

        // Link open valve validator states
        connect(validator("openHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);
        connect(validator("openHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);
        connect(validator("openFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("openFlowController_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("openFlowController_3", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("openExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(validator("openOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("openSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(validator("openFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(validator("openVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("openVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);
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
        // reset vars
        stage = 0;

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
        valves()->closeOutput();
        valves()->closeVacuumOut();
        valves()->closeFastExhuastPath();
        valves()->closeVacuumIn();
        valves()->closeFlowController();
        valves()->closeHighPressureInput();
        valves()->closeHighPressureNitrogen();
    }


    /**
     * Builds the machine connections
     *
     * @brief Vent::buildMachine
     */
    void Vent::buildMachine()
    {
        // Where to start the machine
        machine.setInitialState(state("sml_stageFinder", true));


        // What the next stage should be?
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_openExhuast, state("openExhuast", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_openSlowExhuast, state("openSlowExhuastPath", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventOuput, state("openOutput", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventVacuumOutput, state("openVacuumOut", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventFlowCavity, state("openFlowController", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventNitrogenPipes, state("openHighPressureNitrogen", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventMultiPipes, state("openHighPressureInput", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventFlowOnePipes, state("flowControllerOneValveOverrideOpen", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventFlowTwoPipes, state("flowControllerTwoValveOverrideOpen", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_finished, &sm_stop);


        // Open exhuast wait for pressure drop
        state("openExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("openExhuast", true));
            // Failed close all valves
            validator("openExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            validator("openExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openSlowExhuastPath", true));

        // Open the slow exhuast valve and wait for pressure drop
        state("openSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("openSlowExhuastPath", true));
            // Failed close all valves
            validator("openSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            validator("openSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_waitForPressureAfterSlowExhuast", true));
                // Wait for pressure reading?
                state("sml_waitForPressureAfterSlowExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("pressureAfterSlowExhuast", true));
                    // Are we at atmopheric
                    validator("pressureAfterSlowExhuast", true)->addTransition(this, &Vent::emit_validationSuccess, state("closeFastExhuastPathWaitingInternal", true));
                        // Close fast exhuast
                        state("closeFastExhuastPathWaitingInternal", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, state("sml_stageFinder", true));
                            validator("pressureAfterSlowExhuast", true)->addTransition(this, &Vent::emit_validationFailed, state("sml_waitForPressureAfterSlowExhuast", true));
                            validator("pressureAfterSlowExhuast", true)->addTransition(this, &Vent::emit_openValveFour, state("openFastExhuastPathWaitingInternal", true));
                        // Open fast exhuast
                        state("openFastExhuastPathWaitingInternal", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, state("sml_waitForPressureAfterSlowExhuast", true));


        // Open the output valve, wait for pressure drop and then close
        openPressureClose(state("openOutput", true),
                          validator("openOutput", true),
                          state("closeOutput", true),
                          validator("closeOutput", true),
                          state("openFastExhuastPathWaitingOutput", true),
                          state("closeFastExhuastPathWaitingOutput", true),
                          state("waitForPressureAfterOutput", true),
                          validator("pressureAfterOutput", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("openVacuumOut", true),
                          validator("openVacuumOut", true),
                          state("closeVacuumOut", true),
                          validator("closeVacuumOut", true),
                          state("openFastExhuastPathWaitingVacCavity", true),
                          state("closeFastExhuastPathWaitingVacCavity", true),
                          state("waitForPressureAfterVacOutput", true),
                          validator("pressureAfterVacOutput", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("openFlowController", true),
                          validator("openFlowController", true),
                          state("closeFlowController", true),
                          validator("closeFlowController", true),
                          state("openFastExhuastPathWaitingFlowCavity", true),
                          state("closeFastExhuastPathWaitingFlowCavity", true),
                          state("waitForPressureAfterFlowCavity", true),
                          validator("pressureAfterFlowCavity", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("openHighPressureNitrogen", true),
                          validator("openHighPressureNitrogen", true),
                          state("closeHighPressureNitrogen", true),
                          validator("closeHighPressureNitrogen", true),
                          state("openFastExhuastPathWaitingNitrogenPipe", true),
                          state("closeFastExhuastPathWaitingNitrogenPipe", true),
                          state("waitForPressureAfterNitrogenPipe", true),
                          validator("pressureAfterNitrogenPipe", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("openHighPressureInput", true),
                          validator("openHighPressureInput", true),
                          state("closeHighPressureInput", true),
                          validator("closeHighPressureInput", true),
                          state("openFastExhuastPathWaitingMultiFlow", true),
                          state("closeFastExhuastPathWaitingMultiFlow", true),
                          state("waitForPressureAfterMultiPipe", true),
                          validator("pressureAfterMultiPipe", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the flow controller internal valve
        state("flowControllerOneValveOverrideOpen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("flowControllerOneValveOverrideOpen", true));
            // Check valve open
            validator("flowControllerOneValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
            validator("flowControllerOneValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("openFlowController_2", true));
                // Open flow controller cavity valve, wait for pressure drop, open intenral flow controller valve, and then close
                openPressureClose(state("openFlowController_2", true),
                                  validator("openFlowController_2", true),
                                  state("closeFlowController_2", true),
                                  validator("closeFlowController_2", true),
                                  state("openFastExhuastPathWaitingFlowOnePipe", true),
                                  state("closeFastExhuastPathWaitingFlowOnePipe", true),
                                  state("waitForPressureAfterFlowOnePipe", true),
                                  validator("pressureAfterFlowOnePipe", true),
                                  state("flowControllerOneValveOverrideOff", true),
                                  &sm_stopAsFailed);
                    // Turn off valve override
                    state("flowControllerOneValveOverrideOff", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("flowControllerOneValveOverrideOff", true));
                        // Check valve override off
                        validator("flowControllerOneValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
                        validator("flowControllerOneValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("sml_stageFinder", true));


        // Open the flow controller internal valve
        state("flowControllerTwoValveOverrideOpen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("flowControllerTwoValveOverrideOpen", true));
            // Check valve open
            validator("flowControllerTwoValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
            validator("flowControllerTwoValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("openFlowController_3", true));
                // Open the  valve, wait for pressure drop, open intenral flow controller valve, and then close
                openPressureClose(state("openFlowController_3", true),
                                  validator("openFlowController_3", true),
                                  state("closeFlowController_3", true),
                                  validator("closeFlowController_3", true),
                                  state("openFastExhuastPathWaitingFlowTwoPipe", true),
                                  state("closeFastExhuastPathWaitingFlowTwoPipe", true),
                                  state("waitForPressureAfterFlowTwoPipe", true),
                                  validator("pressureAfterFlowTwoPipe", true),
                                  state("flowControllerTwoValveOverrideOff", true),
                                  &sm_stopAsFailed);
                    // Turn off valve override
                    state("flowControllerTwoValveOverrideOff", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("flowControllerTwoValveOverrideOff", true));
                        // Check valve override off
                        validator("flowControllerTwoValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
                        validator("flowControllerTwoValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("sml_stageFinder", true));
    }



    /**
     * Select which stage we are up to
     *
     * @brief Vent::stageFinder
     */
    void Vent::stageFinder()
    {
        // Reset valve four
        fastExhuastOpen = false;

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
    void Vent::openPressureClose(QState* open,
                                 Functions::CommandValidatorState* openValidate,
                                 QState* close,
                                 Functions::CommandValidatorState* closeValidate,
                                 QState* openFastExhuast,
                                 QState* closeFastExhuast,
                                 QState* pressureWait,
                                 Functions::CommandValidatorState* pressureValidate,
                                 QState* finished,
                                 QState* failed)
    {
        // Open the output valve
        open->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, openValidate);
            // Failed close all valves
            openValidate->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, failed);
            // Success finish here
            openValidate->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, pressureWait);
                // Wait for pressure reading?
                pressureWait->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, pressureValidate);
                    // Are we at atmopheric
                    pressureValidate->addTransition(this, &Vent::emit_validationFailed, pressureWait);
                    pressureValidate->addTransition(this, &Vent::emit_openValveFour, openFastExhuast);
                        // Open valve four
                        openFastExhuast->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, pressureWait);
                    pressureValidate->addTransition(this, &Vent::emit_validationSuccess, closeFastExhuast);
                        // Close fast exhuast
                        closeFastExhuast->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, close);
                            // Close valve
                            close->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, closeValidate);
                                // Validate close
                                closeValidate->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, failed);
                                closeValidate->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, finished);
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

        // Get the max pressure allowed valve 4
        double maxPressureValveFour = 1500;

        // Get the min pressure allowed valv 4
        double minPressureValveFour = 500;

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
        else if( (pressureIn < maxPressureValveFour && pressureIn > minPressureValveFour) && !fastExhuastOpen)
        {
            // Emit safe to proceed
            emit emit_openValveFour();

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






