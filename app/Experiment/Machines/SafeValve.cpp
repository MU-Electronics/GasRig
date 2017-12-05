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
        connect(state("sml_closeHighPressureInput", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureInput);
        connect(state("sml_closeHighPressureNitrogen", true), &QState::entered, this->valves(), &Functions::Valves::closeHighPressureNitrogen);
        connect(state("sml_closeFlowController", true), &QState::entered, this->valves(), &Functions::Valves::closeFlowController);
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
        connect(validator("sml_validateOpenExhuast", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenExhuast);
        connect(validator("sml_validateOpenOutput", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenOutput);
        connect(validator("sml_validateOpenSlowExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenSlowExhuastPath);
        connect(validator("sml_validateOpenFastExhuastPath", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenFastExhuastPath);
        connect(validator("sml_validateOpenVacuumIn", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumIn);
        connect(validator("sml_validateOpenVacuumOut", true), &Functions::CommandValidatorState::entered, this->valves(), &Functions::Valves::validateOpenVacuumOut);

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
            machine.setInitialState(state("sml_openOutput", true));
            // Open the output valve
            state("sml_openOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenOutput", true));
                // Success finish here
                validator("sml_validateOpenOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            machine.setInitialState(state("sml_closeOutput", true));
            // Close the output valve
            state("sml_closeOutput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseOutput", true));
                // Success finish here
                validator("sml_validateCloseOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseOutput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(state("sml_closeVacuumOut", true));

            // Close the vacuum otuput
            state("sml_closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumOut", true));
                // Success open the slow exhuast valve
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openSlowExhuastPath", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            state("sml_openSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenSlowExhuastPath", true));
                // Success finish here
                validator("sml_validateOpenSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeSlowExhuastPath", true));

            // Close the slow exhuast path valve
            state("sml_closeSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseSlowExhuastPath", true));
                // Success finish here
                validator("sml_validateCloseSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(state("sml_closeVacuumIn", true));

            // Close the vacuum input
            state("sml_closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumIn", true));
                // Success open the exhuast valve
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openExhuast", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the exhuast valve
            state("sml_openExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenExhuast", true));
                // Success finish here
                validator("sml_validateOpenExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeExhuast", true));

            // Close the exhuast valve
            state("sml_closeExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseExhuast", true));
                // Success finish here
                validator("sml_validateCloseExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(state("sml_closeVacuumOut", true));

            // Close the vacuum output
            state("sml_closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumOut", true));
                // Success open the fast exhuast valve next
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openFastExhuastPath", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the fast exhuast value
            state("sml_openFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenFastExhuastPath", true));
                // Success finish here
                validator("sml_validateOpenFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeFastExhuastPath", true));

            // Close the fast exhuast value
            state("sml_closeFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFastExhuastPath", true));
                // Success finish here
                validator("sml_validateCloseFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(state("sml_closeExhuast", true));


            // Close exhuaust
            state("sml_closeExhuast", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseExhuast", true));
                // Success close the high pressure input
                validator("sml_validateCloseExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeHighPressureInput", true));
                // Failed close all valves
                validator("sml_validateCloseExhuast", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("sml_closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureInput", true));
                // Success open close the nitrogen input
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeHighPressureNitrogen", true));
                // Failed close all valves
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            state("sml_closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureNitrogen", true));
                // Success close the flow controller input
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeFlowController", true));
                // Failed close all valves
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            state("sml_closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFlowController", true));
                // Success open the vacuum in input
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openVacuumIn", true));
                // Failed close all valves
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the vacuum input
            state("sml_openVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenVacuumIn", true));
                // Success finish here
                validator("sml_validateOpenVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeVacuumIn", true));

            // Close the vacuum input
            state("sml_closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumIn", true));
                // Success finish here
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(state("sml_closeFastExhuastPath", true));

            // Close the fast exhuast path
            state("sml_closeFastExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFastExhuastPath", true));
                // Success close the slow exhuast path
                validator("sml_validateCloseFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeSlowExhuastPath", true));
                // Failed close all valves
                validator("sml_validateCloseFastExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Close the slow exhuast path
            state("sml_closeSlowExhuastPath", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseSlowExhuastPath", true));
                // Success open the vacuum output
                validator("sml_validateCloseSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openVacuumOut", true));
                // Failed close all valves
                validator("sml_validateCloseSlowExhuastPath", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            state("sml_openVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenVacuumOut", true));
                // Success finish here
                validator("sml_validateOpenVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeVacuumOut", true));

            // Close the slow exhuast path valve
            state("sml_closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumOut", true));
                // Success finish here
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
           machine.setInitialState(state("sml_closeVacuumIn", true));


            // Close vacuum input
            state("sml_closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumIn", true));
                // Success close the vacuum output
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeVacuumOut", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("sml_closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumOut", true));
                // Success close the nitrogen input
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeHighPressureNitrogen", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            state("sml_closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureNitrogen", true));
                // Success close the flow controller input
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeFlowController", true));
                // Failed close all valves
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            state("sml_closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFlowController", true));
                // Success open the vacuum in input
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openHighPressureInput", true));
                // Failed close all valves
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            state("sml_openHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenHighPressureInput", true));
                // Success finish here
                validator("sml_validateOpenHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeHighPressureInput", true));

            // Close the vacuum input
            state("sml_closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureInput", true));
                // Success finish here
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(state("sml_closeVacuumIn", true));


            // Close vacuum input
            state("sml_closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumIn", true));
                // Success close the vacuum output
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeVacuumOut", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum input
            state("sml_closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumOut", true));
                // Success close the nitrogen input
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeHighPressureNitrogen", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            state("sml_closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureNitrogen", true));
                // Success close the flow controller input
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeHighPressureInput", true));
                // Failed close all valves
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("sml_closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureInput", true));
                // Success open the flow controller
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openFlowController", true));
                // Failed close all valves
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            state("sml_openFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenFlowController", true));
                // Success finish here
                validator("sml_validateOpenFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeFlowController", true));

            // Close the flow controller
            state("sml_closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFlowController", true));
                // Success finish here
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(state("sml_closeVacuumIn", true));


            // Close vacuum input
            state("sml_closeVacuumIn", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumIn", true));
                // Success close the vacuum output
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeVacuumOut", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumIn", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum output
           state("sml_closeVacuumOut", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseVacuumOut", true));
                // Success close the nitrogen input
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeFlowController", true));
                // Failed close all valves
                validator("sml_validateCloseVacuumOut", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller
            state("sml_closeFlowController", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseFlowController", true));
                // Success close the flow controller input
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_closeHighPressureInput", true));
                // Failed close all valves
                validator("sml_validateCloseFlowController", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            state("sml_closeHighPressureInput", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureInput", true));
                // Success open the flow controller
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, state("sml_openHighPressureNitrogen", true));
                // Failed close all valves
                validator("sml_validateCloseHighPressureInput", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            state("sml_openHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateOpenHighPressureNitrogen", true));
                // Success finish here
                validator("sml_validateOpenHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateOpenHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(state("sml_closeHighPressureNitrogen", true));

            // Close the high pressure nitrogen
            state("sml_closeHighPressureNitrogen", true)->addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, validator("sml_validateCloseHighPressureNitrogen", true));
                // Success finish here
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                validator("sml_validateCloseHighPressureNitrogen", true)->addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }



}}}
