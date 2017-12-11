#pragma once

#include <QObject>
#include <QList>

namespace App { namespace Experiment { namespace Machines { namespace Functions
{

    class FunctionsHelper
    {
        public:
            FunctionsHelper();

            QList<int> getIds();
            QString stringOfIds();
            void registerId(int id);
            bool isRegister(int id);

        private:
            QList<int> m_registeredId;

    };

}}}}

