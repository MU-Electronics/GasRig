#pragma once

#include <QLoggingCategory>

// Define logging catergoies for view managers
Q_DECLARE_LOGGING_CATEGORY(jsonFileService)


// Define logging catergoies for experiment engine parts



// Define logging catergoies for state machines
Q_DECLARE_LOGGING_CATEGORY(flowStateFunctions)

Q_DECLARE_LOGGING_CATEGORY(pressureStateFunctions)

Q_DECLARE_LOGGING_CATEGORY(vacStationStateFunctions)

Q_DECLARE_LOGGING_CATEGORY(valvesStateFunctions)

Q_DECLARE_LOGGING_CATEGORY(machineStates)

Q_DECLARE_LOGGING_CATEGORY(machineStatesValidator)



// Define logging catergoies for HAL
Q_DECLARE_LOGGING_CATEGORY(halAccess)

Q_DECLARE_LOGGING_CATEGORY(halAccessFlowController)

Q_DECLARE_LOGGING_CATEGORY(halAccessFlowControllerPresenter)

Q_DECLARE_LOGGING_CATEGORY(halAccessLabJack)

Q_DECLARE_LOGGING_CATEGORY(halAccessLabJackPresenter)

Q_DECLARE_LOGGING_CATEGORY(halAccessPressureSensor)

Q_DECLARE_LOGGING_CATEGORY(halAccessPressureSensorPresenter)

Q_DECLARE_LOGGING_CATEGORY(halAccessVacStation)

Q_DECLARE_LOGGING_CATEGORY(halAccessVacStationPresenter)



// Define logging catergoies for services
Q_DECLARE_LOGGING_CATEGORY(labJackService)

Q_DECLARE_LOGGING_CATEGORY(serialService)

Q_DECLARE_LOGGING_CATEGORY(usbConnectionStatus)

Q_DECLARE_LOGGING_CATEGORY(threadService)

