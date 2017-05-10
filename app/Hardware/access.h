#pragma once

#include <QObject>
#include <QThread>

namespace App { namespace Hardware
{
    class Access: public QObject
    {
        Q_OBJECT
        public:
            explicit Access();
            ~Access();

            void setup(QThread &thread);

        public slots:
            void runner();

        private:
            //QThread* m_thread;
    };
}}


