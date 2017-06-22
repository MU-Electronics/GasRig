import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQml.Models 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item {
    id: valveTab

    width: parent.width - 10




    /**
      * Safe vacuum station functions
      */
    FluidControls.Card
    {
        id: purgeSystemFunction

        width: parent.width - 10

        height: if(purgeMethodOne.checked){ 400 }else if(purgeMethodTwo.checked){ 300 }else{ 200 }

        padding: 5

        Column
        {
            width: parent.width
            spacing: 20
            AlertBox
            {
                width: purgeSystemFunction.width - 10
                type: "Notice"
                textContent: qsTr("Purging the system can take many forms and parameters. Ensure you setup the rig correctly for the selected options.")
            }
            Column
            {
                ButtonGroup
                {
                    id: purgingMethodGroup
                }

                spacing: 10

                width: purgeSystemFunction.width - 10



                move: Transition {
                    NumberAnimation { properties: "x,y"; duration: 400 }
                }


                RadioButton {
                    checked: false
                    id: purgeMethodOne
                    text: qsTr("1: Pressurise with nitrogren, then exhuast, then vac down and repeat")
                    ButtonGroup.group: purgingMethodGroup
                }

                CheckBox {
                    id:purgeMethodOne_outputValve
                    height: (!purgeMethodOne.checked) ? 0 : 30;
                    checked: false
                    text: qsTr("Should the output valve be open during the purge?")
                    opacity: (!purgeMethodOne.checked) ? 0 : 1;
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 1000
                            easing.type: Easing.InOutQuad
                        }
                    }
                }

                TextField
                {
                    height: (!purgeMethodOne.checked) ? 0 : 30;
                    id:purgeMethodOne_numberCycles
                    placeholderText: qsTr("Number of cycles to run before finishing")
                    validator: IntValidator { bottom:0; top: 2000}
                    inputMethodHints: Qt.ImhDigitsOnly
                    opacity: (!purgeMethodOne.checked) ? 0 : 1;
                    width: parent.width - 110
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 1000
                            easing.type: Easing.InOutQuad
                        }
                    }
                }

                TextField
                {
                    height: (!purgeMethodOne.checked) ? 0 : 30;
                    id:purgeMethodOne_nitrogenPressure
                    placeholderText: qsTr("Maximum nitrogen pressure in bar")
                    validator: IntValidator { bottom:0; top: 90}
                    inputMethodHints: Qt.ImhDigitsOnly
                    opacity: (!purgeMethodOne.checked) ? 0 : 1;
                    width: parent.width - 110
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 1000
                            easing.type: Easing.InOutQuad
                        }
                    }
                }



                RadioButton {
                    id: purgeMethodTwo
                    checked: false
                    text: qsTr("2: Continuous nitrogen flow")
                    ButtonGroup.group:purgingMethodGroup
                }
                AlertBox
                {
                    id: purgeMethodTwo_warning
                    width: purgeSystemFunction.width - 110
                    height: (!purgeMethodTwo.checked) ? 0 : 30;
                    opacity: (!purgeMethodTwo.checked) ? 0 : 1;
                    type: "Danger"
                    textContent: qsTr("Remember to set the pressure on the pressure regulator")
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 1000
                            easing.type: Easing.InOutQuad
                        }
                    }
                }
                TextField
                {
                    id:purgeMethodTwo_time
                    height: (!purgeMethodTwo.checked) ? 0 : 30;
                    placeholderText: qsTr("How long to purge for in minutes")
                    validator: IntValidator { bottom:0; top: 200}
                    inputMethodHints: Qt.ImhDigitsOnly
                    width: parent.width - 110
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    opacity: (!purgeMethodTwo.checked) ? 0 : 1;
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 1000
                            easing.type: Easing.InOutQuad
                        }
                    }
                }
            }
            Column
            {


                Row
                {
                    CheckBox {
                        checked: false
                        text: qsTr("Confirm rig setup")
                    }
                    Button
                    {
                        text: qsTr("Purge System")
                        onClicked:
                        {

                        }
                    }
                }
            }
        }
    }




    /**
      * Set the valve status
      */
    FluidControls.Card
    {
        id: valveStateControl

        anchors.top: purgeSystemFunction.bottom
        anchors.topMargin: 15

        width: parent.width-10

        height: 230

        padding: 5


        Column
        {
            spacing: 0
            width: parent.width
            AlertBox
            {
                id: valveAlert
                width: parent.width
                type: (valveSafeMode.checked) ? "Success" : "Danger";
                textContent: (valveSafeMode.checked) ? qsTr("Protected: Safe mode enabled, which limits the possiable combinations. Be carefully though damage can still be inflicted.") : qsTr("No-Protection: Be carefully there is no automated protection enabled! Enable safe mode which limits the possiable combinations.");
            }

            Row
            {
                CheckBox {
                    id: valveSafeMode
                    checked: true
                    text: qsTr("Enable safe mode to limit valve combinations")
                }
            }

            Item
            {
                id: valveLayoutContainer

                width: parent.width
                height: 90

                FluidLayouts.AutomaticGrid
                {
                    id: valveLayout


                    property var noInColoum: 5
                    property var noInRow: 2

                    width: parent.width

                    cellWidth: parent.width / noInColoum
                    cellHeight: 90

                    anchors.fill: parent

                    model: ListModel {
                        id: valvesList
                        ListElement { number: "1"; name: "Valve 1" }
                        ListElement { number: "2"; name: "Valve 2" }
                        ListElement { number: "3"; name: "Valve 3" }
                        ListElement { number: "4"; name: "Valve 4" }
                        ListElement { number: "5"; name: "Valve 5" }
                        ListElement { number: "6"; name: "Valve 6" }
                        ListElement { number: "7"; name: "Valve 7" }
                        ListElement { number: "8"; name: "Valve 8" }
                        ListElement { number: "9"; name: "Valve 9" }
                    }
                    delegate: Rectangle
                    {
                        width: valveLayout.cellWidth - 10
                        height: valveLayoutContainer.height / 2

                        Button
                        {
                            text: model.name
                            objectName: "valveButton"
                            Material.background: if(SystemStatusManager.valveState[model.number] === 1){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                            Material.foreground: if(SystemStatusManager.valveState[model.number] === 1){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                            onClicked:
                            {
                                // Toggle the state
                                var toggle = !SystemStatusManager.valveState[model.number];

                                // Set vac pump
                                TestingManager.requestValveState(model.number, toggle);
                            }
                        }
                    }
                }
            }
        }
    }
}
