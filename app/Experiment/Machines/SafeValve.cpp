#include "SafeValve.h"

// Include extenral deps
#include <QObject>

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Include possable machine states
#include "States/MachineStates.h"

namespace App { namespace Experiment { namespace Machines
{
    SafeValve::SafeValve(QObject *parent, Settings::Container settings, Hardware::Access& hardware, Safety::Monitor& safety)
        :   MachineStates(parent, settings, hardware, safety)
    {

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
        // Where to start the machine
        (state == true) ? machine.setInitialState(&valves()->sm_openOutput) : machine.setInitialState(&valves()->sm_closeOutput);

        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Open the output valve
            valves()->sm_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenOutput);
                // Success finish here
                valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Close the output valve
            valves()->sm_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseOutput);
                // Success finish here
                valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseOutput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeVacuumOut);

            // Close the vacuum output
            valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
                // Success open the fast exhuast valve next
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openFastExhuastPath);
                // Failed close all valves
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the fast exhuast value
            valves()->sm_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenFastExhuastPath);
                // Success finish here
                valves()->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeFastExhuastPath);

            // Close the fast exhuast value
            valves()->sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFastExhuastPath);
                // Success finish here
                valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeVacuumIn);

            // Close the vacuum input
            valves()->sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumIn);
                // Success open the exhuast valve
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openExhuast);
                // Failed close all valves
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the exhuast valve
            valves()->sm_openExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenExhuast);
                // Success finish here
                valves()->sm_validateOpenExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeExhuast);

            // Close the exhuast valve
            valves()->sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseExhuast);
                // Success finish here
                valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeVacuumOut);

            // Close the vacuum otuput
            valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
                // Success open the slow exhuast valve
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openSlowExhuastPath);
                // Failed close all valves
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            valves()->sm_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenSlowExhuastPath);
                // Success finish here
                valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeSlowExhuastPath);

            // Close the slow exhuast path valve
            valves()->sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseSlowExhuastPath);
                // Success finish here
                valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeExhuast);


            // Close exhuaust
            valves()->sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseExhuast);
                // Success close the high pressure input
                valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeHighPressureInput);
                // Failed close all valves
                valves()->sm_validateCloseExhuast.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            valves()->sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureInput);
                // Success open close the nitrogen input
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeHighPressureNitrogen);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            valves()->sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeFlowController);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            valves()->sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFlowController);
                // Success open the vacuum in input
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openVacuumIn);
                // Failed close all valves
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the vacuum input
            valves()->sm_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenVacuumIn);
                // Success finish here
                valves()->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeVacuumIn);

            // Close the vacuum input
            valves()->sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumIn);
                // Success finish here
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeFastExhuastPath);

            // Close the fast exhuast path
            valves()->sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFastExhuastPath);
                // Success close the slow exhuast path
                valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeSlowExhuastPath);
                // Failed close all valves
                valves()->sm_validateCloseFastExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Close the slow exhuast path
            valves()->sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseSlowExhuastPath);
                // Success open the vacuum output
                valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openVacuumOut);
                // Failed close all valves
                valves()->sm_validateCloseSlowExhuastPath.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            valves()->sm_openVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenVacuumOut);
                // Success finish here
                valves()->sm_validateOpenVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeVacuumOut);

            // Close the slow exhuast path valve
            valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
                // Success finish here
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeVacuumIn);


            // Close vacuum input
            valves()->sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumIn);
                // Success close the vacuum output
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeVacuumOut);
                // Failed close all valves
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
                // Success close the nitrogen input
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeHighPressureNitrogen);
                // Failed close all valves
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            valves()->sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeFlowController);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            valves()->sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFlowController);
                // Success open the vacuum in input
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openHighPressureInput);
                // Failed close all valves
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            valves()->sm_openHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenHighPressureInput);
                // Success finish here
                valves()->sm_validateOpenHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeHighPressureInput);

            // Close the vacuum input
            valves()->sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureInput);
                // Success finish here
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeVacuumIn);


            // Close vacuum input
            valves()->sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumIn);
                // Success close the vacuum output
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeVacuumOut);
                // Failed close all valves
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum input
            valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
                // Success close the nitrogen input
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeHighPressureNitrogen);
                // Failed close all valves
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            valves()->sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeHighPressureInput);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            valves()->sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureInput);
                // Success open the flow controller
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openFlowController);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            valves()->sm_openFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenFlowController);
                // Success finish here
                valves()->sm_validateOpenFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeFlowController);

            // Close the flow controller
            valves()->sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFlowController);
                // Success finish here
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&valves()->sm_closeVacuumIn);


            // Close vacuum input
            valves()->sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumIn);
                // Success close the vacuum output
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeVacuumOut);
                // Failed close all valves
                valves()->sm_validateCloseVacuumIn.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum output
            valves()->sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseVacuumOut);
                // Success close the nitrogen input
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeFlowController);
                // Failed close all valves
                valves()->sm_validateCloseVacuumOut.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller
            valves()->sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseFlowController);
                // Success close the flow controller input
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_closeHighPressureInput);
                // Failed close all valves
                valves()->sm_validateCloseFlowController.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            valves()->sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureInput);
                // Success open the flow controller
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &valves()->sm_openHighPressureNitrogen);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureInput.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            valves()->sm_openHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateOpenHighPressureNitrogen);
                // Success finish here
                valves()->sm_validateOpenHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateOpenHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&valves()->sm_closeHighPressureNitrogen);

            // Close the high pressure nitrogen
            valves()->sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &valves()->sm_validateCloseHighPressureNitrogen);
                // Success finish here
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                valves()->sm_validateCloseHighPressureNitrogen.addTransition(this->valves(), &States::Valves::emit_validationFailed, &sm_stopAsFailed);
        }
    }



}}}






