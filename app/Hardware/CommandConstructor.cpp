#include "CommandConstructor.h"


namespace App { namespace Hardware
{
    CommandConstructor::CommandConstructor(QObject *parent)
    {

    }


    /**
     * Class detructure performs:
     *      - Nothing atm
     *
     * @brief CommandConstructor::~CommandConstructor
     */
    CommandConstructor::~CommandConstructor()
    {

    }









    /**
     *
     *
     * @brief CommandConstructor::setFlowControllerValveOverride
     */
    QVariantMap CommandConstructor::setFlowControllerValveOverride(QString controller, int state)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setValveOverride");

        // Set the state of the overrie
        command.insert("state", QString::number(state));

        // Return the package
        return command;
    }


    /**
     *
     *
     * @brief CommandConstructor::setFlowControllerFlowRate
     */
    QVariantMap CommandConstructor::setFlowControllerFlowRate(QString controller, double flowrate)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setFlowRate");

        // Set relative to the flow unit
        command.insert("unit", "250");

        // Set the flow rate
        command.insert("rate", QString::number(flowrate));

        // Return the package
        return command;
    }

    /**
     *
     *
     * @brief CommandConstructor::setFlowControllerSoftStart
     */
    QVariantMap CommandConstructor::setFlowControllerSoftStart(QString controller, int state)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setSoftStart");

        // Enabled / disabled
        if(state == 4)
        {
            command.insert("type", "4");
        }
        else
        {
            command.insert("type", "0");
        }

        // Return the package
        return command;
    }

    /**
     *
     *
     * @brief CommandConstructor::setFlowControllerSoftStartTime
     */
    QVariantMap CommandConstructor::setFlowControllerSoftStartTime(QString controller, int seconds)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setSoftStartTime");

        // Set number of seconds
        command.insert("seconds", QString::number(seconds));

        // Return the package
        return command;
    }


    /**
     *
     *
     * @brief CommandConstructor::setFlowControllerSourceControl
     * @param controller
     * @param source
     */
    QVariantMap CommandConstructor::setFlowControllerSourceControl(QString controller, int source)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "setSourceControll");

        // Set number of seconds
        command.insert("source", QString::number(source));

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getFlowControllerSourceControl(QString controller)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "getSourceControl");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getFlowControllerSoftStartTime(QString controller)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "getSoftStartTime");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getFlowControllerSoftStart(QString controller)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "getSoftStart");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getFlowControllerFlowRate(QString controller)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "getFlowRate");

        // Return the package
        return command;
    }
    QVariantMap CommandConstructor::getFlowControllerTemperature(QString controller)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "getControllerTemperature");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getSetFlowControllerFlowRate(QString controller)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "getSetFlowRate");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getFlowControllerValveOverride(QString controller)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "FlowController");
        command.insert("controller", controller);
        command.insert("method", "getValveOverride");

        // Return the package
        return command;
    }












    /**
     *
     *
     * @brief CommandConstructor::setBackingPump
     * @param onOff
     */
    QVariantMap CommandConstructor::setBackingPump(bool onOff)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setPumpingState");
        command.insert("state", onOff);

        // Return the package
        return command;
    }

    /**
     *
     *
     * @brief CommandConstructor::setTurboPump
     * @param onOff
     */
    QVariantMap CommandConstructor::setTurboPump(bool onOff)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setTurboPumpState");
        command.insert("state", onOff);

        // Return the package
        return command;
    }

    /**
     *
     *
     * @brief CommandConstructor::setBackingPumpMode
     * @param onOff
     */
    QVariantMap CommandConstructor::setBackingPumpMode(int mode)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setBackingPumpMode");
        command.insert("mode", mode);

        // Return the package
        return command;
    }

    /**
     *
     *
     * @brief CommandConstructor::setGasMode
     * @param onOff
     */
    QVariantMap CommandConstructor::setGasMode(int mode)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "setGasMode");
        command.insert("mode", mode);

        // Return the package
        return command;
    }
    QVariantMap CommandConstructor::getGasMode()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getGasMode");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getBackingPumpMode()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getBackingPumpMode");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getTurboPump()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getTurboPumpState");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getBackingPump()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getPumpingState");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getTurboSpeed()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getTurboSpeed");
        command.insert("type", "1");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getTurboBearingTemperature()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getTemperature");
        command.insert("location", "3");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getPumpBottomTemperature()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getTemperature");
        command.insert("location", "1");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getTC110ElectronicsTemperature()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getTemperature");
        command.insert("location", "2");

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::getMotorTemperature()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "VacStation");
        command.insert("method", "getTemperature");
        command.insert("location", "4");

        // Return the package
        return command;
    }











    /**
     *
     *
     * @brief CommandConstructor::requestPressureConfirmation
     */
    QVariantMap CommandConstructor::pressureConfirmation()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "PressureSensor");
        command.insert("method", "confirmInit");
        command.insert("channel", "1");

        // Return the package
        return command;
    }

    /**
     *
     *
     * @brief CommandConstructor::getPressureReading
     */
    QVariantMap CommandConstructor::getPressureReading(int channel)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "PressureSensor");
        command.insert("method", "readPressure");
        command.insert("channel", channel);

        // Return the package
        return command;
    }

    /**
     *
     *
     * @brief CommandConstructor::getPressureSerialNumber
     */
    QVariantMap CommandConstructor::getPressureSerialNumber()
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "PressureSensor");
        command.insert("method", "readSerialNumber");

        // Return the package
        return command;
    }













    QVariantMap CommandConstructor::setLabJackConfig(int FI07, int FI06, int FI05, int FI04, int EIO7, int EIO6, int EIO5, int EIO4, int EIO3, int EIO2, int EIO1, int EIO0)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "LabJack");
        command.insert("method", "configureIO");

        // Config for gas rig
        command.insert("FIO7", FI07);
        command.insert("FIO6", FI06);
        command.insert("FIO5", FI05);
        command.insert("FIO4", FI04);
        command.insert("EIO7", EIO7);
        command.insert("EIO6", EIO6);
        command.insert("EIO5", EIO5);
        command.insert("EIO4", EIO4);
        command.insert("EIO3", EIO3);
        command.insert("EIO2", EIO2);
        command.insert("EIO1", EIO1);
        command.insert("EIO0", EIO0);

        // Return the package
        return command;
    }

    QVariantMap CommandConstructor::setValveState(QString valveName, bool state)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "LabJack");
        command.insert("method", "setDigitalPort");
        command.insert("port", valveName);
        command.insert("value", state);

        // Return the package
        return command;
    }

    /**
     * Get a pressure reading from the relivent HAL
     *
     * @brief CommandWrapper::getVacuumPressure
     */
    QVariantMap CommandConstructor::getVacuumPressure(QString connection, double slope, double offset)
    {
        // Create command for HAL
        QVariantMap command;
        command.insert("hardware", "LabJack");
        command.insert("method", "readAnaloguePort");
        command.insert("port", connection);
        command.insert("slope", slope);
        command.insert("offset", offset);

        // Return the package
        return command;
    }


}}
