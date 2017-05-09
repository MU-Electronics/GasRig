#pragma once

#include <QObject>
#include <QQuickItem>
#include "Manager.h"

namespace App { namespace ViewManager
{
    class Testing : public QObject, public App::ViewManager::Manager
    {
        public:
            Testing(QObject *parent, QObject* root);

        signals:

        public slots:

        private:
            QObject* m_root;

            explicit Testing(const Testing& rhs) = delete;
            Testing& operator= (const Testing& rhs) = delete;
    };
}}
