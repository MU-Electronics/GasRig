#pragma once

#include <QObject>
#include <QThread>

namespace App { namespace Safety
{
    class Monitor: public QObject
    {
        Q_OBJECT
        public:
            Monitor(QObject *parent = 0);
            ~Monitor();

            void setup(QThread &thread);

        public slots:
            void start();

        private:
            //QThread* m_thread;

            explicit Monitor(const Monitor& rhs) = delete;
            Monitor& operator= (const Monitor& rhs) = delete;
    };
}}


