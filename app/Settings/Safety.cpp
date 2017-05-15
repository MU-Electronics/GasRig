#include "Safety.h"

#include <QObject>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

namespace App { namespace Settings
{

    /**
     * Init class
     *
     * @brief Safety::Safety
     * @param parent
     */
    Safety::Safety(QObject *parent)
    {

    }



    /**
     * Delete Class
     *
     * @brief Safety::~Safety
     */
    Safety::~Safety()
    {

    }


    /**
     * Format the QJsonObject into a useable member varaible
     *
     * @brief read
     * @param json
     */
    void Safety::read(const QJsonObject &json)
    {

    }


    /**
     * Format a QJsonObject to write to a json file
     *
     * @brief write
     * @param json
     */
    void Safety::write(QJsonObject &json) const
    {

    }

}}
