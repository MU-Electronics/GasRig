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

            // Find port name for device
            QString findPortName(quint16 productId, quint16 vendorID);

            // Bus communications
            bool open(QString com, int braud, int timeout);
            void close();
            void write(const QByteArray &writeData);
            bool isOpen();

            // Validation helpers
            bool CheckSumEightValidation(QString data, QString checkSum);
            QString CalculateCheckSumEight(QString string);

            // Check if bus in use
            bool busFree();

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
            bool            m_busFree;

            void clearVars();

            // How the child should handle the read data
            virtual void proccessReadData(QString readData) = 0;

            // Validate the data to determin when the full package has been read
            virtual bool validate(QString data) = 0;

    };
}}


