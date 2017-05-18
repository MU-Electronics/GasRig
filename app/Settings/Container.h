#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>

// Include setting managers
#include "Safety.h"
#include "General.h"
#include "Hardware.h"
#include "View.h"

namespace App { namespace Settings
{
    class Container
    {

        public:
            Container(QObject *parent = 0);

            App::Settings::Safety& safety;
            App::Settings::General& general;
            App::Settings::View& view;
            App::Settings::Hardware& hardware;

       private:
            void loadSettings();
            QString getDir();
    };
}}

