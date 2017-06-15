import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item {
    id: expTab

     width: parent.width - 10

    /**
     * Set the gas mode
     */
    FluidControls.Card
    {
        id: gasMode

        anchors.topMargin: 25

        width: parent.width-10
        height: 100

        Rectangle
        {
            width: parent.width
            height: 40
            id: titleBar
            color: Material.color(Material.Grey, Material.Shade50)

            FluidControls.TitleLabel
            {
                id: titleBarHeading
                text: "Pressure Sensor Related Function"
                horizontalAlignment: Text.AlignHCenter

                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: 5

                color: Material.color(Material.Grey, Material.Shade700)
            }

            Row
            {

                anchors.top: titleBarHeading.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5
                spacing: 5;
                Button
                {
                    id: pressureSensorInit
                    text: qsTr("Confirm Initialisation")
                    onClicked:
                    {
                        // Check pressure sensor is init
                        TestingManager.requestPressureConfirmation();
                    }
                }

                Button
                {
                    id: pressureSensorReading
                    text: qsTr("Pressure Reading")
                    onClicked:
                    {
                        // Check pressure sensor is init
                        TestingManager.requestPressureReading();
                    }
                }

                Button
                {
                    text: qsTr("Serial Number")
                    onClicked:
                    {
                        // Check pressure sensor is init
                        TestingManager.requestPressureSerialNumber();
                    }
                }
            }
        }

    }





    /**
     * Labjack controls
     */
    FluidControls.Card
    {
        id: labjackConfig

        anchors.top: gasMode.bottom
        anchors.topMargin: 25

        width: parent.width-10
        height: 100

        Rectangle
        {
            width: parent.width
            height: 40
            id: labjackTitleBar
            color: Material.color(Material.Grey, Material.Shade50)

            FluidControls.TitleLabel
            {
                id: labJackTitleBarHeading
                text: "LabJack Controls"
                horizontalAlignment: Text.AlignHCenter

                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: 5

                color: Material.color(Material.Grey, Material.Shade700)
            }

            Row
            {

                anchors.top: labJackTitleBarHeading.bottom
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5
                spacing: 5;
                Button
                {
                    id: labJackConfigButton
                    text: qsTr("Configure Ports")
                    onClicked:
                    {
                        // Check pressure sensor is init
                        TestingManager.requestLabJackConfig();
                    }
                }
            }
        }
    }


}
