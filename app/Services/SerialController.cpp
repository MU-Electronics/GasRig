#include "SerialController.h"

#include <QString>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QObject>

#include <QtSerialPort/QSerialPort>

namespace App { namespace Services
{

    /**
     * This construct the serial controller class by performing:
     *      - Create instant of QSerialPort
     *
     * @brief SerialController::SerialController
     * @param parent
     */
    SerialController::SerialController(QObject *parent)
        :   QObject(parent),
            m_serialPort(*new QSerialPort),
            m_timeOut(1000),
            m_bytesWritten(0)
    {

    }


    /**
     * On class destruction performs:
     *      - Closes the serial port
     *
     * @brief SerialController::~SerialController
     */
    SerialController::~SerialController()
    {
        m_serialPort.close();
    }


    /**
     * Configures the serial port class and connects signals related the the serial port the the serial controller
     *
     * @brief SerialController::open
     * @param com
     * @param braud
     * @return
     */
    bool SerialController::open(QString com, int braud, int timeout)
    {
        // Set the port name
        m_serialPort.setPortName(com);

        // Set the braud rate
        m_serialPort.setBaudRate(braud);

        // Set the data bit rate
        m_serialPort.setDataBits(QSerialPort::Data8);

        // Set if any flow control is bing used
        m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

        // Set is any parity is being used
        m_serialPort.setParity(QSerialPort::NoParity);

        // Set if any stop bits are required
        m_serialPort.setStopBits(QSerialPort::OneStop);

        // Check port open
        if (!m_serialPort.open(QIODevice::ReadOnly))
        {
            qDebug() << "Failed to open port" << com << "error: " << m_serialPort.errorString();
            return false;
        }



        // Set the timer to be single shot only
        m_timer.setSingleShot(true);

        // Set correct time out
        m_timeOut = timeout;



        // When serial port is ready to read connect it to the read handler
        connect(&m_serialPort, &QSerialPort::readyRead, this, &SerialController::handleRead);

        // When a btye has been written connect the btye written handler
        connect(&m_serialPort, &QSerialPort::bytesWritten, this, &SerialController::handleBytesWritten);

        // When serial port has an error connect it to the error handler
        connect(&m_serialPort, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &SerialController::handleError);

        // When the timer has timeout connect it to the timeout handler
        connect(&m_timer, &QTimer::timeout, this, &SerialController::handleTimeout);

        // Port configured correctly
        return true;
    }


    /**
     * When data is read to the read on the serial line this method will take over
     *
     * @brief SerialController::handleRead
     */
    void SerialController::handleRead()
    {
        // Read all the data
        // Possable problem with this not guarantee reading all the data something to test!!
        m_readData = m_serialPort.readAll();

        if (m_readData.isEmpty())
        {
            // Run child method to proccess the data
            proccessReadData(m_readData);
        }

    }


    /**
     * When bytes have been written to the serail line this method runs
     *
     * @brief SerialController::handleBytesWritten
     * @param bytes
     */
    void SerialController::handleBytesWritten(qint64 bytes)
    {
        // Append the written btyes to the buffer
        m_bytesWritten += bytes;

        // If the written btye buffer size is the same size as the package being wrote then were finished
        if (m_bytesWritten == m_writeData.size())
        {
            // Reset the bytes written buffer
            m_bytesWritten = 0;

            // Print message
            qDebug() << "Data successfully sent to port " << m_serialPort.portName();
        }
    }


    /**
     * When the timeout has exceed this method runs
     *
     * @brief SerialController::handleTimeout
     */
    void SerialController::handleTimeout()
    {
        // General error message for any timeout event
        qDebug() << "Operation timed out for port " << m_serialPort.portName() << "; error: " << m_serialPort.errorString();
    }


    /**
     * When the serial port experiances an error this method proccess it
     *
     * @brief SerialController::handleError
     * @param serialPortError
     */
    void SerialController::handleError(QSerialPort::SerialPortError serialPortError)
    {
        // Handle read errors
        if (serialPortError == QSerialPort::ReadError)
        {
            qDebug() << "An I/O error occurred while reading the data from port " << m_serialPort.portName() << "; error: " << m_serialPort.errorString();
        }

        // Handle write errors
        if (serialPortError == QSerialPort::WriteError) {
            qDebug() << "An I/O error occurred while writing the data to port " << m_serialPort.portName() << "; error: " << m_serialPort.errorString();
        }
    }


    void SerialController::write(const QByteArray &writeData)
    {
        // Cache the data to be written
        m_writeData = writeData;

        // Tell the serial port to write the data
        qint64 bytesWritten = m_serialPort.write(writeData);

        // Check the number of btyes written
        if (bytesWritten == -1)
        {
            // If not bytes were written then error
            qDebug() << "Failed to write the data to port " << m_serialPort.portName() << "; error: " << m_serialPort.errorString();
        }
        else if (bytesWritten != m_writeData.size())
        {
            // If not enough btyes were written then error
            qDebug() << "Failed to write all the data to port " << m_serialPort.portName() << "; error: " << m_serialPort.errorString();
        }

        // Set timeout
        m_timer.start(m_timeOut);
    }
}}
