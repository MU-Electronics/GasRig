#include "FlowControllerPresenter.h"

#include <QStringList>
#include <QVariantMap>
#include <QDebug>

namespace App { namespace Hardware { namespace HAL { namespace Presenters
{

    FlowControllerPresenter::FlowControllerPresenter(QObject *parent)
    {

    }


    /**
     * Determin which presenter should be ran
     * NOTE: All packages are validated before this point
     *
     * @brief PressureSensorPresenter::proccess
     * @param method
     * @param commands
     * @param package
     * @return
     */
    QVariantMap FlowControllerPresenter::proccess(QString method, QVariantMap commands, QStringList package)
    {
        QVariantMap temp;
        temp["method"] = "emit_pressureSensorInit";
        return temp;
    }




}}}}
