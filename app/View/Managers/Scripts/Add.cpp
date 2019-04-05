#include "Add.h"

// Include external libs
#include <QMap>
#include <QStandardPaths>
#include <QFile>
#include <QDir>

namespace App { namespace View { namespace Managers { namespace Scripts
{
    Add::Add(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings, Experiment::Engine *experimentEngine)
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
    bool Add::save(QString name, QString desc)
    {
        // Create json
        auto doc = QJsonDocument(toJson(name, desc, m_functionList));

        // Where to store
        QString folder = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + "/GasRig";

        // Create folder is not exists
        if(!QDir(folder).exists())
        {
            // Parent folder
            QDir().mkdir(folder);

            // Script folder
            folder += "/Scripts";

            // Check + create
            if(!QDir(folder).exists())
                QDir().mkdir(folder);
        }
        else
        {
            // Script folder
            folder += "/Scripts";
        }

        // File location
        QString file = folder + "/" + name + ".json";

        // Check ${name}.json does not exist
        QFileInfo fileInfo(file);
        if (fileInfo.exists() && fileInfo.isFile())
            return false;

        // Create file and write
        QFile jsonFile(file);
        if(jsonFile.open(QFile::WriteOnly))
        {
            if(jsonFile.write(doc.toJson()) != -1)
            {
                return true;
            }
        }

        // Error creating script
        return false;
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

