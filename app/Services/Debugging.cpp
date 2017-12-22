#include "Debuging.h"

#include <QDebug>
#include <QLoggingCategory>

// Define logging catergoies for view managers
Q_LOGGING_CATEGORY(usbConnectionStatus, "usb.connection.status")



// Define logging catergoies for experiment engine parts



// Define logging catergoies for state machines




// Define logging catergoies for HAL
Q_LOGGING_CATEGORY(halAccess, "hal.access")

Q_LOGGING_CATEGORY(halAccessFlowController, "hal.access.flow-controller")

Q_LOGGING_CATEGORY(halAccessFlowControllerPresenter, "hal.access.flow-controller.presenter")

Q_LOGGING_CATEGORY(halAccessLabJack, "hal.access.lab-jack")

Q_LOGGING_CATEGORY(halAccessLabJackPresenter, "hal.access.lab-jack.presenter")

Q_LOGGING_CATEGORY(halAccessPressureSensor, "hal.access.pressure-sensor")

Q_LOGGING_CATEGORY(halAccessPressureSensorPresenter, "hal.access.pressure-sensor.presenter")

Q_LOGGING_CATEGORY(halAccessVacStation, "hal.access.vac-station")

Q_LOGGING_CATEGORY(halAccessVacStationPresenter, "hal.access.vac-station.presenter")



// Define logging catergoies for services
Q_LOGGING_CATEGORY(jsonFileService, "services.json-file")

Q_LOGGING_CATEGORY(labJackService, "services.labjack")

Q_LOGGING_CATEGORY(serialService, "services.serial-bus")

Q_LOGGING_CATEGORY(threadService, "services.thread")
