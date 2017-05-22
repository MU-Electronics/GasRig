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
     * WARNING: This method may recieve half complete data packages so it
     *          must take that into account when verifing the package
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
        // Get the ID
        QString id = readData.mid(0, 3);

        // Get the parameter number
        QString param = readData.mid(5, 3);

        // Find how long the data part is
        QString dataLength = readData.mid(8, 2);

        // Get the data
        QString data = readData.mid(10, dataLength.toInt());

        // Emit signal to application
        emit vacStationData(id, param, data);

        qDebug() << "Read: " << readData << "ID: " << id << " Param: " << param << " With Lenght: " << dataLength << "Result: " << data;
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
    bool VacStation::send(QString action, QString parameterValue, QString data)
    {
        // Create package to send
        QByteArray package = CreatePackage(action, parameterValue, data);

        // Write the package to the bus
        write(package);

        // Debug message
        //qDebug() << "Sent: " << package;

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

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Get trubo speed
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int type	Select the type of speed reading
     *							1 = Accel-Decel
     *							2 = Actual Rotation Speed
     *							3 = Nominal Speed
     *							4 = Motor
     * @return double
     */
    void VacStation::GetTurboSpeed(int type)
    {
        // Get the correct param type
        QString param = "398";
        switch(type)
        {
            case 1: // Actual Rotation Speed could us 309 for Hz
                param = "398"; break;
            case 2: // Accel-Decel
                param = "336"; break;
            case 3: // Nominal Speed
                param = "399"; break;
            case 4: // Set rotation speed
                param = "397"; break;
        }

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Get error message histroy
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int id 1-10 error history list
     * @return double
     */
    void VacStation::GetError(int id)
    {
        // Get the correct param type
        QString param = "360";
        switch(id)
        {
            case 1:
                param = "360"; break;
            case 2:
                param = "361"; break;
            case 3:
                param = "362"; break;
            case 4:
                param = "364"; break;
            case 5:
                param = "364"; break;
            case 6:
                param = "365"; break;
            case 7:
                param = "366"; break;
            case 8:
                param = "367"; break;
            case 9:
                param = "368"; break;
            case 10:
                param = "369"; break;
        }

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Get the gas mode
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @return int How heavy is the gas?
     *				0 = Heavy (39 < X < 80)
     *				1 = Light(=<39)
     *				2 = Helium
     */
    void VacStation::GetGasMode()
    {
        // Get the correct param type
        QString param = "027";

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Get backing pump mode
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @return int How heavy is the gas?
     *				0 = Continuous
     *				1 = Intermittent
     *				2 = Delayed
     *				3 = Delayed + Intermittent
     */
    void VacStation::GetBackingPumpMode()
    {
       // Get the correct param type
        QString param = "025";

        // Send request, receive it and check it's valid
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Get turbo pump state on or off
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int state Should the turbo pump turn on when station on or just use backing pump?
     *					0 = off
     *					1 = on
     * @return bool
     */
    void VacStation::GetTurboPumpState()
    {
        // Get the correct param type
        QString param = "023";

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Is the unit pumping?
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int state Is the unit pumping?
     *					0 = off
     *					1 = on
     * @return bool
     */
    void VacStation::GetPumpingState()
    {
       // Get the correct param type
       QString param = "010";

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Set the gas mode
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int mode	How heavy is the gas?
     *						0 = Heavy (39 < X < 80)
     *                      1 = Light(=<39)
     *						2 = Helium
     * @return bool
     */
    void VacStation::SetGasMode(int mode)
    {
        // Get the correct param type
        QString param = "027";

        // Get the mode to change to
        QString modeChange;
        if(mode == 0){
            modeChange = "000";
        }else if(mode == 1){
            modeChange = "001";
        }else if(mode == 2){
            modeChange = "002";
        }

        // Send request
        send("10", param, modeChange);
    }


    /**
     * PUBLIC: Set backing pump mode
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int mode How heavy is the gas?
     *					0 = Continuous
     *					1 = Intermittent
     *					2 = Delayed
     *					3 = Delayed + Intermittent
     * @return bool
     */
    void VacStation::SetBackingPumpMode(int mode)
    {
       // Get the correct param type
        QString param = "025";

        // Get the mode to change to
        QString modeChange;
        if(mode == 0){
            modeChange = "000";
        }else if(mode == 1){
            modeChange = "001";
        }else if(mode == 2){
            modeChange = "002";
        }else if(mode == 3){
            modeChange = "003";
        }

        // Send request
        send("10", param, modeChange);
    }


    /**
     * PUBLIC: Set turbo pump on or off
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int state Should the turbo pump turn on when station on or just use backing pump?
     *					0 = off
     *					1 = on
     * @return bool
     */
    void VacStation::SetTurboPumpState(int state)
    {
        // Get the correct param type
        QString param = "023";

        // Get the mode to change to
        QString modeChange;
        if(state == 0){
            modeChange = "000";
        }else if(state == 1){
            modeChange = "111";
        }

        // Send request
        send("10", param, modeChange);
    }


    /**
     * PUBLIC: Set the unit pumping or off?
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param int state Should the unit be pumping?
     *					0 = off
     *					1 = on
     * @return bool
     */
    void VacStation::SetPumpingState(int state)
    {
       // Get the correct param type
        QString param = "010";

        // Get the mode to change to
        QString modeChange;
        if(state == 0){
            modeChange = "000";
        }else if(state == 1){
            modeChange = "111";
        }

        // Send request
        send("10", param, modeChange);
    }

}}}
