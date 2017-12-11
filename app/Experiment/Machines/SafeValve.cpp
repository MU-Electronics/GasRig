#include "SafeValve.h"

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
    SafeValve::SafeValve(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {
        // Link close valve states
        connect(state("closeHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("closeHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("closeFlowController", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
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
        connect(validator("openExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(validator("openOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("openSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(validator("openFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(validator("openVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("openVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);

    }

    SafeValve::~SafeValve()
    {
    }


    /**
     * Set the commands to be used by the machine
     *
     * @brief setParams
     * @param mintues
     * @param turbo
     * @param gasMode
     */
    void SafeValve::setParams(int id, bool state)
    {
        // ID of the valve in question
        params.insert("id", id);

        // State of the valve
        params.insert("state", state);
    }


    /**
     * Start the state machine
     *
     * @brief VacDown::start
     */
    void SafeValve::start()
    {
        machine.start();
    }


    /**
     * Start the state machine
     *
     * @brief VacDown::stopped
     */
    void SafeValve::stopped()
    {
    }


    /**
     * Builds the machine connections
     *
     * @brief VacDown::buildMachine
     */
    void SafeValve::buildMachine()
    {
        // Get the valve id
        int valveId = params.value("id").toInt();

        // Get the state
        bool state = params.value("state").toBool();

        // Build and run the correct state machine
        if(valveId == 1)
            valveOne(state);
        if(valveId == 2)
            valveTwo(state);
        if(valveId == 3)
            valveThree(state);
        if(valveId == 4)
            valveFour(state);
        if(valveId == 5)
            valveFive(state);
        if(valveId == 6)
            valveSix(state);
        if(valveId == 7)
            valveSeven(state);
        if(valveId == 8)
            valveEight(state);
        if(valveId == 9)
            valveNine(state);
    }



    /**
     * Opens / closes valve one and closes any other valves that should not be open
     *
     * @brief SafeValve::valveOne
     * @param state
     */
    void SafeValve::valveOne(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            machine.setInitialState(state("openOutput", true));
            // Open the output valve
            state("openOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openOutput", true));
                // Success finish here
                validator("openOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            machine.setInitialState(state("closeOutput", true));
            // Close the output valve
            state("closeOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeOutput", true));
                // Success finish here
                validator("closeOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }

    /**
     * Opens / closes valve two and closes any other valves that should not be open
     *
     * @brief SafeValve::valveTwo
     * @param state
     */
    void SafeValve::valveTwo(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
            machine.setInitialState(state("closeVacuumOut", true));

            // Close the vacuum otuput
            state("closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumOut", true));
                // Success open the slow exhuast valve
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openSlowExhuastPath", true));
                // Failed close all valves
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            state("openSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openSlowExhuastPath", true));
                // Success finish here
                validator("openSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeSlowExhuastPath", true));

            // Close the slow exhuast path valve
            state("closeSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeSlowExhuastPath", true));
                // Success finish here
                validator("closeSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve three and closes any other valves that should not be open
     *
     * @brief SafeValve::valveThree
     * @param state
     */
    void SafeValve::valveThree(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
            machine.setInitialState(state("closeVacuumIn", true));

            // Close the vacuum input
            state("closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumIn", true));
                // Success open the exhuast valve
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openExhuast", true));
                // Failed close all valves
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the exhuast valve
            state("openExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openExhuast", true));
                // Success finish here
                validator("openExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeExhuast", true));

            // Close the exhuast valve
            state("closeExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeExhuast", true));
                // Success finish here
                validator("closeExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve four and closes any other valves that should not be open
     *
     * @brief SafeValve::valveFour
     * @param state
     */
    void SafeValve::valveFour(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
            machine.setInitialState(state("closeVacuumOut", true));

            // Close the vacuum output
            state("closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumOut", true));
                // Success open the fast exhuast valve next
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openFastExhuastPath", true));
                // Failed close all valves
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the fast exhuast value
            state("openFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openFastExhuastPath", true));
                // Success finish here
                validator("openFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeFastExhuastPath", true));

            // Close the fast exhuast value
            state("closeFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeFastExhuastPath", true));
                // Success finish here
                validator("closeFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve five and closes any other valves that should not be open
     *
     * @brief SafeValve::valveFive
     * @param state
     */
    void SafeValve::valveFive(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
            machine.setInitialState(state("closeExhuast", true));


            // Close exhuaust
            state("closeExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeExhuast", true));
                // Success close the high pressure input
                validator("closeExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeHighPressureInput", true));
                // Failed close all valves
                validator("closeExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureInput", true));
                // Success open close the nitrogen input
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeHighPressureNitrogen", true));
                // Failed close all valves
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            state("closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureNitrogen", true));
                // Success close the flow controller input
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeFlowController", true));
                // Failed close all valves
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            state("closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeFlowController", true));
                // Success open the vacuum in input
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openVacuumIn", true));
                // Failed close all valves
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the vacuum input
            state("openVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openVacuumIn", true));
                // Success finish here
                validator("openVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeVacuumIn", true));

            // Close the vacuum input
            state("closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumIn", true));
                // Success finish here
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve six and closes any other valves that should not be open
     *
     * @brief SafeValve::valveSix
     * @param state
     */
    void SafeValve::valveSix(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
            machine.setInitialState(state("closeFastExhuastPath", true));

            // Close the fast exhuast path
            state("closeFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeFastExhuastPath", true));
                // Success close the slow exhuast path
                validator("closeFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeSlowExhuastPath", true));
                // Failed close all valves
                validator("closeFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Close the slow exhuast path
            state("closeSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeSlowExhuastPath", true));
                // Success open the vacuum output
                validator("closeSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openVacuumOut", true));
                // Failed close all valves
                validator("closeSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            state("openVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openVacuumOut", true));
                // Success finish here
                validator("openVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeVacuumOut", true));

            // Close the slow exhuast path valve
            state("closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumOut", true));
                // Success finish here
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve seven and closes any other valves that should not be open
     *
     * @brief SafeValve::valveSeven
     * @param state
     */
    void SafeValve::valveSeven(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
           machine.setInitialState(state("closeVacuumIn", true));


            // Close vacuum input
            state("closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumIn", true));
                // Success close the vacuum output
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeVacuumOut", true));
                // Failed close all valves
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumOut", true));
                // Success close the nitrogen input
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeHighPressureNitrogen", true));
                // Failed close all valves
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            state("closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureNitrogen", true));
                // Success close the flow controller input
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeFlowController", true));
                // Failed close all valves
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            state("closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeFlowController", true));
                // Success open the vacuum in input
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openHighPressureInput", true));
                // Failed close all valves
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            state("openHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openHighPressureInput", true));
                // Success finish here
                validator("openHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeHighPressureInput", true));

            // Close the vacuum input
            state("closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureInput", true));
                // Success finish here
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve eight and closes any other valves that should not be open
     *
     * @brief SafeValve::valveEight
     * @param state
     */
    void SafeValve::valveEight(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
            machine.setInitialState(state("closeVacuumIn", true));


            // Close vacuum input
            state("closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumIn", true));
                // Success close the vacuum output
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeVacuumOut", true));
                // Failed close all valves
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum input
            state("closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumOut", true));
                // Success close the nitrogen input
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeHighPressureNitrogen", true));
                // Failed close all valves
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            state("closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureNitrogen", true));
                // Success close the flow controller input
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeHighPressureInput", true));
                // Failed close all valves
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureInput", true));
                // Success open the flow controller
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openFlowController", true));
                // Failed close all valves
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            state("openFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openFlowController", true));
                // Success finish here
                validator("openFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeFlowController", true));

            // Close the flow controller
            state("closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeFlowController", true));
                // Success finish here
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve nine and closes any other valves that should not be open
     *
     * @brief SafeValve::valveNine
     * @param state
     */
    void SafeValve::valveNine(bool logic)
    {
        // Check the output valve opened / closed correctly
        if(logic == true)
        {
            // Set the starting point
            machine.setInitialState(state("closeVacuumIn", true));


            // Close vacuum input
            state("closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumIn", true));
                // Success close the vacuum output
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeVacuumOut", true));
                // Failed close all valves
                validator("closeVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum output
           state("closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeVacuumOut", true));
                // Success close the nitrogen input
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeFlowController", true));
                // Failed close all valves
                validator("closeVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller
            state("closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeFlowController", true));
                // Success close the flow controller input
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("closeHighPressureInput", true));
                // Failed close all valves
                validator("closeFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureInput", true));
                // Success open the flow controller
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("openHighPressureNitrogen", true));
                // Failed close all valves
                validator("closeHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            state("openHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("openHighPressureNitrogen", true));
                // Success finish here
                validator("openHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("openHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("closeHighPressureNitrogen", true));

            // Close the high pressure nitrogen
            state("closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setValveState, validator("closeHighPressureNitrogen", true));
                // Success finish here
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("closeHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }



}}}
