#pragma once

#include <QObject>
#include <QQuickItem>
#include "Manager.h"

namespace App { namespace ViewManager
{
    class ConnectionStatus : public QObject, public Manager
    {
        public:
            ConnectionStatus(QObject *parent, QObject* root);

        signals:

        public slots:

        private:
            QObject* m_root;

            explicit ConnectionStatus(const ConnectionStatus& rhs) = delete;
            ConnectionStatus& operator= (const ConnectionStatus& rhs) = delete;
    };
}}
