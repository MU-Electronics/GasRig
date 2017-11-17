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
 * Set a pulsing valve
 */
FluidControls.Card
{
    width: parent.width-10
    height: pulsingValveControlContainer.height + 30

    padding: 5

    Column
    {
        id: pulsingValveControlContainer

        width: parent.width

        spacing: 20

        AlertBox
        {
            width: pulsingValveControl.width - 10
            type: "Notice"
            textContent: qsTr("This function will pulse a valve for a number of cycles with a set duty cycle.")
        }

        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pulseValveMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Select valve:          ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            ComboBox {
                id: setPulseValve_valveId
                height: parent.height
                width: parent.width - 170
                textRole: "key"
                model: ListModel {
                    ListElement { key: "Valve 1 (Output port)"; value: 1 }
                    ListElement { key: "Valve 2 (Slow exhuast path)"; value: 2 }
                    ListElement { key: "Valve 3 (Exhaust port)"; value: 3 }
                    ListElement { key: "Valve 4 (Fast exhaust path)"; value: 4 }
                    ListElement { key: "Valve 5 (Vacuum In)"; value: 5 }
                    ListElement { key: "Valve 6 (Vacuum Out)"; value: 6 }
                    ListElement { key: "Valve 7 (High Pressure In)"; value: 7 }
                    ListElement { key: "Valve 8 (Flow Controllers)"; value: 8 }
                    ListElement { key: "Valve 9 (Nitrogen In)"; value: 9 }
                }
            }
        }

        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pulseValveMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Number of cycles: ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            TextField
            {
                id: setPulseValve_numberCycles
                validator: IntValidator { bottom:1; top: 10000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "1 minimum, 10000 maximum"
            }
        }

        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pulseValveMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Time open (ms):    ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            TextField
            {
                id: setPulseValve_timeOpen
                validator: IntValidator { bottom:1; top: 10000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "1 minimum, 10000 maximum"
            }
        }

        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pulseValveMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Time closed (ms):  ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            TextField
            {
                id: setPulseValve_timeClosed
                validator: IntValidator { bottom:1; top: 10000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "1 minimum, 10000 maximum"
            }
        }

        Row{
            spacing: 20
            opacity: ((!setPulseValve_numberCycles.text) || (setPulseValve_timeOpen.text < 1) || (setPulseValve_timeClosed.text < 1)) ? 0 : 1;
            height: ((!setPulseValve_numberCycles.text) || (setPulseValve_timeOpen.text < 1) || (setPulseValve_timeClosed.text < 1)) ? 0 : 50;
            width: parent.width
            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
            CheckBox {
                id:setPulseValve_confirm
                checked: false
                enabled: (MachineStatusManager.pulseValveMachine["status"] === true) ? 0 : 1;
                text: qsTr("Comfirm rig setup")
            }
            Button
            {
                text: qsTr("Pulse Valve")
                enabled: (setPulseValve_confirm.checked) ? true : false;
                visible: (MachineStatusManager.pulseValveMachine["status"] === true) ? 0 : 1;
                onClicked:
                {
                    TestingManager.requestPulseValve(setPulseValve_valveId.model.get(setPulseValve_valveId.currentIndex).value, setPulseValve_numberCycles.text, setPulseValve_timeOpen.text, setPulseValve_timeClosed.text);
                }
            }

            Button
            {
                text: qsTr("Stop Pulse Valve")
                enabled: (setPulseValve_confirm.checked) ? true : false;
                visible: (MachineStatusManager.pulseValveMachine["status"] === true) ? 1 : 0;
                onClicked:
                {
                    TestingManager.requestPulseValveStop();
                }
            }
        }
    }

}
