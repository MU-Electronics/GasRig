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
        connect(validator("sml_validatePressureAfterSlowExhuast", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("sml_validatePressureAfterOutput", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("sml_validatePressureAfterVacOutput", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("sml_validatePressureAfterFlowCavity", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("sml_validatePressureAfterNitrogenPipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("sml_validatePressureAfterMultiPipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("sml_validatePressureAfterFlowOnePipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);
        connect(validator("sml_validatePressureAfterFlowTwoPipe", true), &Functions::CommandValidatorState::entered, this, &Vent::validatePressureForAtmospheric);


        // Control flow cotnrollers override valve
        connect(state("sml_flowControllerOneValveOverrideOpen", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideOpen);
        connect(state("sml_flowControllerTwoValveOverrideOpen", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideOpen);
        connect(state("sml_flowControllerOneValveOverrideOff", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideOff);
        connect(state("sml_flowControllerTwoValveOverrideOff", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideOff);
        connect(state("sml_flowControllerOneValveOverrideClose", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerOneValveOverrideClose);
        connect(state("sml_flowControllerTwoValveOverrideClose", true), &QState::entered, this->flow(), &Functions::Flow::flowControllerTwoValveOverrideClose);
        connect(validator("sml_validateFlowControllerOneValveOverrideOpen", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideOpen);
        connect(validator("sml_validateFlowControllerTwoValveOverrideOpen", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideOpen);
        connect(validator("sml_validateFlowControllerOneValveOverrideClose", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideClose);
        connect(validator("sml_validateFlowControllerTwoValveOverrideClose", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideClose);
        connect(validator("sml_validateFlowControllerOneValveOverrideOff", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerOneValveOverrideOff);
        connect(validator("sml_validateFlowControllerTwoValveOverrideOff", true), &Functions::CommandValidatorState::entered, this->flow(), &Functions::Flow::validateFlowControllerTwoValveOverrideOff);



        // Fast exhuast
        connect(state("sml_openFastExhuastPathWaitingInternal", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openFastExhuastPathWaitingOutput", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openFastExhuastPathWaitingVacCavity", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openFastExhuastPathWaitingFlowCavity", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openFastExhuastPathWaitingNitrogenPipe", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openFastExhuastPathWaitingMultiFlow", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openFastExhuastPathWaitingFlowOnePipe", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openFastExhuastPathWaitingFlowTwoPipe", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);

        connect(state("sml_closeFastExhuastPathWaitingInternal", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeFastExhuastPathWaitingOutput", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeFastExhuastPathWaitingVacCavity", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeFastExhuastPathWaitingFlowCavity", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeFastExhuastPathWaitingNitrogenPipe", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeFastExhuastPathWaitingMultiFlow", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeFastExhuastPathWaitingFlowOnePipe", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeFastExhuastPathWaitingFlowTwoPipe", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);





        // Link close valve states
        connect(state("sml_closeHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("sml_closeHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("sml_closeFlowController", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("sml_closeFlowController_2", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("sml_closeFlowController_3", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
        connect(state("sml_closeExhuast", true), &QState::entered, this->valves(), &Functions::Valves::closeExhuast);
        connect(state("sml_closeOutput", true), &QState::entered, this->valves(), &Functions::Valves::closeOutput);
        connect(state("sml_closeSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeSlowExhuastPath);
        connect(state("sml_closeFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::closeFastExhuastPath);
        connect(state("sml_closeVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumIn);
        connect(state("sml_closeVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::closeVacuumOut);

        // Link close valve validator states
        connect(validator("sml_validateCloseHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureInput);
        connect(validator("sml_validateCloseHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseHighPressureNitrogen);
        connect(validator("sml_validateCloseFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("sml_validateCloseFlowController_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("sml_validateCloseFlowController_3", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFlowController);
        connect(validator("sml_validateCloseExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseExhuast);
        connect(validator("sml_validateCloseOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseOutput);
        connect(validator("sml_validateCloseSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseSlowExhuastPath);
        connect(validator("sml_validateCloseFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseFastExhuastPath);
        connect(validator("sml_validateCloseVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumIn);
        connect(validator("sml_validateCloseVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateCloseVacuumOut);

        // Link open valve states
        connect(state("sml_openHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureInput);
        connect(state("sml_openHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::openHighPressureNitrogen);
        connect(state("sml_openFlowController", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("sml_openFlowController_2", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("sml_openFlowController_3", true), &QState::entered, this->valves(), &Functions::Valves::openFlowController);
        connect(state("sml_openExhuast", true), &QState::entered, this->valves(), &Functions::Valves::openExhuast);
        connect(state("sml_openOutput", true), &QState::entered, this->valves(), &Functions::Valves::openOutput);
        connect(state("sml_openSlowExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openSlowExhuastPath);
        connect(state("sml_openFastExhuastPath", true), &QState::entered, this->valves(), &Functions::Valves::openFastExhuastPath);
        connect(state("sml_openVacuumIn", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumIn);
        connect(state("sml_openVacuumOut", true), &QState::entered, this->valves(), &Functions::Valves::openVacuumOut);

        // Link open valve validator states
        connect(validator("sml_validateOpenHighPressureInput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureInput);
        connect(validator("sml_validateOpenHighPressureNitrogen", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenHighPressureNitrogen);
        connect(validator("sml_validateOpenFlowController", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("sml_validateOpenFlowController_2", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("sml_validateOpenFlowController_3", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFlowController);
        connect(validator("sml_validateOpenExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(validator("sml_validateOpenOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("sml_validateOpenSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(validator("sml_validateOpenFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(validator("sml_validateOpenVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("sml_validateOpenVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);
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
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_openExhuast, state("sml_openExhuast", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_openSlowExhuast, state("sml_openSlowExhuastPath", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventOuput, state("sml_openOutput", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventVacuumOutput, state("sml_openVacuumOut", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventFlowCavity, state("sml_openFlowController", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventNitrogenPipes, state("sml_openHighPressureNitrogen", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventMultiPipes, state("sml_openHighPressureInput", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventFlowOnePipes, state("sml_flowControllerOneValveOverrideOpen", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_ventFlowTwoPipes, state("sml_flowControllerTwoValveOverrideOpen", true));
        state("sml_stageFinder", true)->addTransition(this, &Vent::emit_finished, &sm_stop);


        // Open exhuast wait for pressure drop
        state("sml_openExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenExhuast", true));
            // Failed close all valves
            validator("sml_validateOpenExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            validator("sml_validateOpenExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openSlowExhuastPath", true));

        // Open the slow exhuast valve and wait for pressure drop
        state("sml_openSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenSlowExhuastPath", true));
            // Failed close all valves
            validator("sml_validateOpenSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
            // Success finish here
            validator("sml_validateOpenSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_waitForPressureAfterSlowExhuast", true));
                // Wait for pressure reading?
                state("sml_waitForPressureAfterSlowExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_pressureSensorPressure, validator("sml_validatePressureAfterSlowExhuast", true));
                    // Are we at atmopheric
                    validator("sml_validatePressureAfterSlowExhuast", true)->addTransition(this, &Vent::emit_validationSuccess, state("sml_closeFastExhuastPathWaitingInternal", true));
                        // Close fast exhuast
                        state("sml_closeFastExhuastPathWaitingInternal", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, state("sml_stageFinder", true));
                            validator("sml_validatePressureAfterSlowExhuast", true)->addTransition(this, &Vent::emit_validationFailed, state("sml_waitForPressureAfterSlowExhuast", true));
                            validator("sml_validatePressureAfterSlowExhuast", true)->addTransition(this, &Vent::emit_openValveFour, state("sml_openFastExhuastPathWaitingInternal", true));
                        // Open fast exhuast
                        state("sml_openFastExhuastPathWaitingInternal", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, state("sml_waitForPressureAfterSlowExhuast", true));


        // Open the output valve, wait for pressure drop and then close
        openPressureClose(state("sml_openOutput", true),
                          validator("sml_validateOpenOutput", true),
                          state("sml_closeOutput", true),
                          validator("sml_validateCloseOutput", true),
                          state("sml_openFastExhuastPathWaitingOutput", true),
                          state("sml_closeFastExhuastPathWaitingOutput", true),
                          state("sml_waitForPressureAfterOutput", true),
                          validator("sml_validatePressureAfterOutput", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("sml_openVacuumOut", true),
                          validator("sml_validateOpenVacuumOut", true),
                          state("sml_closeVacuumOut", true),
                          validator("sml_validateCloseVacuumOut", true),
                          state("sml_openFastExhuastPathWaitingVacCavity", true),
                          state("sml_closeFastExhuastPathWaitingVacCavity", true),
                          state("sml_waitForPressureAfterVacOutput", true),
                          validator("sml_validatePressureAfterVacOutput", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("sml_openFlowController", true),
                          validator("sml_validateOpenFlowController", true),
                          state("sml_closeFlowController", true),
                          validator("sml_validateCloseFlowController", true),
                          state("sml_openFastExhuastPathWaitingFlowCavity", true),
                          state("sml_closeFastExhuastPathWaitingFlowCavity", true),
                          state("sml_waitForPressureAfterFlowCavity", true),
                          validator("sml_validatePressureAfterFlowCavity", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("sml_openHighPressureNitrogen", true),
                          validator("sml_validateOpenHighPressureNitrogen", true),
                          state("sml_closeHighPressureNitrogen", true),
                          validator("sml_validateCloseHighPressureNitrogen", true),
                          state("sml_openFastExhuastPathWaitingNitrogenPipe", true),
                          state("sml_closeFastExhuastPathWaitingNitrogenPipe", true),
                          state("sml_waitForPressureAfterNitrogenPipe", true),
                          validator("sml_validatePressureAfterNitrogenPipe", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the  valve, wait for pressure drop and then close
        openPressureClose(state("sml_openHighPressureInput", true),
                          validator("sml_validateOpenHighPressureInput", true),
                          state("sml_closeHighPressureInput", true),
                          validator("sml_validateCloseHighPressureInput", true),
                          state("sml_openFastExhuastPathWaitingMultiFlow", true),
                          state("sml_closeFastExhuastPathWaitingMultiFlow", true),
                          state("sml_waitForPressureAfterMultiPipe", true),
                          validator("sml_validatePressureAfterMultiPipe", true),
                          state("sml_stageFinder", true),
                          &sm_stopAsFailed);

        // Open the flow controller internal valve
        state("sml_flowControllerOneValveOverrideOpen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("sml_validateFlowControllerOneValveOverrideOpen", true));
            // Check valve open
            validator("sml_validateFlowControllerOneValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
            validator("sml_validateFlowControllerOneValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("sml_openFlowController_2", true));
                // Open flow controller cavity valve, wait for pressure drop, open intenral flow controller valve, and then close
                openPressureClose(state("sml_openFlowController_2", true),
                                  validator("sml_validateOpenFlowController_2", true),
                                  state("sml_closeFlowController_2", true),
                                  validator("sml_validateCloseFlowController_2", true),
                                  state("sml_openFastExhuastPathWaitingFlowOnePipe", true),
                                  state("sml_closeFastExhuastPathWaitingFlowOnePipe", true),
                                  state("sml_waitForPressureAfterFlowOnePipe", true),
                                  validator("sml_validatePressureAfterFlowOnePipe", true),
                                  state("sml_flowControllerOneValveOverrideOff", true),
                                  &sm_stopAsFailed);
                    // Turn off valve override
                    state("sml_flowControllerOneValveOverrideOff", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("sml_validateFlowControllerOneValveOverrideOff", true));
                        // Check valve override off
                        validator("sml_validateFlowControllerOneValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
                        validator("sml_validateFlowControllerOneValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("sml_stageFinder", true));


        // Open the flow controller internal valve
        state("sml_flowControllerTwoValveOverrideOpen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("sml_validateFlowControllerTwoValveOverrideOpen", true));
            // Check valve open
            validator("sml_validateFlowControllerTwoValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
            validator("sml_validateFlowControllerTwoValveOverrideOpen", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("sml_openFlowController_3", true));
                // Open the  valve, wait for pressure drop, open intenral flow controller valve, and then close
                openPressureClose(state("sml_openFlowController_3", true),
                                  validator("sml_validateOpenFlowController_3", true),
                                  state("sml_closeFlowController_3", true),
                                  validator("sml_validateCloseFlowController_3", true),
                                  state("sml_openFastExhuastPathWaitingFlowTwoPipe", true),
                                  state("sml_closeFastExhuastPathWaitingFlowTwoPipe", true),
                                  state("sml_waitForPressureAfterFlowTwoPipe", true),
                                  validator("sml_validatePressureAfterFlowTwoPipe", true),
                                  state("sml_flowControllerTwoValveOverrideOff", true),
                                  &sm_stopAsFailed);
                    // Turn off valve override
                    state("sml_flowControllerTwoValveOverrideOff", true)->addTransition(&m_hardware, &Hardware::Access::emit_setFlowControllerValveOverride, validator("sml_validateFlowControllerTwoValveOverrideOff", true));
                        // Check valve override off
                        validator("sml_validateFlowControllerTwoValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationFailed, &sm_stopAsFailed);
                        validator("sml_validateFlowControllerTwoValveOverrideOff", true)->addTransition(this->flow(), &Functions::Flow::emit_validationSuccess, state("sml_stageFinder", true));
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






