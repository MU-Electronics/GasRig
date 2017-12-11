#include "FunctionsHelper.h"

#include <QDebug>
#include <QList>

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    FunctionsHelper::FunctionsHelper()
    {

    }


    QList<int> FunctionsHelper::getIds()
    {
        return m_registeredId;
    }


    QString FunctionsHelper::stringOfIds()
    {
        // String to return
        QString string;

        // Loop through list
        for (int i = 0; i < m_registeredId.size(); ++i) {
            // Append the id
            string += QString::number(m_registeredId.at(i));

            // If not last value append a commor
            if(i < m_registeredId.size()-1)
                string += "," ;
        }

        // Return the commor seperated string
        return string;
    }


    void FunctionsHelper::registerId(int id)
    {
        m_registeredId.append(id);
    }

    bool FunctionsHelper::isRegister(int id)
    {
        // Loop through list
        for (int i = 0; i < m_registeredId.size(); ++i) {
            // If value exists that is the same as the if
            if (m_registeredId.at(i) == id)
            {
                // Remove value
                m_registeredId.removeAt(i);

                // Returned was registered
                return true;
            }
        }

        return false;
    }


}}}}
