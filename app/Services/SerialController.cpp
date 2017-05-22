#include "SerialController.h"

// Include external libs
#include <QString>
#include <QDebug>
#include <QTimer>
#include <QByteArray>
#include <QObject>

// Include serial port class
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>

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
            m_serialPort(*new QSerialPort(parent)),
            m_timer(*new QTimer(parent)),
            m_timeOut(1000),
            m_bytesWritten(0),
            m_busFree(true)
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
        close();
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
        m_serialPort.setBaudRate(QSerialPort::Baud9600); // braud

        // Set the data bit rate
        m_serialPort.setDataBits(QSerialPort::Data8);

        // Set if any flow control is bing used
        m_serialPort.setFlowControl(QSerialPort::NoFlowControl);

        // Set is any parity is being used
        m_serialPort.setParity(QSerialPort::NoParity);

        // Set if any stop bits are required
        m_serialPort.setStopBits(QSerialPort::OneStop);

        // Check port open
        if (!m_serialPort.open(QIODevice::ReadWrite))
        {
            qDebug() << "Failed to open port" << com << "error: " << m_serialPort.errorString();
            return false;
        }

        m_serialPort.setDataTerminalReady(true);




        // State the bus is no longer use
        m_busFree = true;




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
     * Close the serial port connection
     *
     * @brief SerialController::close
     */
    void SerialController::close()
    {
        // Close the port
        m_serialPort.close();

        // Free bus
        m_busFree = false;

        // Reset vars
        clearVars();
    }


    /**
     * PRIVATE: Clear any member variables holding data related to I/O
     *
     * @brief SerialController::clearVars
     */
    void SerialController::clearVars()
    {
        // Reset any vars
        m_bytesWritten = 0;
        m_writeData.clear();
        m_readData.clear();
    }


    /**
     * Determin if the bus is in use, therefore no more commands can be sent
     *
     * @brief SerialController::busInUse
     * @return
     */
    bool SerialController::busFree()
    {
        return m_busFree;
    }


    /**
     * Find the com port identifier by the venor and product IDs
     *
     * @brief SerialController::findPortName
     * @param productId
     * @param vendorID
     * @return QString port name
     */
    QString SerialController::findPortName(quint16 productId, quint16 vendorID)
    {
        // Port name
        QString port;

        //  Loop through all com ports
        foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            // If the port has a product and vendor id
            if(serialPortInfo.hasProductIdentifier() && serialPortInfo.hasVendorIdentifier())
            {
                // If the product and vendor ids match
                if((serialPortInfo.productIdentifier() == productId) && (serialPortInfo.vendorIdentifier() == vendorID))
                {
                    // Set the port name
                    port = serialPortInfo.portName();
                }
            }
        }

        // Return port
        return port;
    }


    /**
     * PRIVATE: Calcuate a check sum-8 for given data
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param QString string The string to create the check sum for
     * @return QString
     */
    QString SerialController::CalculateCheckSumEight(QString string)
    {
        // Find check sum 8
        int sum = 0;
        for (unsigned int i = 0; i < string.size(); i++) {
            if(string[i] == ' '){
                sum += 32;
            }else{
                sum += string[i].toLatin1();
            }
        }
        int modulus_int = sum % 256;

        return QString("%1").arg(modulus_int, 3, 10, QChar('0'));
    }


    /**
     * PRIVATE: Check the check sum provided against the data provided
     * @todo remove the std library for Qt libs completely
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param string package The string with check sum that needs validating
     * @return bool
     */
    bool SerialController::CheckSumEightValidation(QString data, QString checkSum)
    {
        // Calcuate check sum for data
        QString newSum = CalculateCheckSumEight(data);

        // Check sums are the same
        int validate = QString::compare(newSum, checkSum, Qt::CaseInsensitive);

        // If the differance between the two stirng is zero then check sums match
        if (validate == 0)
            return true;

        return false;
    }



    /**
     * When data is read to the read on the serial line this method will take over
     *
     * @brief SerialController::handleRead
     */
    void SerialController::handleRead()
    {
        // Read all the data
        m_readData.append(m_serialPort.readAll());

        if (!m_readData.isEmpty() && validate(m_readData))
        {
            // Stop the timeout timer
            m_timer.stop();

            // State the bus is no longer use
            m_busFree = true;

            // Run child method to proccess the data
            proccessReadData(m_readData);

            // Reset vars
            clearVars();
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
            //qDebug() << "Data successfully sent to port " << m_serialPort.portName();
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

        // Clear the output and input buffer
        m_serialPort.clear();

        // State the bus is no longer use
        m_busFree = true;

        // Reset vars
        clearVars();
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

        // Clear the output and input buffer
        m_serialPort.clear();

        // State the bus is no longer use
        m_busFree = true;

        // Reset vars
        clearVars();
    }


    void SerialController::write(const QByteArray &writeData)
    {
        // State the bus is no longer use
        m_busFree = false;

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