#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QList>
#include <QVariantMap>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

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

            // Function structure for selected functions
            struct t_functionOption
            {
                QString id = "";
                QMap<QString, QVariant> options{};
                QJsonObject toJson() const
                {
                    // Create root object
                    QJsonObject root;

                    // Assigned root to object
                    root["id"] = id;

                    // Create object for options
                    QJsonObject opt;

                    // Loop through all current functon options
                    QMapIterator<QString, QVariant> i(options);
                    while (i.hasNext())
                    {
                        // Continue to next
                        i.next();

                        // Add new value to option object
                        opt[i.key()] = QJsonValue::fromVariant(i.value());
                    }

                    // Assign option objetc to root
                    root["options"] = opt;

                    // Return root json object
                    return root;
               }
            } functionOption;

            Add(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:
            void functionAdded();
            void functionRemoved(int i);
            void functionMoved(int from, int to);

        public slots:

            /**
             * Save current to file
             *
             * @brief save
             */
            bool save(QString name, QString desc);


            /**
             * Is the function list empty?
             *
             * @brief emptyList
             * @return
             */
            bool emptyList()
            {
                return m_functionList.isEmpty();
            }


            /**
             * Destory current data
             *
             * @brief destroy
             */
            void destroyData()
            {
                m_functionList.clear();
                clearStrutOptions();
            }


            /**
             * Move functions in list
             *
             * @brief moveFunction
             * @param from
             * @param to
             */
            void moveFunction(int from, int to)
            {
                m_functionList.swap(from, to);
                emit functionMoved(from, to);
            }


            /**
             * Remove function from list
             *
             * @brief removeFunction
             * @param i
             */
            void removeFunction(int i)
            {
                m_functionList.removeAt(i);
                emit functionRemoved(i);
            }


            /**
             * Count number of functions that have been added
             *
             * @brief countFunctions
             * @return
             */
            int countFunctions() { return m_functionList.count(); }


            /**
             * Return function options
             *
             * @brief option
             * @param i
             * @return
             */
            QVariantMap option(int i) { return m_functionList.at(i).options; }


            /**
             * Return function id
             *
             * @brief id
             * @param i
             * @return
             */
            QString id(int i) { return m_functionList.at(i).id; }


            // State machines
            void addHighPressure(QString pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve);
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
            Settings::Container* m_settings;

            // Hold current setup
            QList<t_functionOption> m_functionList;


            /**
             * Convert function list to json object
             *
             * @brief toJson
             * @param list
             * @return
             */
            QJsonObject toJson(QString name, QString desc, const QList<t_functionOption> & list)
            {
                // Root object
                QJsonObject root;
                root.insert("name", name);
                root.insert("desc", desc);

                // Create array of functions
                QJsonArray array;

                // Loop though functions and append to array in order
                for (auto & func : list)
                    array.append(func.toJson());

                root.insert("functions", array);

                // Return json array
                return root;
            }


            /**
             * Clear the strut of all data
             *
             * @brief clearStrutOptions
             */
            void clearStrutOptions()
            {
                functionOption.id = "";
                functionOption.options.clear();
            }


            /**
             * Append function to list
             *
             * @brief appendFunction
             * @param i
             */
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

