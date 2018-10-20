#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QDebug>

// View contract
#include "../Manager.h"

// Macro for c++ => QML properties
#include "../../../Utilities/Properties.h"

// Include settings container
#include "../../../Settings/Container.h"

// Include threads
#include "../../../Hardware/Access.h"
#include "../../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../../Experiment/Engine.h"


namespace App { namespace View { namespace Managers { namespace Scripts
{

    class Add : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        public:
            struct t_functionOption
            {
                QString id;
                QMap<QString, QVariant> options;
            } functionOption;

            Add(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:
            void functionAdded();
            void functionRemoved(int i);

        public slots:
            // Methods for controlling script functions and there matching options
            void removeFunction(int i) { m_functionList.removeAt(i); emit functionRemoved(i); }
            int countFunctions() { return m_functionList.count(); }
            QVariantMap option(int i) { return m_functionList.at(i).options; }
            QString id(int i) { return m_functionList.at(i).id; }

            // State machines
            void addHighPressure(QString pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve)
            {
                // Set data
                functionOption.id = "pressurise";
                functionOption.options["pressure"] = pressure;
                functionOption.options["initVacDown"] = initVacDown;
                functionOption.options["stepSize"] = stepSize;
                functionOption.options["inputValve"] = inputValve;
                functionOption.options["openOutputValve"] = openOutputValve;

                // Save
                appendFunction(functionOption);
            }
//            void addVent(bool output, bool vacuumOutput, bool flowCavity, bool nitrogenPipes, bool multiPipes, bool flowOnePipes, bool flowTwoPipes);
//            void addPurge(bool outputValve, int numberCycles, double nitrogenPressure, double vacTo);
//            void addValveStateSafe(int port, bool state);
//            void addVacDown(double value, int valueType, bool turbo, int gasMode, int mode);
//            void addPulseValve(int valve, int cycles, int timeOpen, int timeClosed);
//            void addBackingPump(bool state);

            // Hardware commands
//            void addBackingPump(bool onOff);
//            void addTurboPump(bool onOff);
//            void addBackingPumpMode(int mode);
//            void addGasMode(int mode);
//            void addValveState(int port, bool state);
//            void addFlowControllerFlowRate(QString controller, double flowrate);
//            void addFlowControllerValveOverride(QString controller, int state);
//            void addFlowControllerSoftStartTime(QString controller, int state, int seconds);

            // Functions
//             void addWait(int time);
//             void addConditionalWait(int time);
//             void addConditionalProgression(int time);


        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container m_settings;

            // Hold current setup
            QString m_name;
            QString m_description;
            QList<t_functionOption> m_functionList;

            // Clear structure
            void clearStrutOptions()
            {
                functionOption.id = "";
                functionOption.options.clear();
            }

            // Save data
            void appendFunction(t_functionOption i)
            {
                // Save
                m_functionList.append(i);

                // Clear structure
                clearStrutOptions();

                // Emit change
                emit functionAdded();
            }
    };
}}}}

