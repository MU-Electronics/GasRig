#pragma once

#include <QObject>

namespace App { namespace Hardware
{
    class AccessSignalsSlots
    {
        public:
            AccessSignalsSlots(QObject *parent);

        signals:
            void emit_serialComUpdated(QVariantMap command);
            void emit_timeoutSerialError(QVariantMap command);
            void emit_critialSerialError(QVariantMap command);

        public slots:
            void listen_serialComUpdates(QVariantMap command);
            void listen_critialSerialError(QVariantMap command);
            void listen_timeoutSerialError(QVariantMap command);

            void listen_pressureSensorOne(QVariantMap command);
    };
}}

