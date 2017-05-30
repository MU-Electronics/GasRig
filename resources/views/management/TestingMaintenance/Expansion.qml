import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: expTab

    Button
    {
        id: pressureSensorInit
        text: "Confirm Pressure Sensor Init"
        onClicked:
        {
            // Check pressure sensor is init
            TestingManager.requestPressureConfirmation();
        }
    }
}
