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
        (state == true) ? machine.setInitialState(&sm_openOutput) : machine.setInitialState(&sm_closeOutput);

        // Check the output valve opened / closed correctly
        if(state == true)
        {
            // Open the output valve
            sm_openOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenOutput);
                // Success finish here
                sm_validateOpenOutput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenOutput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Close the output valve
            sm_closeOutput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseOutput);
                // Success finish here
                sm_validateCloseOutput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseOutput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeVacuumOut);

            // Close the vacuum output
            sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumOut);
                // Success open the fast exhuast valve next
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openFastExhuastPath);
                // Failed close all valves
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);

            // Open the fast exhuast value
            sm_openFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenFastExhuastPath);
                // Success finish here
                sm_validateOpenFastExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenFastExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeFastExhuastPath);

            // Close the fast exhuast value
            sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFastExhuastPath);
                // Success finish here
                sm_validateCloseFastExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseFastExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeVacuumIn);

            // Close the vacuum input
            sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumIn);
                // Success open the exhuast valve
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openExhuast);
                // Failed close all valves
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);

            // Open the exhuast valve
            sm_openExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenExhuast);
                // Success finish here
                sm_validateOpenExhuast.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenExhuast.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeExhuast);

            // Close the exhuast valve
            sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseExhuast);
                // Success finish here
                sm_validateCloseExhuast.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseExhuast.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeVacuumOut);

            // Close the vacuum otuput
            sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumOut);
                // Success open the slow exhuast valve
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openSlowExhuastPath);
                // Failed close all valves
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            sm_openSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenSlowExhuastPath);
                // Success finish here
                sm_validateOpenSlowExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenSlowExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeSlowExhuastPath);

            // Close the slow exhuast path valve
            sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseSlowExhuastPath);
                // Success finish here
                sm_validateCloseSlowExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseSlowExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeExhuast);


            // Close exhuaust
            sm_closeExhuast.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseExhuast);
                // Success close the high pressure input
                sm_validateCloseExhuast.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureInput);
                // Failed close all valves
                sm_validateCloseExhuast.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureInput);
                // Success open close the nitrogen input
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureNitrogen);
                // Failed close all valves
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeFlowController);
                // Failed close all valves
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFlowController);
                // Success open the vacuum in input
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openVacuumIn);
                // Failed close all valves
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Open the vacuum input
            sm_openVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenVacuumIn);
                // Success finish here
                sm_validateOpenVacuumIn.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenVacuumIn.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeVacuumIn);

            // Close the vacuum input
            sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumIn);
                // Success finish here
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeFastExhuastPath);

            // Close the fast exhuast path
            sm_closeFastExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFastExhuastPath);
                // Success close the slow exhuast path
                sm_validateCloseFastExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeSlowExhuastPath);
                // Failed close all valves
                sm_validateCloseFastExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);

            // Close the slow exhuast path
            sm_closeSlowExhuastPath.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseSlowExhuastPath);
                // Success open the vacuum output
                sm_validateCloseSlowExhuastPath.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openVacuumOut);
                // Failed close all valves
                sm_validateCloseSlowExhuastPath.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);

            // Open the slow exhuast path valve
            sm_openVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenVacuumOut);
                // Success finish here
                sm_validateOpenVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeVacuumOut);

            // Close the slow exhuast path valve
            sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumOut);
                // Success finish here
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeVacuumIn);


            // Close vacuum input
            sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumIn);
                // Success close the vacuum output
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeVacuumOut);
                // Failed close all valves
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumOut);
                // Success close the nitrogen input
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureNitrogen);
                // Failed close all valves
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeFlowController);
                // Failed close all valves
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller input
            sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFlowController);
                // Success open the vacuum in input
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openHighPressureInput);
                // Failed close all valves
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            sm_openHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenHighPressureInput);
                // Success finish here
                sm_validateOpenHighPressureInput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenHighPressureInput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeHighPressureInput);

            // Close the vacuum input
            sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureInput);
                // Success finish here
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeVacuumIn);


            // Close vacuum input
            sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumIn);
                // Success close the vacuum output
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeVacuumOut);
                // Failed close all valves
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum input
            sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumOut);
                // Success close the nitrogen input
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureNitrogen);
                // Failed close all valves
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close nitrogen input
            sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureNitrogen);
                // Success close the flow controller input
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureInput);
                // Failed close all valves
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureInput);
                // Success open the flow controller
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openFlowController);
                // Failed close all valves
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            sm_openFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenFlowController);
                // Success finish here
                sm_validateOpenFlowController.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenFlowController.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeFlowController);

            // Close the flow controller
            sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFlowController);
                // Success finish here
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
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
            machine.setInitialState(&sm_closeVacuumIn);


            // Close vacuum input
            sm_closeVacuumIn.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumIn);
                // Success close the vacuum output
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeVacuumOut);
                // Failed close all valves
                sm_validateCloseVacuumIn.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close vacuum output
            sm_closeVacuumOut.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseVacuumOut);
                // Success close the nitrogen input
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeFlowController);
                // Failed close all valves
                sm_validateCloseVacuumOut.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close flow controller
            sm_closeFlowController.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseFlowController);
                // Success close the flow controller input
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationSuccess, &sm_closeHighPressureInput);
                // Failed close all valves
                sm_validateCloseFlowController.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Close high pressure input
            sm_closeHighPressureInput.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureInput);
                // Success open the flow controller
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationSuccess, &sm_openHighPressureNitrogen);
                // Failed close all valves
                sm_validateCloseHighPressureInput.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);


            // Open the high pressure input
            sm_openHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateOpenHighPressureNitrogen);
                // Success finish here
                sm_validateOpenHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateOpenHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            machine.setInitialState(&sm_closeHighPressureNitrogen);

            // Close the high pressure nitrogen
            sm_closeHighPressureNitrogen.addTransition(&m_hardware, &Hardware::Access::emit_setDigitalPort, &sm_validateCloseHighPressureNitrogen);
                // Success finish here
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationSuccess, &sm_stop);
                // Failed close all valves
                sm_validateCloseHighPressureNitrogen.addTransition(this, &MachineStates::emit_validationFailed, &sm_stopAsFailed);
        }
    }



}}}






