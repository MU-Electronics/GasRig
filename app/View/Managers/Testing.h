#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QVariantMap>
#include <QDebug>

#include "Manager.h"
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

// Include command constructor
#include "../../Hardware/CommandConstructor.h"

namespace App { namespace View { namespace Managers
{
    class Testing : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        public:
            Testing(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);


        signals:
            void hardwareRequest(QVariantMap command);
            void emit_testingMaintenanceReply(QString reply);

        public slots:
            // Requests comming from the views
            void requestBackingPump(bool onOff);
            void requestTurboPump(bool onOff);
            void requestBackingPumpMode(int mode);
            void requestGasMode(int mode);
            void requestTurboSpeed();
            void requestTurboBearingTemperature();

            void requestPressureConfirmation();
            void requestPressureReading();
            void requestPressureSerialNumber();

            void requestLabJackConfig();
            void requestValveState(int port, bool state);
            void requestVacuumPressure();

            void requestFlowControllerValveOverride(QString controller, int state);
            void requestFlowControllerFlowRate(QString controller, double flowrate);
            void requestFlowControllerSoftStartTime(QString controller, int seconds);
            void requestFlowControllerSoftStart(QString controller, int state);
            void requestSetFlowControllerSourceControl(QString controller, int source);
            void requestActualFlowControllerFlowRate(QString controller);
            void requestFlowControllerTemperature(QString controller);

            // Request experiment methods
            void requestHighPressure(QString pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve);
            void requestVent(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);
            void requestPurge(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo);
            void requestValveStateSafe(int port, bool state);
            void requestVacDown(double value, int valueType, bool turbo, int gasMode, int mode);
            void requestPulseValve(int valve, int cycles, int timeOpen, int timeClosed);

            // Request experiment methods to be stopped
            void requestVacDownStop();
            void requestPulseValveStop();
            void requestHighPressureStop();
            void requestVentStop();
            void requestPurgeStop();



            // Returned actions
            void receiveVacSetPump(QVariantMap command);
            void receiveVacSetTurbo(QVariantMap command);
            void receiveVacSetGasMode(QVariantMap command);
            void receiveVacSetPumpMode(QVariantMap command);

            void receivePressureSensorInit(QVariantMap command);
            void receivePressureSensorReading(QVariantMap command);
            void receivePressureSensorSerialNumber(QVariantMap command);

            void receiveValveStatus(QVariantMap command);
            void receiveLabJackConfig(QVariantMap command);
            void receiveVacuumReading(QVariantMap command);
            void receiveVacTemperature(QVariantMap command);
            void receiveVacTurboSpeed(QVariantMap command);

            void receiveFlowControllerTemperature(QVariantMap command);
            void receiveFlowControllerFlowRate(QVariantMap command);
            void receiveFlowControllerValveOverride(QVariantMap command);
            void receiveSetFlowControllerFlowRate(QVariantMap command);
            void receiveSetFlowControllerSoftStart(QVariantMap command);
            void receiveSetFlowControllerSoftStartTime(QVariantMap command);
            void receiveSetFlowControllerSourceControl(QVariantMap command);



        private:
            QQmlApplicationEngine* m_root;

            // Holds the settings container
            Settings::Container m_settings;

            // Hold experiment engine
            Experiment::Engine& m_experimentEngine;

            // Hold instance of command constructor
            Hardware::CommandConstructor m_commandConstructor;

            // Requested command which is allowed to show a responce in the command terminal
            QString requestedCommand;


            void setRequestedCommand(QString command)
            {
                requestedCommand = command;
            }

            bool isRequestedCommand(QString command)
            {
                if(requestedCommand == command)
                {
                    // Remove from request
                    requestedCommand = "";

                    // Return true
                    return true;
                }

                // Return false
                return false;
            }

    };
}}}


