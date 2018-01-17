#include "VacStation.h"

// Include external libs
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QVariantMap>
#include <QThread>


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
        // Sets what this class is responable for; @NOTE: Could be done in base class
        m_responsability = "VacStation";
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
     * Sets the ID for the vac pump
     *
     * @brief VacStation::setId
     * @param id
     */
    void VacStation::setId(int id)
    {
        m_id = id;
    }


    /**
     * PRIVATE: Calcuate a check sum-8 for given data
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param QString string The string to create the check sum for
     * @return QString
     */
    QString VacStation::calculateCheckSum(QStringList stringList)
    {
        // Join all data to one acsii string
        QString string = stringList.join("");

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
     * This method validate the data before procceeding to proccessReadData
     * WARNING: This method may recieve half complete data packages so it
     *          must take that into account when verifing the package
     *
     * @brief VacStation::validate
     * @param package
     * @return
     */
    bool VacStation::validate(QStringList package)
    {
        // Combind all read packages into on string
        QString packageString = package.join("");

        // Get check sum
        QString checkSum = packageString.right(4).left(3);

        // Get data that is for the check sum
        QString data = packageString.left(packageString.length()-4);
        QStringList validate;
        validate.append(data);

        // If the differance between the two stirng is zero then check sums match
        if (checkSumValidation(validate, checkSum))
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
    void VacStation::proccessReadData(QStringList package)
    {
        // Send the data to the handware access manager
        emit emit_vacStationData(m_responsability, m_method, m_command, package);
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
        QByteArray package = createPackage(action, parameterValue, data);

        // Write the package to the bus
        write(package);

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
    QByteArray VacStation::createPackage(QString action, QString parameterValue, QString data)
    {
        // Format data legnth 2 bytes
        int size = data.length();
        QString data_length = QString::number(size);
        if(size < 10){
            data_length = "0" + QString::number(size);
        }

        // Compile string
        QString package = QString::number(m_id) + action + parameterValue + data_length + data;
        QStringList validate;
        validate.append(package);

        // Append the check sum  and carriage return
        package = package + calculateCheckSum(validate) + "\r";

        // Return string and Conver to btye array
        return QByteArray::fromStdString(package.toStdString());
    }


    /**
     * Test whether the connection is working
     *
     * @brief VacStation::testConnection
     */
    void VacStation::testConnection()
    {
        // Set the method
        m_method = "testConnection";

        // Does the com port & connection exist?
        if(!checkDeviceAvaliable(false))
        {
            // Send a critial issue, MOST PROBABLY USB not connected issue
            //emit emit_critialSerialError(errorPackageGenerator(m_connectionValues.value("com").toString(), m_connectionValues.value("com").toString(), "Cant not find the device on this PC, check cable is plugged in."));
            emit emit_comConnectionStatus(comConnectionPackageGenerator(m_connectionValues.value("com").toString(), false));

            // return
            return;
        }

        // Test sending data works, for now just use get gas mode
        getGasMode();
    }


    /**
     * Reset the current connection
     *
     * @brief VacStation::resetConnection
     */
    void VacStation::resetConnection()
    {
        // Set the method
        m_method = "resetConnection";

        // Refresh connection attempt
        if(!checkDeviceAvaliable(true))
        {
            // Send a critial issue, MOST PROBABLY USB not connected issue
            emit emit_comConnectionStatus(comConnectionPackageGenerator(m_connectionValues.value("com").toString(), false));

            return;
        }

        // Test sending data works, for now just use get gas mode
        // Other method will send out relivent success
        getGasMode();

    }


    /**
     * PUBLIC: Get temperature of selected location
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @paramPackage int location	Select the area for the temperature reading
     *							1 = Pump Bottom
     *							2 = Electronics
     *							3 = Bearing
     *							4 = Motor
     * @return void
     */
    void VacStation::getTemperature()
    {
        // Set the method
        m_method = "getTemperature";

        // Get the location of temperture
        int location = m_command.value("location").toInt();

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
     * @paramPackage int type	Select the type of speed reading
     *							1 = Accel-Decel
     *							2 = Actual Rotation Speed
     *							3 = Nominal Speed
     *							4 = Set rotation speed
     * @return double
     */
    void VacStation::getTurboSpeed()
    {
        // Set the method
        m_method = "getTurboSpeed";

        // Set the desired id
        int type = m_command.value("type").toInt();

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
     * @paramPackage int id 1-10 error history list
     */
    void VacStation::getError()
    {
        // Set the method
        m_method = "getError";

        // Set the desired id
        int id = m_command.value("line").toInt();

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
     */
    void VacStation::getGasMode()
    {
        // Set the method
        m_method = "getGasMode";

        // Get the correct param type
        QString param = "027";

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Get backing pump mode
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     */
    void VacStation::getBackingPumpMode()
    {
        // Set the method
        m_method = "getBackingPumpMode";

       // Get the correct param type
        QString param = "025";

        // Send request, receive it and check it's valid
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Get turbo pump state on or off
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @return bool
     */
    void VacStation::getTurboPumpState()
    {
        // Set the method
        m_method = "getTurboPumpState";

        // Get the correct param type
        QString param = "023";

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Is the unit pumping?
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @return bool
     */
    void VacStation::getPumpingState()
    {
        // Set the method
        m_method = "getPumpingState";

       // Get the correct param type
       QString param = "010";

        // Send request
        send("00", param, "=?");
    }


    /**
     * PUBLIC: Set the gas mode
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @return bool
     */
    void VacStation::setGasMode()
    {
        // Set the method
        m_method = "setGasMode";

        // Set the desired mode
        int mode = m_command.value("mode").toInt();

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
     * @paramPackage int mode How heavy is the gas?
     *					0 = Continuous
     *					1 = Intermittent
     *					2 = Delayed
     *					3 = Delayed + Intermittent
     * @return bool
     */
    void VacStation::setBackingPumpMode()
    {
        // Set the method
        m_method = "setBackingPumpMode";

        // Set the desired mode
        int mode = m_command.value("mode").toInt();

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
     * @paramPackage int state Should the turbo pump turn on when station on or just use backing pump?
     *					0 = off
     *					1 = on
     * @return bool
     */
    void VacStation::setTurboPumpState()
    {
        // Set the method
        m_method = "setTurboPumpState";

        // Get the desired state
        int state = m_command.value("state").toInt();

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
     * @paramPackage int state Should the unit be pumping?
     *					0 = off
     *					1 = on
     * @return bool
     */
    void VacStation::setPumpingState()
    {
        // Set the method
        m_method = "setPumpingState";

        // Get the desired state
        int state = m_command.value("state").toInt();

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
