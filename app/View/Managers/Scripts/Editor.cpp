#include "Editor.h"

// Include external libs
#include <QQmlContext>
#include <QMap>
#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QJsonDocument>

namespace App { namespace View { namespace Managers { namespace Scripts
{           

    Editor::Editor(QObject *parent, QQmlApplicationEngine *root, Settings::Container *settings, Experiment::Engine* experimentEngine)
        : QObject(parent),
          m_root(root),
          m_settings(settings)
    {
    }


    /**
     * Connect signals from and to this class and the hardware & safety thread
     *
     * @brief Global::makeConnections
     * @param hardware
     * @param safety
     */
    void Editor::makeConnections(Hardware::Access& hardware, Safety::Monitor& safety)
    {
        // Disable unused compiler warnings
        UNUSED(hardware);
        UNUSED(safety);

        getScripts();

    }


    /**
     * Loads current scripts into local var
     *
     * @brief Editor::getScripts
     */
    void Editor::getScripts()
    {
        // Location of scripts
        QString folder = QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation) + "/GasRig/Scripts";

        // Dir info
        QDir directory(folder);

        // Find scripts
        QStringList scripts = directory.entryList(QStringList() << "*.json", QDir::Files);

        // Clear old content
        m_scripts.clear();

        // For each script
        foreach(QString script, scripts)
        {
            // Open file
            QFile file(folder + "/" + script);
            if (!file.open(QFile::ReadOnly | QFile::Text)) break;

            // Read content to string
            QString raw = "";
            QTextStream stream(&file);
            while (!stream.atEnd()){
                raw += stream.readLine();
            }
            file.close();

            // Decode json
            QJsonDocument json = QJsonDocument::fromJson(raw.toUtf8());

            // Get content required
            QJsonObject object = json.object();

            // Cache info
            m_scripts.append(new ScriptInfoObject(object.value("name").toString(), object.value("desc").toString()));
        }

        // Set the data
        m_root->rootContext()->setContextProperty("ScriptInfoObject", QVariant::fromValue(m_scripts));
    }


}}}}

