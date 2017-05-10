#pragma once

#include <QObject>
#include <QThread>

namespace App { namespace Safety
{
    class Monitor: public QObject
    {
        Q_OBJECT
        public:
            explicit Monitor();
            ~Monitor();

            void setup(QThread &thread);

        public slots:
            void runner();

        private:
            //QThread* m_thread;
    };
}}


