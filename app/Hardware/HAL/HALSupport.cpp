#include "HALSupport.h"

#include <QVariantMap>

namespace App { namespace Hardware { namespace HAL
{
    HALSupport::HALSupport(QObject *parent)
    {

    }

    /**
     * Set the param package to be used with the methods below
     *
     * @brief VacStation::setParams
     * @param command
     * @return
     */
    void HALSupport::setParams(QVariantMap command)
    {
        m_command = command;
    }
}}}
