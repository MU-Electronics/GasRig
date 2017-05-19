#include "VacStation.h"

// Include external libs
#include <QDebug>

// Include serial controller


namespace App { namespace Hardware { namespace HAL
{
    VacStation::VacStation(QObject *parent, int id)
        :   SerialController(parent),
            m_id(id)
    {
        open("com1", 9600, 1000);
    }

    VacStation::~VacStation()
    {

    }


    /**
     * PRIVATE: Create package for sending to the TC110 controller, the check sum is generated automatically
     *
     * @author Sam Mottley <sam.mottley@manchester.ac.uk>
     * @param QString action The action eg request, command etc (See Pfeffier Vacuum Protocol for "RS485" p.25 chap. 7.2 in TC110 OPerating Instructions)
     * @param QString parameterValue The id for the information / action required
     * @param QString data The control data to be sent
     * @return QString package data
     */
    QString VacStation::CreatePackage(QString action, QString parameterValue, QString data)
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

        // Return string
        return package;
    }


    void VacStation::proccessReadData(QString readData)
    {
        qDebug() << readData;
    }

}}}
