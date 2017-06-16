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

    width: parent.width-10

    // Hold the select value for backing pump mode
    property int backingPumpModeState: SystemStatusManager.vacuumState["backing_pump_mode"]

    // Hold the select value for gas type mode
    property int gasTypeModeState: SystemStatusManager.vacuumState["gas_type_mode"]

    /**
     * Set the backing pump mode
     */
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
                    Material.background: if(SystemStatusManager.vacuumState["backing_pump"]){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                    Material.foreground: if(SystemStatusManager.vacuumState["backing_pump"]){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                    onClicked:
                    {
                        // Toggle the state
                        var toggle = !SystemStatusManager.vacuumState["backing_pump"];

                        // Set vac pump
                        TestingManager.requestBackingPump(toggle);
                    }
                }

                Button
                {
                    id: turboPump
                    objectName: "turboPumpControl"
                    text: "Turbo Pump Enable / Disable"
                    Material.background: if(SystemStatusManager.vacuumState["turbo_pump"]){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                    Material.foreground: if(SystemStatusManager.vacuumState["turbo_pump"]){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                    onClicked:
                    {
                        // Save state
                        var toggle = !SystemStatusManager.vacuumState["turbo_pump"];

                        // Set vac pump
                        TestingManager.requestTurboPump(toggle);
                    }
                }
            }
        }
    }



    /**
     * Set the backing pump mode
     */
    FluidControls.Card
    {
        id: backPumpMode

        anchors.top: pumpControl.bottom
        anchors.topMargin: 25

        width: parent.width-10

        padding:5

        Row
        {
            spacing: 2

            ButtonGroup
            {
                id: pumpModeTypeGroup
            }

            Column
            {
                RadioButton {
                    checked: (SystemStatusManager.vacuumState["backing_pump_mode"] === 0) ? true : false;
                    text: qsTr("Continuous")
                    ButtonGroup.group: pumpModeTypeGroup
                    onClicked: {
                        backingPumpModeState = 0;
                    }
                }
                RadioButton {
                    checked: (SystemStatusManager.vacuumState["backing_pump_mode"] === 1) ? true : false;
                    text: qsTr("Intermittent")
                    ButtonGroup.group: pumpModeTypeGroup
                    onClicked: {
                        backingPumpModeState = 1;
                    }
                }
            }
            Column
            {
                RadioButton {
                    checked: (SystemStatusManager.vacuumState["backing_pump_mode"] === 2) ? true : false;
                    text: qsTr("Delayed")
                    ButtonGroup.group: pumpModeTypeGroup
                    onClicked: {
                        backingPumpModeState = 2;
                    }
                }
                RadioButton {
                    checked: (SystemStatusManager.vacuumState["backing_pump_mode"] === 3) ? true : false;
                    text: qsTr("Delayed + Intermittent")
                    ButtonGroup.group: pumpModeTypeGroup
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




    /**
     * Set the gas mode
     */
    FluidControls.Card
    {
        id: gasMode

        anchors.top: backPumpMode.bottom
        anchors.topMargin: 25

        width: parent.width-10

        padding:5

        Row
        {
            spacing: 2

            ButtonGroup
            {
                id: gasModesGroup
            }

            Column
            {
                RadioButton {
                    checked: (SystemStatusManager.vacuumState["gas_type_mode"] === 0) ? true : false;
                    text: qsTr("Molecular mass great than or equal to 39 (E.g Argon)")
                    ButtonGroup.group: gasModesGroup
                    onClicked: {
                        gasTypeModeState = 0;
                    }
                }
                RadioButton {
                    checked: (SystemStatusManager.vacuumState["gas_type_mode"] === 1) ? true : false;
                    text: qsTr("Molecular mass less than 39 (E.g Methane)")
                    ButtonGroup.group: gasModesGroup
                    onClicked: {
                        gasTypeModeState = 1;
                    }
                }
                RadioButton {
                    checked: (SystemStatusManager.vacuumState["gas_type_mode"] === 2) ? true : false;
                    text: qsTr("Helium Gas")
                    ButtonGroup.group: gasModesGroup
                    onClicked: {
                        gasTypeModeState = 2;
                    }
                }
                Button
                {
                    text: "Set Gas Type"
                    Material.background: Material.color(Material.Blue, Material.Shade500)
                    Material.foreground: Material.color(Material.Grey, Material.Shade100)
                    onClicked:
                    {
                       // Set vac pump
                       TestingManager.requestGasMode(gasTypeModeState);
                    }
                }
            }

        }
    }





}
