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
  * Set a high pressure in the system
  */
FluidControls.Card
{
    id: root

    width: parent.width - 10
    height: setHighPressureContainer.height + 30
    padding: 5

    property int labelWidth: 220
    property int labelPadding: 20

    Behavior on height {
        PropertyAnimation {
            duration: 500
        }
    }

    Column
    {
        width: parent.width

        spacing: 20

        id: setHighPressureContainer

        enabled: (MachineStatusManager.controllable["pressuriseMachine"] === 0) ? 0 : 1 ;

        move: Transition {
            NumberAnimation { properties: "x,y"; duration: 500 }
        }


        AlertBox
        {
            width: setHighPressure.width - 10
            type: "Danger"
            textContent: qsTr("This function will set a high pressure (output valve will open periodically), ensure the rig is correctly setup before running.")
        }

        Row
        {
            anchors.left: parent.left
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Desired end pressure (mBar): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            TextField
            {
                id: setHighPressure_pressure
                validator: IntValidator { bottom:20; top: 90000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - root.labelWidth - root.labelPadding
                placeholderText: "Minimum: 20 mBar;      Maximum: 90000 mBar"
            }
        }
        Row
        {
            anchors.left: parent.left
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Valve one step size (mBar): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            TextField
            {
                id: setStepSize_pressure
                validator: IntValidator { bottom:100; top: 6000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - root.labelWidth - root.labelPadding
                text: "2000"
                placeholderText: "Minimum: 100 mBar;      Maximum: 6000 mBar"
            }
        }
        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Select valve:")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            ComboBox {
                id: setInputValve_pressure
                height: parent.height
                width: parent.width - root.labelWidth - root.labelPadding
                textRole: "key"
                model: ListModel {
                    ListElement { key: "Valve 7 (High Pressure In)"; value: true }
                    ListElement { key: "Valve 9 (Nitrogen In)"; value: false }
                }
            }
        }
        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Disable inital vac down: ")
                color: "#595959"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            CheckBox {
                id: setUseVac_pressure
                checked: true
                text: (!setUseVac_pressure.checked) ? "Init pressure must be atmospheric" : ""
            }
        }
        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Continuous Pressurising: ")
                color: "#595959"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            CheckBox {
                id: setContiniousPressure
                checked: (setHighPressure_pressure.text < 20) ? false : false;
                enabled: (setHighPressure_pressure.text < 20) ? false : true;
                text: (!setContiniousPressure.checked) ? "Pressurises then stops" : "Tops up pressure at set intervals if required"
            }
        }
        Row
        {
            anchors.left: parent.left
            spacing: 10
            width: parent.width
            height: (setContiniousPressure.checked) ? 50 : 0;
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true && setContiniousPressure.checked) ? 0 : 1;
            opacity: (setContiniousPressure.checked) ? 1 : 0;
            Text {
                text: qsTr("Max time (min): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth - 100
            }
            TextField
            {
                id: setMaxTime
                validator: IntValidator { bottom:1; top: 9999 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: (parent.width/2) - ((root.labelWidth - root.labelPadding) * 1)
                text: "60"
                placeholderText: "Minimum: 1 minute;      Maximum: 9999 mBar"
            }

            Text {
                text: qsTr("            Monitor Time (secs): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            TextField
            {
                id: setMonitorTime
                validator: IntValidator { bottom:5; top: 120 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: (parent.width/2) - ((root.labelWidth - root.labelPadding) * 1)
                text: "60"
                placeholderText: "Minimum: 5 second;      Maximum: 120 second"
            }
        }
        Row
        {
            anchors.left: parent.left
            spacing: 10
            width: parent.width
            height: (setContiniousPressure.checked) ? 50 : 0;
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true && setContiniousPressure.checked) ? 0 : 1;
            opacity: (setContiniousPressure.checked) ? 1 : 0;
            Text {
                text: qsTr("Top up (mBar): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth - 100
            }
            TextField
            {
                id: setTopUp
                validator: IntValidator { bottom:1; top: (setHighPressure_pressure.text - 11) }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: (parent.width/2) - ((root.labelWidth - root.labelPadding) * 1)
                text: (setHighPressure_pressure.text > 1000) ? "100" : 10
                placeholderText: "Minimum: 10 mBar;      Maximum: " + (setHighPressure_pressure.text - 11) + " mBar"
            }

            Text {
                text: qsTr("            Leak (mBar): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            TextField
            {
                id: setLeak
                validator: IntValidator { bottom:(setTopUp.text + 5); top: (setHighPressure_pressure.text - 5) }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: (parent.width/2) - ((root.labelWidth - root.labelPadding) * 1)
                text: (setHighPressure_pressure.text > 1000) ? "500" : 15
                placeholderText: "Minimum: " + (setTopUp.text + 5) + " mBar;      Maximum: " + (setHighPressure_pressure.text - 5) + " mBar"
            }
        }
        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            enabled: (MachineStatusManager.pressuriseMachine["status"] === true) ? 0 : 1;
            Text {
                text: qsTr("Should open output valve: ")
                color: "#595959"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: root.labelWidth
            }
            CheckBox {
                id: setUseVac_openOutputValve
                checked: true
                text: (setUseVac_openOutputValve.checked) ? "Opens at step intervals" : "Never opens"
            }
        }
        Row{
            spacing: 20
            opacity: (setHighPressure_pressure.text < 20 || setStepSize_pressure.text < 100) ? 0 : 1;
            height: (setHighPressure_pressure.text < 20 || setStepSize_pressure.text < 100) ? 0 : 50;
            width: parent.width
            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
            CheckBox {
                id:setHighPressure_confirm
                checked: false
                text: qsTr("Comfirm rig setup")
                enabled: (MachineStatusManager.pressuriseMachine["status"] === 0) ? 1 : 0;
            }
            Button
            {
                text: qsTr("Set high pressure")
                enabled: (setHighPressure_confirm.checked) ? true : false;
                visible: (MachineStatusManager.pressuriseMachine["status"] === 0) ? 1 : 0;
                onClicked:
                {
                    TestingManager.requestHighPressure(setHighPressure_pressure.text, setUseVac_pressure.checked, setStepSize_pressure.text, setInputValve_pressure.model.get(setInputValve_pressure.currentIndex).value, setUseVac_openOutputValve.checked);
                }
            }
            Button
            {
                text: qsTr("Stop")
                visible: (MachineStatusManager.pressuriseMachine["status"] === 1) ? 1 : 0;
                onClicked:
                {
                    TestingManager.requestHighPressureStop();
                }
            }
            Button
            {
                text: qsTr("Stopping")
                enabled: false
                visible: (MachineStatusManager.pressuriseMachine["status"] === 2) ? 1 : 0;
            }
        }
    }
}
