#pragma once

// Include external deps
#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QString>
#include <QScopedPointer>

// Include setting managers
#include "Safety.h"
#include "General.h"
#include "Hardware.h"
#include "View.h"

namespace App { namespace Settings
{
    class Container: public QObject
    {

        Q_OBJECT

        public:
            Container(QObject *parent);

            QSharedPointer<General> general()
            {
                QSharedPointer<General> pointer(m_general);
                return pointer;
            }

            QSharedPointer<Safety> safety()
            {
                QSharedPointer<Safety> pointer(m_safety);
                return pointer;
            }

            QSharedPointer<View> view()
            {
                QSharedPointer<View> pointer(m_view);
                return pointer;
            }

            QSharedPointer<Hardware> hardware()
            {
                QSharedPointer<Hardware> pointer(m_hardware);
                return pointer;
            }

       private:
            void loadSettings();
            QString getDir();

            QSharedPointer<Safety> m_safety;
            QSharedPointer<General> m_general;
            QSharedPointer<View> m_view;
            QSharedPointer<Hardware> m_hardware;
    };
}}

