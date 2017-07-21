#include "PresenterSupport.h"

#include <QStringList>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    PresenterSupport::PresenterSupport()
    {

    }

    bool PresenterSupport::validatePackageSize(QStringList package, int size)
    {
        if(!package.empty() && package.size() == size)
            return true;

        return false;
    }


}}}}
