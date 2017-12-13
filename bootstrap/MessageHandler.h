#pragma once

#include <QtGlobal>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QFile>


namespace Bootstrap{

    class MessageHandler
    {

        public:
            //MessageHandler();
            //~MessageHandler();

            void installMessageHandler();
            static void handleMessages(QtMsgType type, const QMessageLogContext &context, const QString &msg);


        private:

            // Singleton for message handler
            static MessageHandler* currentHandler;

            // Stream to log file
            QTextStream *out = nullptr;

            // The message handling method
            void messageOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    };
}


