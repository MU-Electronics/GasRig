# include for external packages
include(vendor/fluid/fluid.pri)

TEMPLATE = app

QT += qml quick quickcontrols2 widgets serialport
CONFIG += c++11


# Copy config files to build dir
win32 {
    PWD_WIN = $${PWD}
    DESTDIR_WIN = $${OUT_PWD}
    PWD_WIN ~= s,/,\\,g
    DESTDIR_WIN ~= s,/,\\,g

    configfiles.commands = $$quote(cmd /c xcopy /S /I /Y $${PWD_WIN}\\resources\\config $${DESTDIR_WIN}\\debug\\config)

    LIBS += "$${PWD_WIN}\\vendor\\labjack\\windowsUD\\LJUDDotNet.dll"
    LIBS += "$${PWD_WIN}\\vendor\\labjack\\windowsUD\\LabJackUD.lib"
}

macx {
    configfiles.commands = rsync -av $$PWD/resources/config/* $$OUT_PWD/config

    LIBS += "$$PWD/vendor/labjack/exodriver/liblabjackusb-2.5.2.dylib"
}

linux {
    configfiles.commands = cp -r $$PWD/resources/config/* $$OUT_PWD/config

    LIBS += "$$PWD/vendor/labjack/exodriver/liblabjackusb-2.5.2.dylib"
}

QMAKE_EXTRA_TARGETS += configfiles
POST_TARGETDEPS += configfiles




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
    app/ViewManager/ConnectionStatus.cpp \
    app/ViewManager/Testing.cpp \
    app/Application.cpp \
    app/ViewManager/SystemStatus.cpp \
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
    app/Hardware/HAL/Presenters/VacStationPresenter.cpp

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
    app/ViewManager/ConnectionStatus.h \
    app/ViewManager/Testing.h \
    app/ViewManager/Manager.h \
    app/Utilities/Properties.h \
    app/Application.h \
    app/ViewManager/SystemStatus.h \
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
    app/Hardware/HAL/Presenters/FlowControllerPresenter.h
