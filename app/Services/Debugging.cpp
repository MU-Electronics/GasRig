#include "Debuging.h"

#include <QDebug>
#include <QLoggingCategory>

// Define logging catergoies
Q_LOGGING_CATEGORY(jsonFileService, "services.json-file")

Q_LOGGING_CATEGORY(labJackService, "services.labjack")

Q_LOGGING_CATEGORY(serialService, "services.serial-bus")

Q_LOGGING_CATEGORY(usbConnectionStatus, "usb.connection.status")

Q_LOGGING_CATEGORY(threadService, "services.thread")
