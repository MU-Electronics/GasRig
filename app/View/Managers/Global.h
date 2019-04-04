#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QMap>
#include <QVariantMap>

// View contract
#include "Manager.h"

// Macro for c++ => QML properties
#include "../../Utilities/Properties.h"

// Include settings container
#include "../../Settings/Container.h"

// Include threads
#include "../../Hardware/Access.h"
#include "../../Safety/Monitor.h"

// Experiment Engine with state machine methods
#include "../../Experiment/Engine.h"

typedef QMap<QString, bool> MapStringBool;
Q_DECLARE_METATYPE(MapStringBool)


namespace App { namespace View { namespace Managers
{

    class Global : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        AUTO_PROPERTY(int, currentNavigation)
        Q_PROPERTY(QVariantMap rigSettings READ rigSettings NOTIFY emit_rigSettingsChanged)

        public:
            Global(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine &experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

            QVariantMap rigSettings() const { return m_rigSettings; }

        signals:
            void emit_rigSettingsChanged(QVariantMap);

        public slots:

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;

            QVariantMap m_rigSettings;

    };
}}}


