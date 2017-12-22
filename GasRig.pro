# include for external packages
include(vendor/fluid/fluid.pri)

TEMPLATE = app

QT += qml quick quickcontrols2 widgets serialport charts
CONFIG += c++11

QMAKE_MAC_SDK = macosx10.13


# Copy config files to build dir
win32 {
    PWD_WIN = $${PWD}
    DESTDIR_WIN = $${OUT_PWD}
    PWD_WIN ~= s,/,\\,g
    DESTDIR_WIN ~= s,/,\\,g

    #configfiles.commands = $$quote(cmd /c xcopy /S /I /Y $${PWD_WIN}\\resources\\config $${DESTDIR_WIN}\\debug\\config)
    #configfiles_release.commands = $$quote(cmd /c xcopy /S /I /Y $${PWD_WIN}\\resources\\config $${DESTDIR_WIN}\\release\\config)


    #LIBS += "$${PWD_WIN}\\vendor\\labjack\\windowsUD\\LJUDDotNet.dll"

    # Supplier labjack driver is for windows 7
    LIBS += "$${PWD_WIN}\\vendor\\labjack\\windowsUD\\LabJackUD_64.lib"

}

macx {
    configfiles.commands = rsync -av $$PWD/resources/config/* $$OUT_PWD/config

    # Supplier labjack driver is for macOS Sierra (10.12.6)
    #LIBS += "$$PWD/vendor/labjack/exodriver/liblabjackusb-2.5.2.dylib"

    # Assume the user has labjack driver installed
    LIBS += "/usr/local/lib/liblabjackusb.dylib"
}

linux {
    configfiles.commands = cp -r $$PWD/resources/config/* $$OUT_PWD/config

    # Supplied labjack driver is for Ubuntu 32 bit
    #LIBS += "$$PWD/vendor/labjack/exodriver/liblabjackusb-2.5.2.so"

    # Assume the user has labjack driver installed
    LIBS += "/usr/local/lib/liblabjackusb.so"
}

QMAKE_EXTRA_TARGETS += configfiles
POST_TARGETDEPS += configfiles


# Application is very dynamic in nature to allow for easy expantion and since there is no worry of resource
# As such paramaters are passed to methods that they may not use but are required to keep a tidy format or for reflection
#QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter


SOURCES += \
    main.cpp \
    bootstrap/Startup.cpp \
    app/Hardware/Access.cpp \
    app/Hardware/HAL/PressureSensor.cpp \
    app/Hardware/HAL/VacStation.cpp \
    app/Hardware/HAL/Labjack.cpp \
    app/Hardware/HAL/FlowController.cpp \
    app/Experiment/Engine.cpp \
    app/Experiment/Loader.cpp \
    app/Experiment/Installer.cpp \
    app/Services/FormBuilder.cpp \
    app/Safety/Monitor.cpp \
    app/Services/Logger.cpp \
    app/View/Managers/ConnectionStatus.cpp \
    app/View/Managers/Testing.cpp \
    app/Application.cpp \
    app/View/Managers/SystemStatus.cpp \
    app/Services/Thread.cpp \
    app/Settings/Safety.cpp \
    app/Settings/General.cpp \
    app/Settings/View.cpp \
    app/Services/JsonFile.cpp \
    app/Settings/Container.cpp \
    app/Services/SerialController.cpp \
    app/Hardware/HAL/HALSupport.cpp \
    app/Settings/Hardware.cpp \
    app/Hardware/HAL/Presenters/PressureSensorPresenter.cpp \
    app/Services/LabJackController.cpp \
    app/Hardware/HAL/Presenters/FlowControllerPresenter.cpp \
    app/Hardware/HAL/Presenters/LabJackPresenter.cpp \
    app/Hardware/HAL/Presenters/VacStationPresenter.cpp \
    app/Hardware/HAL/Presenters/PresenterSupport.cpp \
    app/Hardware/CommandConstructor.cpp \
    app/Experiment/Machines/VacDown.cpp \
    app/Experiment/Machines/Functions/MachineStates.cpp \
    app/Experiment/Machines/Functions/CommandValidatorState.cpp \
    app/Experiment/Machines/Machines.cpp \
    app/View/Managers/MachineStatus.cpp \
    app/Experiment/Machines/SafeValve.cpp \
    app/Experiment/Machines/ReadFlowControllerFlow.cpp \
    app/Experiment/Machines/ReadPressure.cpp \
    app/Experiment/Machines/ReadVacuum.cpp \
    app/Experiment/Machines/ReadFlowControllerTemperatures.cpp \
    app/Experiment/Machines/ReadVacStationTemperatures.cpp \
    app/Experiment/Machines/ReadTurboSpeed.cpp \
    app/Experiment/Machines/Functions/Valves.cpp \
    app/Experiment/Machines/Functions/Vacuum.cpp \
    app/Experiment/Machines/Functions/Flow.cpp \
    app/Experiment/Machines/Functions/Pressure.cpp \
    app/Experiment/Machines/PulseValve.cpp \
    app/Experiment/Machines/Pressurise.cpp \
    app/View/Managers/Global.cpp \
    app/Experiment/Machines/Vent.cpp \
    app/Experiment/Machines/Purge.cpp \
    app/View/Managers/Modes/PressuriseCell.cpp \
    app/Experiment/Machines/Functions/TransitionsBuilder.cpp \
    app/Experiment/Machines/Functions/FunctionsHelper.cpp \
    bootstrap/LoggingCategory.cpp \
    app/Services/Debugger.cpp \
    app/View/Managers/Graphs/PressureVsTime.cpp \
    app/View/Managers/Graphs/ValvesVsTime.cpp \
    app/View/Managers/Graphs/VacuumVsTime.cpp

RESOURCES += \
    resources/resources.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    readme.md

HEADERS += \
    bootstrap/Startup.h \
    bootstrap/LoggingCategory.h \
    app/Hardware/Access.h \
    app/Hardware/HAL/PressureSensor.h \
    app/Hardware/HAL/FlowController.h \
    app/Hardware/HAL/Labjack.h \
    app/Hardware/HAL/VacStation.h \
    app/Experiment/Engine.h \
    app/Experiment/Loader.h \
    app/Experiment/Installer.h \
    app/Services/FormBuilder.h \
    app/Safety/Monitor.h \
    app/Services/Logger.h \
    app/View/Managers/ConnectionStatus.h \
    app/View/Managers/Testing.h \
    app/View/Managers/Manager.h \
    app/Utilities/Properties.h \
    app/Application.h \
    app/View/Managers/SystemStatus.h \
    app/Services/Thread.h \
    app/Settings/Safety.h \
    app/Settings/General.h \
    app/Settings/View.h \
    app/Services/JsonFile.h \
    app/Settings/Container.h \
    app/Services/SerialController.h \
    app/Hardware/HAL/HALSupport.h \
    app/Settings/Hardware.h \
    app/Hardware/HAL/Presenters/PressureSensorPresenter.h \
    app/Services/LabJackController.h \
    vendor/labjack/exodriver/labjackusb.h \
    vendor/labjack/windowsUD/LabJackUD.h \
    app/Hardware/HAL/Presenters/LabJackPresenter.h \
    app/Hardware/HAL/Presenters/VacStationPresenter.h \
    app/Hardware/HAL/Presenters/FlowControllerPresenter.h \
    app/Hardware/HAL/Presenters/PresenterSupport.h \
    app/Hardware/CommandConstructor.h \
    app/Experiment/Machines/VacDown.h \
    app/Experiment/Machines/Functions/MachineStates.h \
    app/Experiment/Machines/Functions/CommandValidatorState.h \
    app/Experiment/Machines/Machines.h \
    app/View/Managers/MachineStatus.h \
    app/Experiment/Machines/SafeValve.h \
    app/Experiment/Machines/ReadFlowControllerFlow.h \
    app/Experiment/Machines/ReadPressure.h \
    app/Experiment/Machines/ReadVacuum.h \
    app/Experiment/Machines/ReadFlowControllerTemperatures.h \
    app/Experiment/Machines/ReadVacStationTemperatures.h \
    app/Experiment/Machines/ReadTurboSpeed.h \
    app/Experiment/Machines/Functions/Valves.h \
    app/Experiment/Machines/Functions/Vacuum.h \
    app/Experiment/Machines/Functions/Flow.h \
    app/Experiment/Machines/Functions/Pressure.h \
    app/Experiment/Machines/PulseValve.h \
    app/Experiment/Machines/Pressurise.h \
    app/View/Managers/Global.h \
    app/Experiment/Machines/Vent.h \
    app/Experiment/Machines/Purge.h \
    app/View/Managers/Modes/PressuriseCell.h \
    app/Experiment/Machines/Functions/TransitionsBuilder.h \
    app/Experiment/Machines/Functions/FunctionsHelper.h \
    app/Services/Debugger.h \
    app/View/Managers/Graphs/PressureVsTime.h \
    app/View/Managers/Graphs/ValvesVsTime.h \
    app/View/Managers/Graphs/VacuumVsTime.h
