#include "AccessSignalsSlots.h"


namespace App { namespace Hardware
{
    AccessSignalsSlots::AccessSignalsSlots(QObject *parent)
    {

    }

    /**
     * Listen for new pressure readings and emits the new data
     *
     * @brief Access::listen_pressureSensorOne
     * @param command
     */
    void AccessSignalsSlots::listen_pressureSensorOne(QVariantMap command)
    {
        qDebug() << "I'll be sending that signal for you";
    }


    /**
     * Listen to all com updates and emit relivent onces
     *
     * @brief Access::listen_serialComUpdates
     * @param command
     */
    void AccessSignalsSlots::listen_serialComUpdates(QVariantMap command)
    {
        emit emit_serialComUpdated(command);
    }


    /**
     * @brief Access::listen_critialSerialError
     * @param command
     */
    void AccessSignalsSlots::listen_critialSerialError(QVariantMap command)
    {
        emit emit_critialSerialError(command);
    }


    /**
     * @brief Access::listen_timeoutSerialError
     * @param command
     */
    void AccessSignalsSlots::listen_timeoutSerialError(QVariantMap command)
    {
        emit emit_timeoutSerialError(command);
    }
}}

Q_DECLARE_INTERFACE(AccessSignalsSlots, "AccessSignalsSlots")
