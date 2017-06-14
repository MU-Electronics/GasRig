import QtQuick 2.5
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.1


import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

Item {
    id: vacuumTab

    // Define vars for interface state
    property bool backingpumpstate: false
    property bool turbopumpstate: false

    width: parent.width-10

    FluidControls.Card
    {
        id: pumpControl

        width: parent.width-10

        height: 110

        padding: 5

        Column
        {
            spacing: 10
            width: parent.width

            Rectangle
            {
                color: Material.color(Material.Red, Material.Shade800)
                border.color: Material.color(Material.Red, Material.Shade300)
                border.width: 1

                height: 30
                width: parent.width

                Text {
                    text: "WARNING: Do not enable the turbo untill the vacuum is lower than X10^-3"
                    color: Material.color(Material.Grey, Material.Shade100)
                    font.pixelSize: 13
                    font.weight: Font.Bold

                    anchors.top: parent.top
                    anchors.topMargin: 7
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                }
            }

            Row
            {
                spacing: 10

                Button
                {
                    id: backingpump
                    objectName: "backingPumpControl"
                    text: "Backing Pump"
                    Material.background: if(vacuumTab.backingpumpstate){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                    Material.foreground: if(vacuumTab.backingpumpstate){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                    onClicked:
                    {
                        // Save state
                        backingpumpstate = !backingpumpstate

                        // Set vac pump
                        TestingManager.requestBackingPump(backingpumpstate);
                    }
                }

                Button
                {
                    id: turboPump
                    objectName: "turboPumpControl"
                    text: "Turbo Pump Enable / Disable"
                    Material.background: if(vacuumTab.turbopumpstate){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                    Material.foreground: if(vacuumTab.turbopumpstate){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                    onClicked:
                    {
                        // Save state
                        turbopumpstate = !turbopumpstate

                        // Set vac pump
                        TestingManager.requestTurboPump(turbopumpstate);
                    }
                }
            }
        }
    }


    FluidControls.Card
    {
        anchors.top: pumpControl.bottom
        anchors.topMargin: 25

        width: parent.width-10

        padding:5

        Row
        {
            spacing: 2

            Column
            {
                RadioButton {
                    checked: true
                    text: qsTr("Continuous")
                }
                RadioButton {
                    text: qsTr("Intermittent")
                }
            }
            Column
            {
                RadioButton {
                    text: qsTr("Delayed")
                }
                RadioButton {
                    text: qsTr("Delayed + Intermittent")
                }
            }
            Button
            {
                id: backingPumpModeMode
                text: "Set Backing Pump Mode"
                anchors.bottom: parent.bottom
                Material.background: Material.color(Material.Blue, Material.Shade500)
                Material.foreground: Material.color(Material.Grey, Material.Shade100)
                onClicked:
                {
                    // Save state
                    turbopumpstate = !turbopumpstate

                    // Set vac pump
                    TestingManager.requestTurboPump(turbopumpstate);
                }
            }
        }
    }
}
