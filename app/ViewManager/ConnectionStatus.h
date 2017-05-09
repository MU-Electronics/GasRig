#pragma once

#include <QObject>
#include <QQuickItem>

namespace App { namespace ViewManager
{
    class ConnectionStatus : public QObject
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
