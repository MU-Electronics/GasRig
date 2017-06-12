#include "PressureSensorPresenter.h"

#include <QStringList>
#include <QVariantMap>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    PressureSensorPresenter::PressureSensorPresenter(QObject *parent)
    {

    }

    QVariantMap PressureSensorPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        QVariantMap presented;
        presented["method"] = "listen_pressureSensorOne";
        presented["value"] = (float) 1.2025;

        return presented;
    }

}}}}
