import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1


import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item
{
    id: vacuumTab

    // Define vars for interface state
    property bool backingpumpstate: false
    property bool turbopumpstate: false
    property int backingPumpModeState: false

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

            AlertBox
            {
                height: 30
                width: parent.width
                type: "Danger"
                textContent: "Do not enable the turbo untill vacuum has reached 1mBar"
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

            ButtonGroup
            {
                id: gasTypes
            }

            Column
            {
                RadioButton {
                    checked: true
                    text: qsTr("Continuous")
                    ButtonGroup.group: gasTypes
                    onClicked: {
                        backingPumpModeState = 0;
                    }
                }
                RadioButton {
                    text: qsTr("Intermittent")
                    ButtonGroup.group: gasTypes
                    onClicked: {
                        backingPumpModeState = 1;
                    }
                }
            }
            Column
            {
                RadioButton {
                    text: qsTr("Delayed")
                    ButtonGroup.group: gasTypes
                    onClicked: {
                        backingPumpModeState = 2;
                    }
                }
                RadioButton {
                    text: qsTr("Delayed + Intermittent")
                    ButtonGroup.group: gasTypes
                    onClicked: {
                        backingPumpModeState = 3;
                    }
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
                   // Set vac pump
                    TestingManager.requestBackingPumpMode(backingPumpModeState);
                }
            }
        }
    }
}
