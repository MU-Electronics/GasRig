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
  * Output high pressure safely
  */
FluidControls.Card
{
    id: outputHighPressure
    width: parent.width - 10
    height: outputHighPressureContainer.height + 30
    padding: 5

    Column
    {
        width: parent.width
        spacing: 20
        id: outputHighPressureContainer
        AlertBox
        {
            width: outputHighPressure.width - 10
            type: "Warning"
            textContent: qsTr("This function will pulse the output valve to output port pressures safely. Ensure the output port is conneted to the correct and safe location.")
        }

        Row
        {
            anchors.left: parent.left
            spacing: 10
            width: parent.width
            height: 50
            Text {
                text: qsTr("Frequency: ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            TextField
            {
                id: outputHighPressure_frequency
                validator: IntValidator { bottom:-1; top: 5 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "2"
            }
        }
        Row{
            spacing: 20
            opacity: (!outputHighPressure_frequency.text) ? 0 : 1;
            height: (!outputHighPressure_frequency.text) ? 0 : 50;
            width: parent.width
            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
            CheckBox {
                id:outputHighPressure_confirm
                checked: false
                text: qsTr("Comfirm rig setup")
            }
            Button
            {
                text: qsTr("Begin Outputting Pressure")
                enabled: (outputHighPressure_confirm.checked) ? true : false;
                onClicked:
                {
                    TestingManager.requestOutputPressure(outputHighPressure_frequency.text);
                }
            }
        }
    }
}
