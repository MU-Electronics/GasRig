#pragma once

#include <QtSerialPort/QSerialPort>

#include <QTimer>
#include <QByteArray>
#include <QObject>
#include <QString>

namespace App { namespace Services
{
    class SerialController: public QObject
    {
        Q_OBJECT

        public:
            explicit SerialController(QObject *parent = 0);
            ~SerialController();



            bool open(QString com, int braud, int timeout);
            void close();
            void write(const QByteArray &writeData);

            bool CheckSumEightValidation(QString data, QString checkSum);
            QString CalculateCheckSumEight(QString string);
            QString findPortName(quint16 productId, quint16 vendorID);

        private slots:
            void handleRead();
            void handleBytesWritten(qint64 bytes);
            void handleTimeout();
            void handleError(QSerialPort::SerialPortError error);


        private:
            QSerialPort&    m_serialPort;
            QTimer&         m_timer;
            int             m_timeOut;
            qint64          m_bytesWritten;
            QByteArray      m_writeData;
            QByteArray      m_readData;

            void clearVars();


            virtual void proccessReadData(QString readData) = 0;
            virtual bool validate(QString data) = 0;

    };
}}


