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
        // Set class name
        childClassName = QString::fromStdString(typeid(this).name());

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
    void SafeValve::beforeStart()
    {

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
            sm_master.setInitialState(state("openOutput", true));

            // Open the output valve
            transitionsBuilder()->openValve(state("openOutput", true), validator("openOutput", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            sm_master.setInitialState(state("closeOutput", true));

            // Close the output valve
            transitionsBuilder()->closeValve(state("closeOutput", true), validator("closeOutput", true), &sm_stop, &sm_stopAsFailed);
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
            sm_master.setInitialState(state("closeVacuumOut", true));

            // Close the vacuum otuput
            transitionsBuilder()->closeValve(state("closeVacuumOut", true), validator("closeVacuumOut", true), state("openSlowExhuastPath", true), &sm_stopAsFailed);

            // Open the slow exhuast path valve
            transitionsBuilder()->openValve(state("openSlowExhuastPath", true), validator("openSlowExhuastPath", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeSlowExhuastPath", true));

            // Close the slow exhuast path valve
            transitionsBuilder()->closeValve(state("closeSlowExhuastPath", true), validator("closeSlowExhuastPath", true), &sm_stop, &sm_stopAsFailed);
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
            sm_master.setInitialState(state("closeVacuumIn", true));

            // Close the vacuum input
            transitionsBuilder()->closeValve(state("closeVacuumIn", true), validator("closeVacuumIn", true), state("closeSlowExhuastPath", true), &sm_stopAsFailed);

            // Close slow exhuast valve
            transitionsBuilder()->closeValve(state("closeSlowExhuastPath", true), validator("closeSlowExhuastPath", true), state("closeFastExhuastPath", true), &sm_stopAsFailed);

            // Close fast exhust valve
            transitionsBuilder()->closeValve(state("closeFastExhuastPath", true), validator("closeFastExhuastPath", true), state("closeOutput", true), &sm_stopAsFailed);

            // Close output
            transitionsBuilder()->closeValve(state("closeOutput", true), validator("closeOutput", true), state("closeVacuumOut", true), &sm_stopAsFailed);

            // Close vacuum output
            transitionsBuilder()->closeValve(state("closeVacuumOut", true), validator("closeVacuumOut", true), state("closeHighPressureNitrogen", true), &sm_stopAsFailed);

            // Close the high pressure nitrogen
            transitionsBuilder()->closeValve(state("closeHighPressureNitrogen", true), validator("closeHighPressureNitrogen", true), state("closeFlowController", true), &sm_stopAsFailed);

            // Close the flow controller
            transitionsBuilder()->closeValve(state("closeFlowController", true), validator("closeFlowController", true), state("closeHighPressureInput", true), &sm_stopAsFailed);

            // Close high pressure in
            transitionsBuilder()->closeValve(state("closeHighPressureInput", true), validator("closeHighPressureInput", true), state("openExhuast", true), &sm_stopAsFailed);

            // Open the exhuast valve
            transitionsBuilder()->openValve(state("openExhuast", true), validator("openExhuast", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeExhuast", true));

            // Close the exhuast valve
            transitionsBuilder()->closeValve(state("closeExhuast", true), validator("closeExhuast", true), &sm_stop, &sm_stopAsFailed);
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
            sm_master.setInitialState(state("closeVacuumOut", true));

            // Close the vacuum output
            transitionsBuilder()->closeValve(state("closeVacuumOut", true), validator("closeVacuumOut", true), state("openFastExhuastPath", true), &sm_stopAsFailed);
            // Open the fast exhuast value
            transitionsBuilder()->openValve(state("openFastExhuastPath", true), validator("openFastExhuastPath", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeFastExhuastPath", true));

            // Close the fast exhuast value
            transitionsBuilder()->closeValve(state("closeFastExhuastPath", true), validator("closeFastExhuastPath", true), &sm_stop, &sm_stopAsFailed);
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
            sm_master.setInitialState(state("closeExhuast", true));

            // Close exhuaust
            transitionsBuilder()->closeValve(state("closeExhuast", true), validator("closeExhuast", true), state("closeHighPressureInput", true), &sm_stopAsFailed);

            // Close high pressure input
            transitionsBuilder()->closeValve(state("closeHighPressureInput", true), validator("closeHighPressureInput", true), state("closeHighPressureNitrogen", true), &sm_stopAsFailed);

            // Close nitrogen input
            transitionsBuilder()->closeValve(state("closeHighPressureNitrogen", true), validator("closeHighPressureNitrogen", true), state("closeFlowController", true), &sm_stopAsFailed);

            // Close flow controller input
            transitionsBuilder()->closeValve(state("closeFlowController", true), validator("closeFlowController", true), state("openVacuumIn", true), &sm_stopAsFailed);

            // Open the vacuum input
            transitionsBuilder()->openValve(state("openVacuumIn", true), validator("openVacuumIn", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeVacuumIn", true));

            // Close the vacuum input
            transitionsBuilder()->closeValve(state("closeVacuumIn", true), validator("closeVacuumIn", true), &sm_stop, &sm_stopAsFailed);
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
            sm_master.setInitialState(state("closeFastExhuastPath", true));

            // Close the fast exhuast path
            transitionsBuilder()->closeValve(state("closeFastExhuastPath", true), validator("closeFastExhuastPath", true), state("closeSlowExhuastPath", true), &sm_stopAsFailed);

            // Close the slow exhuast path
            transitionsBuilder()->closeValve(state("closeSlowExhuastPath", true), validator("closeSlowExhuastPath", true), state("openVacuumOut", true), &sm_stopAsFailed);

            // Open the slow exhuast path valve
            transitionsBuilder()->openValve(state("openVacuumOut", true), validator("openVacuumOut", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeVacuumOut", true));

            // Close the slow exhuast path valve
            transitionsBuilder()->closeValve(state("closeVacuumOut", true), validator("closeVacuumOut", true), &sm_stop, &sm_stopAsFailed);
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
           sm_master.setInitialState(state("closeVacuumIn", true));


            // Close vacuum input
            transitionsBuilder()->closeValve(state("closeVacuumIn", true), validator("closeVacuumIn", true), state("closeVacuumOut", true), &sm_stopAsFailed);

            // Close high pressure input
            transitionsBuilder()->closeValve(state("closeVacuumOut", true), validator("closeVacuumOut", true), state("closeHighPressureNitrogen", true), &sm_stopAsFailed);

            // Close nitrogen input
            transitionsBuilder()->closeValve(state("closeHighPressureNitrogen", true), validator("closeHighPressureNitrogen", true), state("closeFlowController", true), &sm_stopAsFailed);

            // Close flow controller input
            transitionsBuilder()->closeValve(state("closeFlowController", true), validator("closeFlowController", true), state("openHighPressureInput", true), &sm_stopAsFailed);

            // Open the high pressure input
            transitionsBuilder()->openValve(state("openHighPressureInput", true), validator("openHighPressureInput", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeHighPressureInput", true));

            // Close the vacuum input
            transitionsBuilder()->closeValve(state("closeHighPressureInput", true), validator("closeHighPressureInput", true), &sm_stop, &sm_stopAsFailed);
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
            sm_master.setInitialState(state("closeVacuumIn", true));

            // Close vacuum input
            transitionsBuilder()->closeValve(state("closeVacuumIn", true), validator("closeVacuumIn", true), state("closeVacuumOut", true), &sm_stopAsFailed);

            // Close vacuum input
            transitionsBuilder()->closeValve(state("closeVacuumOut", true), validator("closeVacuumOut", true), state("closeHighPressureNitrogen", true), &sm_stopAsFailed);

            // Close nitrogen input
            transitionsBuilder()->closeValve(state("closeHighPressureNitrogen", true), validator("closeHighPressureNitrogen", true), state("closeHighPressureInput", true), &sm_stopAsFailed);

            // Close high pressure input
            transitionsBuilder()->closeValve(state("closeHighPressureInput", true), validator("closeHighPressureInput", true), state("openFlowController", true), &sm_stopAsFailed);

            // Open the high pressure input
            transitionsBuilder()->openValve(state("openFlowController", true), validator("openFlowController", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeFlowController", true));

            // Close the flow controller
            transitionsBuilder()->closeValve(state("closeFlowController", true), validator("closeFlowController", true), &sm_stop, &sm_stopAsFailed);
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
            sm_master.setInitialState(state("closeVacuumIn", true));

            // Close vacuum input
            transitionsBuilder()->closeValve(state("closeVacuumIn", true), validator("closeVacuumIn", true), state("closeVacuumOut", true), &sm_stopAsFailed);

            // Close vacuum output
            transitionsBuilder()->closeValve(state("closeVacuumOut", true), validator("closeVacuumOut", true), state("closeFlowController", true), &sm_stopAsFailed);

            // Close flow controller
            transitionsBuilder()->closeValve(state("closeFlowController", true), validator("closeFlowController", true), state("closeHighPressureInput", true), &sm_stopAsFailed);

            // Close high pressure input
            transitionsBuilder()->closeValve(state("closeHighPressureInput", true), validator("closeHighPressureInput", true), state("openHighPressureNitrogen", true), &sm_stopAsFailed);

            // Open the high pressure input
            transitionsBuilder()->openValve(state("openHighPressureNitrogen", true), validator("openHighPressureNitrogen", true), &sm_stop, &sm_stopAsFailed);
        }
        else
        {
            // Set the starting point
            sm_master.setInitialState(state("closeHighPressureNitrogen", true));

            // Close the high pressure nitrogen
            transitionsBuilder()->closeValve(state("closeHighPressureNitrogen", true), validator("closeHighPressureNitrogen", true), &sm_stop, &sm_stopAsFailed);
        }
    }



}}}
