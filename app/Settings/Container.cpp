#include "Container.h"

// External deps
#include <QDir>

// Include the setting managers
#include "Safety.h"
#include "General.h"
#include "Hardware.h"
#include "View.h"

namespace App { namespace Settings
{

    /**
     * Contruct the container performs:
     *      - Creation of all setting managers
     *
     * @brief Container::Container
     * @param parent
     */
    Container::Container(QObject *parent)
           : safety(*new Safety()),
             general(*new General()),
             view(*new View()),
             hardware(*new Hardware())
    {
        // Configure the setting
        loadSettings();
    }


    /**
     * Load the json setting files into the setting objects
     *
     * @brief Container::loadSettings
     */
    void Container::loadSettings()
    {
        // Get the current programs dir
        QString path = path();

        // Load the safety settings file
        safety.load(path+"/config/Safety.json", safety.SaveFormat::Json);

        // Load the general settings file
        general.load(path+"/config/General.json", general.SaveFormat::Json);

        // Load the view settings file
        view.load(path+"/config/View.json", view.SaveFormat::Json);

        // Load the hardware settings file
        hardware.load(path+"/config/Hardware.json", hardware.SaveFormat::Json);
    }


    /**
     * Get the programs root path
     *
     * @brief Container::getDir
     * @return QString path
     */
    QString Container::getDir()
    {
        // Instance of QDir at current path of program
        QDir pathsRoot(QDir::currentPath());

        // CD up to the programs root dir
        pathsRoot.cdUp(); pathsRoot.cdUp(); pathsRoot.cdUp();

        // Return the path
        return pathsRoot.path();
    }

}}
