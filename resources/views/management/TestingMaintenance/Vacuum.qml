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

    Flickable {

        width: parent.width
        height: window.height - 180
        contentWidth: parent.width
        contentHeight: vacuumStationSafeFunction.height + pumpControl.height + backPumpMode.height + gasMode.height + 50;


        /**
         * Safe vacuum station functions
         */
        FluidControls.Card
        {
            id: vacuumStationSafeFunction

            width: parent.width-10

            height: 200

            padding: 5

            Column
            {
                width: parent.width
                spacing: 20
                AlertBox
                {
                    width: parent.width
                    type: "Notice"
                    textContent: qsTr("This container contains safe functions for the vacuum station.")
                }
                Row
                {
                    spacing: 15
                    Button
                    {
                        id: getVacuum
                        text: "Get Vacuum"
                        onClicked:
                        {
                            // Get the vacuum pressure
                            TestingManager.requestVacuumPressure();
                        }
                    }
                    Button
                    {
                        id: getTurboSpeed
                        text: "Get Turbo Speed"
                        onClicked:
                        {
                            // Get the turbo speed
                            TestingManager.requestTurboSpeed();
                        }
                    }
                    Button
                    {
                        id: getBearingTemperatures
                        text: "Get Bearing Temperature"
                        onClicked:
                        {
                            // Set vac pump
                            TestingManager.requestTurboBearingTemperature();
                        }
                    }
                }
                Rectangle {
                    height: 3
                    color: "#ece9e9"
                    width: vacuumStationSafeFunction.width -10
                }
                Row
                {
                    spacing: 20;
                    TextField
                    {
                        id: vacDown_time
                        placeholderText: qsTr("Time in minutes")
                        validator: IntValidator { bottom:0; top: 10000}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: vacuumStationSafeFunction.width - 250
                    }

                    CheckBox {
                        id: vacDown_turbo
                        checked: false
                        text: qsTr("Turbo")
                    }

                    Button
                    {
                        text: qsTr("Vac down")
                        enabled: (vacDown_time.text) ? 1 : 0;
                        onClicked:
                        {
                            TestingManager.requestVacDown(vacDown_time.text, vacDown_turbo.checked);
                        }
                    }
                }
            }
        }

        /**
         * Turn pump and turbo on and off
         */
        FluidControls.Card
        {
            id: pumpControl

            anchors.top: vacuumStationSafeFunction.bottom
            anchors.topMargin: 25

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
                    textContent: qsTr("No-Protection: Do not enable the turbo untill vacuum has reached ~3mBar")
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
                width: parent.width
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
                            // Set vac pump
                            TestingManager.requestBackingPumpMode(0);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.vacuumState["backing_pump_mode"] === 1) ? true : false;
                        text: qsTr("Intermittent")
                        ButtonGroup.group: pumpModeTypeGroup
                        onClicked: {
                            // Set vac pump
                            TestingManager.requestBackingPumpMode(1);
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
                            // Set vac pump
                            TestingManager.requestBackingPumpMode(2);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.vacuumState["backing_pump_mode"] === 3) ? true : false;
                        text: qsTr("Delayed + Intermittent")
                        ButtonGroup.group: pumpModeTypeGroup
                        onClicked: {
                            // Set vac pump
                            TestingManager.requestBackingPumpMode(3);
                        }
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
                            // Set vac pump
                            TestingManager.requestGasMode(0);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.vacuumState["gas_type_mode"] === 1) ? true : false;
                        text: qsTr("Molecular mass less than 39 (E.g Methane)")
                        ButtonGroup.group: gasModesGroup
                        onClicked: {
                            // Set vac pump
                            TestingManager.requestGasMode(1);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.vacuumState["gas_type_mode"] === 2) ? true : false;
                        text: qsTr("Helium Gas")
                        ButtonGroup.group: gasModesGroup
                        onClicked: {
                            // Set vac pump
                            TestingManager.requestGasMode(2);
                        }
                    }
                }

            }
        }
    }
}
