#include "VacStation.h"

// Include external libs
#include <QDebug>
#include <QString>
#include <QByteArray>


// Include serial controller


namespace App { namespace Hardware { namespace HAL
{

    /**
     * On class construction the below is proccessed:
     *      - Serial controller is init
     *      - id is set
     *
     * @brief VacStation::VacStation
     * @param parent
     * @param id
     */
    VacStation::VacStation(QObject *parent, int id)
        :   SerialController(parent),
            m_id(id)
    {

    }


    /**
     * On destruction of class the below will be ran:
     *
     * @brief VacStation::~VacStation
     */
    VacStation::~VacStation()
    {

    }


    /**
     * This method validate the data before procceeding to proccessReadData
     *
     * @brief VacStation::validate
     * @param package
     * @return
     */
    bool VacStation::validate(QString package)
    {
        // Get check sum
        QString checkSum = package.right(4).left(3);

        // Get data that is for the check sum
        QString data = package.left(package.length()-4);

        // If the differance between the two stirng is zero then check sums match
        if (CheckSumEightValidation(data, checkSum))
            return true;

        return false;
    }


    /**
     * When data has been recived it will be handled here
     * by this time validation has been performed on the data via a check sum
     *
     * @brief VacStation::proccessReadData
     * @param readData
     */
    void VacStation::proccessReadData(QString readData)
    {
        qDebug() << "Read: " << readData;
    }


    /**
     * PUBLIC: Creates a package and send it to the bus, then wait for a responce and validates the responce.
     *
     * @param QString action The action eg request, command etc (See Pfeffier Vacuum Protocol for "RS485" p.25 chap. 7.2 in TC110 OPerating Instructions)
     * @param QString parameterValue The id for the information / action required
     * @param QString  data The control data to be sent
     * @param unsigned int bytesRead The number of bytes for the returned response
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @return string
     */
    bool VacStation::send(QString action, QString parameterValue, QString data, unsigned int bytesRead)
    {
        // Create package to send
        QByteArray package = CreatePackage(action, parameterValue, data);

        // Write the package to the bus
        write(package);

        //debug message
        qDebug() << package;

        // package write
        return true;
    }


    /**
     * PRIVATE: Create package for sending to the TC110 controller, the check sum is generated automatically
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param QString action The action eg request, command etc (See Pfeffier Vacuum Protocol for "RS485" p.25 chap. 7.2 in TC110 OPerating Instructions)
     * @param QString parameterValue The id for the information / action required
     * @param QString data The control data to be sent
     * @return QByteArray package data
     */
    QByteArray VacStation::CreatePackage(QString action, QString parameterValue, QString data)
    {
        // Format data legnth 2 bytes
        int size = data.length();
        QString data_length = QString::number(size);
        if(size < 10){
            data_length = "0" + QString::number(size);
        }

        // Compile string
        QString package = QString::number(m_id) + action + parameterValue + data_length + data;

        // Append the check sum  and carriage return
        package = package + CalculateCheckSumEight(package) + "\r";

        // Return string and Conver to btye array
        return QByteArray::fromStdString(package.toStdString());
    }


    /**
     * PUBLIC: Get temperature of selected location
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int location	Select the area for the temperature reading
     *							1 = Pump Bottom
     *							2 = Electronics
     *							3 = Bearing
     *							4 = Motor
     * @return void
     */
    void VacStation::GetTemperature(int location)
    {
       // Get the correct param type
        QString param = "342";
        switch(location)
        {
            case 1: // Pump Bottom
                param = "330"; break;
            case 2: // Electronics
                param = "326"; break;
            case 3: // Bearing
                param = "342"; break;
            case 4: // Motor
                param = "346"; break;
        }

        // Send request, receive it and check it's valid
        send("00", param, "=?", 20);
    }

}}}
