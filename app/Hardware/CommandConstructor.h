#pragma once

#include <QObject>

#include <QVariantMap>

namespace App { namespace Hardware
{

    class CommandConstructor
    {

        public:
            CommandConstructor(QObject *parent = 0);

            ~CommandConstructor();

            int generateCommandCount();
            int m_commandCount = 0;

            // Labjack command constructors
            QVariantMap setLabJackConfig(int FI07, int FI06, int FI05, int FI04, int EIO7, int EIO6, int EIO5, int EIO4, int EIO3, int EIO2, int EIO1, int EIO0);
            QVariantMap setValveState(QString valveName, bool state);
            QVariantMap getVacuumPressure(QString connection, double slope, double offset);



            // Pressure sensor command constructors
            QVariantMap pressureConfirmation();
            QVariantMap getPressureReading(int channel);
            QVariantMap getPressureSerialNumber();



            // Vacuum station command constructors
            QVariantMap getGasMode();
            QVariantMap getBackingPumpMode();
            QVariantMap getTurboPump();
            QVariantMap getBackingPump();
            QVariantMap getTurboSpeed();
            QVariantMap getTurboBearingTemperature();
            QVariantMap getPumpBottomTemperature();
            QVariantMap getTC110ElectronicsTemperature();
            QVariantMap getMotorTemperature();

            QVariantMap setGasMode(int mode);
            QVariantMap setBackingPumpMode(int mode);
            QVariantMap setTurboPump(bool onOff);
            QVariantMap setBackingPump(bool onOff);



            // Flow controller command constructors
            QVariantMap getFlowControllerSourceControl(QString controller);
            QVariantMap getFlowControllerSoftStartTime(QString controller);
            QVariantMap getFlowControllerSoftStart(QString controller);
            QVariantMap getFlowControllerFlowRate(QString controller);
            QVariantMap getSetFlowControllerFlowRate(QString controller);
            QVariantMap getFlowControllerValveOverride(QString controller);
            QVariantMap getFlowControllerTemperature(QString controller);

            QVariantMap setFlowControllerSourceControl(QString controller, int source);
            QVariantMap setFlowControllerSoftStartTime(QString controller, int seconds);
            QVariantMap setFlowControllerSoftStart(QString controller, int state);
            QVariantMap setFlowControllerFlowRate(QString controller, double flowrate);
            QVariantMap setFlowControllerValveOverride(QString controller, int state);

    };

}}
