#include "PresenterSupport.h"

#include <QStringList>
#include <QDebug>
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


    /**
     * In some cases a return method should be replaced with another alias
     *
     * @brief PresenterSupport::methodOverride
     * @param commands
     * @param package
     * @return
     */
    QVariantMap PresenterSupport::methodOverride(QVariantMap commands, QVariantMap package)
    {
        QString override = commands.value("emit_override").toString();

        if(override != "")
            package.insert("method", override);

        return package;
    }

}}}}
