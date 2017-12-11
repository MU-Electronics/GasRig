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

    property var elementHight: 50
    property var labelWidth: 300

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

            enabled: (MachineStatusManager.controllable["purgeMachine"] === 0) ? 0 : 1 ;


            move: Transition {
                NumberAnimation { properties: "x,y"; duration: 400 }
            }

            Column
            {
                spacing: 10
                width: parent.width



                Row
                {
                    spacing: 10
                    width: parent.width
                    height: purgeSystemFunction.elementHight
                    enabled: (MachineStatusManager.purgeMachine["status"] === true) ? 0 : 1;
                    Text {
                        text: qsTr("Open output valve during purge: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                        width: purgeSystemFunction.labelWidth
                    }
                    CheckBox {
                        id:purgeMethodOne_outputValve
                        checked: false
                    }
                }

                Row
                {
                    spacing: 10
                    width: parent.width
                    height: purgeSystemFunction.elementHight
                    enabled: (MachineStatusManager.purgeMachine["status"] === true) ? 0 : 1;
                    Text {
                        text: qsTr("Number of cycles to run before finishing: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                        width: purgeSystemFunction.labelWidth
                    }
                    TextField
                    {
                        id: purgeMethodOne_numberCycles
                        validator: IntValidator { bottom:0; top: 2000 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - purgeSystemFunction.labelWidth - 20
                    }
                }

                Row
                {
                    spacing: 10
                    width: parent.width
                    height: purgeSystemFunction.elementHight
                    enabled: (MachineStatusManager.purgeMachine["status"] === true) ? 0 : 1;
                    Text {
                        text: qsTr("Maximum nitrogen pressure in bar: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                        width: purgeSystemFunction.labelWidth
                    }
                    TextField
                    {
                        id:purgeMethodOne_nitrogenPressure
                        validator: IntValidator { bottom:200; top: 10000}
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - purgeSystemFunction.labelWidth - 20
                        text: "2000"
                    }
                }

                Row
                {
                    spacing: 10
                    width: parent.width
                    height: purgeSystemFunction.elementHight
                    enabled: (MachineStatusManager.purgeMachine["status"] === true) ? 0 : 1;
                    Text {
                        text: qsTr("Vac below: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                        width: purgeSystemFunction.labelWidth
                    }
                    TextField
                    {
                        id:purgeMethodOne_vacBelow
                        validator: IntValidator { bottom:5; top: 200}
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - purgeSystemFunction.labelWidth - 20
                        text: "50"
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
                    if(purgeMethodOne_numberCycles.text && purgeMethodOne_nitrogenPressure.text && purgeMethodOne_vacBelow.text)
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
                    enabled: (MachineStatusManager.purgeMachine["status"] === 0) ? 1 : 0;
                }
                Button
                {
                    text: qsTr("Start Purging System")
                    enabled: (purgeMethod_confirm.checked) ? true : false;
                    visible: (MachineStatusManager.purgeMachine["status"] === 0) ? 1 : 0;
                    onClicked:
                    {
                        if(purgeSubmittion.methodSelected == 1)
                        {
                            TestingManager.requestPurge(purgeMethodOne_outputValve.checked, purgeMethodOne_numberCycles.text, purgeMethodOne_nitrogenPressure.text, purgeMethodOne_vacBelow.text);
                        }
                    }
                }
                Button
                {
                    text: qsTr("Stop")
                    visible: (MachineStatusManager.purgeMachine["status"] === 1) ? 1 : 0;
                    onClicked:
                    {
                        TestingManager.requestPurgeStop();
                    }
                }
                Button
                {
                    text: qsTr("Stopping")
                    enabled: false
                    visible: (MachineStatusManager.purgeMachine["status"] === 2) ? 1 : 0;
                }
            }
        }
    }
}
