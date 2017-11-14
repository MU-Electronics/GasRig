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
    width: parent.width - 10
    height: setHighPressureContainer.height + 30
    padding: 5


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
            Text {
                text: qsTr("Pressure (min 1 mBar): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            TextField
            {
                id: setHighPressure_pressure
                validator: IntValidator { bottom:1; top: 90000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 220
                placeholderText: "1100"
            }
        }
        Row
        {
            spacing: 10
            width: parent.width
            height: 50
            Text {
                text: qsTr("Disable Inital Vac Down: ")
                color: "#595959"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            CheckBox {
                id: setUseVac_pressure
                checked: true
            }
        }
        Row{
            spacing: 20
            opacity: (setHighPressure_pressure.text < 1) ? 0 : 1;
            height: (setHighPressure_pressure.text < 1) ? 0 : 50;
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
            }
            Button
            {
                text: qsTr("Set high pressure")
                enabled: (setHighPressure_confirm.checked) ? true : false;
                visible: (MachineStatusManager.pressuriseMachine["status"] === false) ? 1 : 0;
                onClicked:
                {
                    TestingManager.requestHighPressure(setHighPressure_pressure.text, setUseVac_pressure.checked);
                }
            }
            Button
            {
                text: qsTr("Stop")
                visible: (MachineStatusManager.pressuriseMachine["status"] === true) ? 1 : 0;
                onClicked:
                {
                    TestingManager.requestHighPressureStop();
                }
            }
        }
    }
}
