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
                validator: IntValidator { bottom:1; top: 1000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "1 minimum, 1000 maximum"
            }
        }

        Row
        {
            spacing: 10
            width: parent.width
            height: 50
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
                validator: IntValidator { bottom:100; top: 1000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "100 minimum, 1000 maximum"
            }
        }

        Row
        {
            spacing: 10
            width: parent.width
            height: 50
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
                validator: IntValidator { bottom:100; top: 1000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "100 minimum, 1000 maximum"
            }
        }

        Row{
            spacing: 20
            opacity: ((!setPulseValve_numberCycles.text) || (setPulseValve_timeOpen.text < 100) || (setPulseValve_timeClosed.text < 100)) ? 0 : 1;
            height: ((!setPulseValve_numberCycles.text) || (setPulseValve_timeOpen.text < 100) || (setPulseValve_timeClosed.text < 100)) ? 0 : 50;
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
                text: qsTr("Comfirm rig setup")
            }
            Button
            {
                text: qsTr("Pulse Valve")
                enabled: (setPulseValve_confirm.checked) ? true : false;
                onClicked:
                {
                    TestingManager.requestHighPressure(setHighPressure_pressure.text);
                }
            }
        }
    }

}
