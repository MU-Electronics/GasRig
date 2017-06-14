import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1

Item {
    id: expTab

    Row
    {
        spacing: 5;
        Button
        {
            id: pressureSensorInit
            text: qsTr("Confirm Pressure Sensor Init")
            onClicked:
            {
                // Check pressure sensor is init
                TestingManager.requestPressureConfirmation();
            }
        }

        Button
        {
            id: pressureSensorReading
            text: qsTr("Pressure Sensor Reading")
            onClicked:
            {
                // Check pressure sensor is init
                TestingManager.requestPressureReading();
            }
        }
    }


}
