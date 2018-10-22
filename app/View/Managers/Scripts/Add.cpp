#include "Add.h"

// Include external libs
#include <QMap>

namespace App { namespace View { namespace Managers { namespace Scripts
{
    Add::Add(QObject *parent, QQmlApplicationEngine *root, Settings::Container settings, Experiment::Engine& experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {

    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Add::makeConnections
     * @param hardware
     * @param safety
     */
    void Add::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Disable unused compiler warnings
        UNUSED(hardware);
        UNUSED(safety);
    }


    /**
     * Save current config to file
     *
     * @brief Add::save
     */
    void Add::save()
    {
        // Check ${name}.json does not exist
            // Create json file
                // Create json object
                    // Serialise json object
                        // Put serialise json into file
                            // Navigate to list tab
                        // General file error
                    // General file error
                // General file error
            // File creation error
        // Name already exists error
    }


    /**
     * Save info on funtion to list
     *
     * @brief Add::addHighPressure
     * @param pressure
     * @param initVacDown
     * @param stepSize
     * @param inputValve
     * @param openOutputValve
     */
    void Add::addHighPressure(QString pressure, bool initVacDown, int stepSize, bool inputValve, bool openOutputValve)
    {
        // Set data
        functionOption.id = "Pressurise";
        functionOption.options["pressure"] = pressure;
        functionOption.options["initVacDown"] = initVacDown;
        functionOption.options["stepSize"] = stepSize;
        functionOption.options["inputValve"] = inputValve;
        functionOption.options["openOutputValve"] = openOutputValve;

        // Save
        appendFunction(functionOption);
    }


}}}}

