#pragma once

#include <QObject>
#include <QQuickItem>

namespace App { namespace ViewManager
{
    class Testing : public QObject
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
