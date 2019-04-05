#pragma once

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QString>
#include <QMap>
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

namespace App { namespace View { namespace Managers { namespace Modes
{

    class PressuriseCell : public QObject, public App::View::Managers::Manager
    {
        Q_OBJECT

        AUTO_PROPERTY(int, currentNavigation)

        public:
            PressuriseCell(QObject *parent, QQmlApplicationEngine *root, Settings::Container* settings, Experiment::Engine *experimentEngine);

            // Make connections with outside world
            void makeConnections(Hardware::Access& hardware, Safety::Monitor &safety);

        signals:

        public slots:

        private:
            QQmlApplicationEngine* m_root;

            // Holds the application settings
            Settings::Container* m_settings;


    };
}}}}


