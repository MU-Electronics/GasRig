#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QMap>
#include <QVector>
#include <QVariantMap>

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

    // @todo move to View/Types
    class ScriptInfoObject : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(QString name READ name NOTIFY nameChanged)
        Q_PROPERTY(QString desc READ desc NOTIFY descChanged)

        public:
            ScriptInfoObject(QString name, QString desc)
            {
                m_name = name;
                m_desc = desc;

                emit nameChanged(m_name);
                emit descChanged(desc);
            }

            QString name() const { return m_name; }
            QString desc() const { return m_desc; }

        signals:
            void nameChanged(QString);
            void descChanged(QString);

        private:
            QString m_name = "";
            QString m_desc = "";
    };

    class Editor : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        public:
            Editor(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine *experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:

        public slots:
            void refresh() { getScripts(); }

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            // Avalible scripts
            QList<QObject*> m_scripts;

            // Get avaliable scripts
            void getScripts();

    };
}}}}


