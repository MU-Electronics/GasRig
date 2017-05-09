#pragma once

#include <QObject>

namespace App { namespace ViewManager
{
    class View:  public QObject
    {
        public:
            View(QObject* root);

        private:
            QObject* m_root;

            explicit View(const View& rhs) = delete;
            View& operator= (const View& rhs) = delete;
    };
}}


