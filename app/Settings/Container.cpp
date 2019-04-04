#include "Container.h"

// External deps
#include <QDir>
#include <QCoreApplication>
#include <QStandardPaths>

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
        : QObject(parent)
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
        m_safety = QSharedPointer<Safety>(new Safety(this, (getDir() + "Safety.json")));

        // Load the general settings file
        m_general = QSharedPointer<General>(new General(this, (getDir() + "General.json")));

        // Load the view settings file
        m_view = QSharedPointer<View>(new View(this, (getDir() + "View.json")));

        // Load the hardware settings file
        m_hardware = QSharedPointer<Hardware>(new Hardware(this, (getDir() + "Hardware.json")));
    }


    /**
     * Get the programs root path
     *
     * @brief Container::getDir
     * @return QString path
     */
    QString Container::getDir()
    {
        /*
         * This was to ensure settings remains inside the application folder.
         * However windows now only lets applications write a certain locations
         * hece the below is being replaced.
         */
        QString path = QCoreApplication::applicationDirPath() + "/config/";

        // I know the below works fine for mac
        #ifdef __APPLE__
            #include "TargetConditionals.h"
            #ifdef TARGET_OS_MAC
                // Instance of QDir at current path of program
                QDir pathsRoot(QDir::currentPath());
                pathsRoot.cdUp(); pathsRoot.cdUp(); pathsRoot.cdUp();
                path = pathsRoot.path() + "/config/";
            #endif
        #endif

        /*
         * // This should work across platform, working windows need to check mac
        QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/vacuum_reservoir_controller/config/";

        qDebug() << path;

        // Check it exists and if not create it
        if(!QDir(path).exists())
            QDir().mkdir(path);
        */

        // Return the path
        return path;
    }

}}
