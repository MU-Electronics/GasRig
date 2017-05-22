#include "Container.h"

// External deps
#include <QDir>
#include <QCoreApplication>

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
        QString pathDir = getDir();

        // Load the safety settings file
        safety.load(pathDir+"/config/Safety.json", safety.SaveFormat::Json);

        // Load the general settings file
        general.load(pathDir+"/config/General.json", general.SaveFormat::Json);

        // Load the view settings file
        view.load(pathDir+"/config/View.json", view.SaveFormat::Json);

        // Load the hardware settings file
        hardware.load(pathDir+"/config/Hardware.json", hardware.SaveFormat::Json);
    }


    /**
     * Get the programs root path
     *
     * @brief Container::getDir
     * @return QString path
     */
    QString Container::getDir()
    {
        // This should work across platform, working windows need to check mac
        QString path = QCoreApplication::applicationDirPath();

        // I know the below works fine for mac
        #ifdef __APPLE__
            #include "TargetConditionals.h"
            #ifdef TARGET_OS_MAC
                // Instance of QDir at current path of program
                QDir pathsRoot(QDir::currentPath());
                pathsRoot.cdUp(); pathsRoot.cdUp(); pathsRoot.cdUp();
                path = pathsRoot.path();
            #endif
        #endif

        // Return the path
        return path;
    }

}}
