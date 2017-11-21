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
    {
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
     * @brief VacDown::start
     */
    void SafeValve::stop()
    {
        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_safeValveFinished(params);
    }


    /**
     * Stop the state machine as it failed somewhere
     *
     * @brief VacDown::start
     */
    void SafeValve::stopAsFailed()
    {
        // Close all valves as we had a failure somewhere


        // Stop the machine
        machine.stop();

        // Get all states from machine and loop through them
        removeAllTransitions();

        // Emit the machine is finished
        emit emit_safeValveFailed(params);
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
    void SafeValve::valveOne(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            machine.setInitialState(&sml_openOutput);
            // Open the output valve
            sml_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenOutput);
                // Success finish here
                sml_validateOpenOutput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenOutput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            machine.setInitialState(&sml_closeOutput);
            // Close the output valve
            sml_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseOutput);
                // Success finish here
                sml_validateCloseOutput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseOutput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }

    /**
     * Opens / closes valve two and closes any other valves that should not be open
     *
     * @brief SafeValve::valveTwo
     * @param state
     */
    void SafeValve::valveTwo(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumOut);

            // Close the vacuum otuput
            sml_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumOut);
                // Success open the slow exhuast valve
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openSlowExhuastPath);
                // Failed close all valves
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            sml_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenSlowExhuastPath);
                // Success finish here
                sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeSlowExhuastPath);

            // Close the slow exhuast path valve
            sml_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseSlowExhuastPath);
                // Success finish here
                sml_validateCloseSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve three and closes any other valves that should not be open
     *
     * @brief SafeValve::valveThree
     * @param state
     */
    void SafeValve::valveThree(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumIn);

            // Close the vacuum input
            sml_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumIn);
                // Success open the exhuast valve
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openExhuast);
                // Failed close all valves
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the exhuast valve
            sml_openExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenExhuast);
                // Success finish here
                sml_validateOpenExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeExhuast);

            // Close the exhuast valve
            sml_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseExhuast);
                // Success finish here
                sml_validateCloseExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve four and closes any other valves that should not be open
     *
     * @brief SafeValve::valveFour
     * @param state
     */
    void SafeValve::valveFour(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumOut);

            // Close the vacuum output
            sml_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumOut);
                // Success open the fast exhuast valve next
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openFastExhuastPath);
                // Failed close all valves
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the fast exhuast value
            sml_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenFastExhuastPath);
                // Success finish here
                sml_validateOpenFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeFastExhuastPath);

            // Close the fast exhuast value
            sml_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFastExhuastPath);
                // Success finish here
                sml_validateCloseFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve five and closes any other valves that should not be open
     *
     * @brief SafeValve::valveFive
     * @param state
     */
    void SafeValve::valveFive(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeExhuast);


            // Close exhuaust
            sml_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseExhuast);
                // Success close the high pressure input
                sml_validateCloseExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeHighPressureInput);
                // Failed close all valves
                sml_validateCloseExhuast.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sml_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureInput);
                // Success open close the nitrogen input
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeHighPressureNitrogen);
                // Failed close all valves
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            sml_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeFlowController);
                // Failed close all valves
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            sml_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFlowController);
                // Success open the vacuum in input
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openVacuumIn);
                // Failed close all valves
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the vacuum input
            sml_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenVacuumIn);
                // Success finish here
                sml_validateOpenVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumIn);

            // Close the vacuum input
            sml_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumIn);
                // Success finish here
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve six and closes any other valves that should not be open
     *
     * @brief SafeValve::valveSix
     * @param state
     */
    void SafeValve::valveSix(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeFastExhuastPath);

            // Close the fast exhuast path
            sml_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFastExhuastPath);
                // Success close the slow exhuast path
                sml_validateCloseFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeSlowExhuastPath);
                // Failed close all valves
                sml_validateCloseFastExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Close the slow exhuast path
            sml_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseSlowExhuastPath);
                // Success open the vacuum output
                sml_validateCloseSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openVacuumOut);
                // Failed close all valves
                sml_validateCloseSlowExhuastPath.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            sml_openVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenVacuumOut);
                // Success finish here
                sml_validateOpenVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumOut);

            // Close the slow exhuast path valve
            sml_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumOut);
                // Success finish here
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve seven and closes any other valves that should not be open
     *
     * @brief SafeValve::valveSeven
     * @param state
     */
    void SafeValve::valveSeven(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumIn);


            // Close vacuum input
            sml_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumIn);
                // Success close the vacuum output
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeVacuumOut);
                // Failed close all valves
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sml_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumOut);
                // Success close the nitrogen input
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeHighPressureNitrogen);
                // Failed close all valves
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            sml_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeFlowController);
                // Failed close all valves
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            sml_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFlowController);
                // Success open the vacuum in input
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openHighPressureInput);
                // Failed close all valves
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            sml_openHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenHighPressureInput);
                // Success finish here
                sml_validateOpenHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeHighPressureInput);

            // Close the vacuum input
            sml_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureInput);
                // Success finish here
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve eight and closes any other valves that should not be open
     *
     * @brief SafeValve::valveEight
     * @param state
     */
    void SafeValve::valveEight(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumIn);


            // Close vacuum input
            sml_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumIn);
                // Success close the vacuum output
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeVacuumOut);
                // Failed close all valves
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum input
            sml_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumOut);
                // Success close the nitrogen input
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeHighPressureNitrogen);
                // Failed close all valves
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            sml_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeHighPressureInput);
                // Failed close all valves
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sml_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureInput);
                // Success open the flow controller
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openFlowController);
                // Failed close all valves
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            sml_openFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenFlowController);
                // Success finish here
                sml_validateOpenFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeFlowController);

            // Close the flow controller
            sml_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFlowController);
                // Success finish here
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }


    /**
     * Opens / closes valve nine and closes any other valves that should not be open
     *
     * @brief SafeValve::valveNine
     * @param state
     */
    void SafeValve::valveNine(bool state)
    {
        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Set the starting point
            machine.setInitialState(&sml_closeVacuumIn);


            // Close vacuum input
            sml_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumIn);
                // Success close the vacuum output
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeVacuumOut);
                // Failed close all valves
                sml_validateCloseVacuumIn.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum output
            sml_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseVacuumOut);
                // Success close the nitrogen input
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeFlowController);
                // Failed close all valves
                sml_validateCloseVacuumOut.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller
            sml_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseFlowController);
                // Success close the flow controller input
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_closeHighPressureInput);
                // Failed close all valves
                sml_validateCloseFlowController.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sml_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureInput);
                // Success open the flow controller
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sml_openHighPressureNitrogen);
                // Failed close all valves
                sml_validateCloseHighPressureInput.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            sml_openHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateOpenHighPressureNitrogen);
                // Success finish here
                sml_validateOpenHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateOpenHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sml_closeHighPressureNitrogen);

            // Close the high pressure nitrogen
            sml_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sml_validateCloseHighPressureNitrogen);
                // Success finish here
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sml_validateCloseHighPressureNitrogen.addTransition(this->valves(), &Functions::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }



}}}






