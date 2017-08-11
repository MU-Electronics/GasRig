import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQml.Models 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../../parts"

/**
  * Safe vacuum station functions
  */
FluidControls.Card
{
    id: purgeSystemFunction
    width: parent.width - 10
    height: purgingColumConatiner.height + 30
    padding: 5

    Column
    {
        width: parent.width
        spacing: 20
        id: purgingColumConatiner
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
                onClicked: {
                    purgeMethod_confirm.checked = false
                }
            }
            Column
            {
                spacing: 10
                opacity: (!purgeMethodOne.checked) ? 0 : 1;
                width: parent.width
                Behavior on opacity {
                    NumberAnimation {
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                }
                CheckBox {
                    id:purgeMethodOne_outputValve
                    height: (!purgeMethodOne.checked) ? 0 : 30;
                    checked: false
                    text: qsTr("Should the output valve be open during the purge?")
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                }

                Row
                {
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    height: (!purgeMethodOne.checked) ? 0 : 50;
                    spacing: 10
                    width: parent.width - 110
                    Text {
                        text: qsTr("Number of cycles to run before finishing: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    TextField
                    {
                        id: purgeMethodOne_numberCycles
                        validator: IntValidator { bottom:0; top: 2000 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - 315
                    }
                }

                Row
                {
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    height: (!purgeMethodOne.checked) ? 0 : 50;
                    spacing: 10
                    width: parent.width - 110
                    Text {
                        text: qsTr("Maximum nitrogen pressure in bar: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    TextField
                    {
                        id:purgeMethodOne_nitrogenPressure
                        validator: IntValidator { bottom:0; top: 90}
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - 270
                    }
                }
            }



            RadioButton {
                id: purgeMethodTwo
                checked: false
                text: qsTr("2: Continuous nitrogen flow")
                ButtonGroup.group:purgingMethodGroup
                onClicked: {
                    purgeMethod_confirm.checked = false
                }
            }
            Column
            {
                spacing: 10
                opacity: (!purgeMethodTwo.checked) ? 0 : 1;
                width: parent.width
                Behavior on opacity {
                    NumberAnimation {
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                }
                AlertBox
                {
                    id: purgeMethodTwo_warningOne
                    width: purgeSystemFunction.width - 110
                    height: (!purgeMethodTwo.checked) ? 0 : 30;
                    type: "Danger"
                    textContent: qsTr("Output valve and exhuast valve will be open!")
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                }
                AlertBox
                {
                    id: purgeMethodTwo_warningTwo
                    width: purgeSystemFunction.width - 110
                    height: (!purgeMethodTwo.checked) ? 0 : 30;
                    type: "Warning"
                    textContent: qsTr("Remember to set the pressure on the pressure regulator")
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                }

                Row
                {
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    height: (!purgeMethodTwo.checked) ? 0 : 50;
                    spacing: 10
                    width: parent.width - 110
                    Text {
                        text: qsTr("How long to purge for in minutes: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    TextField
                    {
                        id:purgeMethodTwo_time
                        validator: IntValidator { bottom:0; top: 200}
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - 270
                    }
                }
                Row
                {
                    anchors.leftMargin: 100
                    anchors.left: parent.left
                    height: (!purgeMethodTwo.checked) ? 0 : 50;
                    spacing: 10
                    width: parent.width - 110
                    Text {
                        text: qsTr("Maximum nitrogen pressure in bar: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    TextField
                    {
                        id:purgeMethodTwo_nitrogenPressure
                        validator: IntValidator { bottom:0; top: 90}
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - 270
                    }
                }
            }
        }
        Column
        {
            Row
            {
                id: purgeSubmittion
                property int methodSelected: {
                    if(purgeMethodTwo.checked && purgeMethodTwo_time.text && purgeMethodTwo_nitrogenPressure.text)
                    {
                        return 2;
                    }
                    if(purgeMethodOne.checked && purgeMethodOne_numberCycles.text && purgeMethodOne_nitrogenPressure.text)
                    {
                        return 1;
                    }

                    return 0;
                }

                height: ( methodSelected > 0 ) ? 50 : 0;
                opacity: ( methodSelected > 0 ) ? 1 : 0;

                Behavior on opacity {
                    NumberAnimation {
                        duration: 1000
                        easing.type: Easing.InOutQuad
                    }
                }

                CheckBox {
                    id: purgeMethod_confirm
                    checked: false
                    text: qsTr("Confirm rig setup")
                }
                Button
                {
                    text: qsTr("Start Purging System")
                    enabled: (purgeMethod_confirm.checked) ? true : false;
                    onClicked:
                    {
                        if(purgeSubmittion.methodSelected == 1)
                        {
                            TestingManager.requestPurgeSystemMethodOne(purgeMethodOne_outputValve.text, purgeMethodOne_numberCycles.text, purgeMethodOne_nitrogenPressure.text);
                        }
                        else if (purgeSubmittion.methodSelected == 2)
                        {
                            TestingManager.requestPurgeSystemMethodTwo(purgeMethodTwo_time.text, purgeMethodTwo_nitrogenPressure.text);
                        }
                    }
                }
            }
        }
    }
}
