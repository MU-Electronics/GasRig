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
  * Exhuast sysem
  */
FluidControls.Card
{
    id: exhuastSystem
    padding: 5
    width: parent.width - 10
    height: exhuastSystemContainer.height + 30

    Column
    {
        width: parent.width
        spacing: 20
        id: exhuastSystemContainer
        AlertBox
        {
            width: exhuastSystem.width - 10
            type: "Warning"
            textContent: qsTr("This function will exhuast high pressures safely to the exhuast port. Ensure the exhuast port is conneted to the correct and safe location.")
        }
        Row{
            RadioButton {
                checked: false
                id: exhuastSystem_valveSlow
                text: qsTr("Slow filter")
            }
            RadioButton {
                checked: false
                id: exhuastSystem_valveFast
                text: qsTr("Fast filter")
            }
            RadioButton {
                checked: true
                id: exhuastSystem_valveCombo
                text: qsTr("Combination of fast + slow filter")
            }
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
                id: exhuastSystem_frequency
                validator: IntValidator { bottom:-1; top: 5 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "2"
            }
        }
        Row{
            spacing: 20
            opacity: (!exhuastSystem_frequency.text) ? 0 : 1;
            height: (!exhuastSystem_frequency.text) ? 0 : 50;
            width: parent.width
            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
            CheckBox {
                id:exhuastSystem_confirm
                checked: false
                text: qsTr("Comfirm rig setup")
            }
            Button
            {
                text: qsTr("Begin Exhuast")
                enabled: (exhuastSystem_confirm.checked) ? true : false;
                onClicked:
                {
                    var filterType;
                    if(exhuastSystem_valveSlow.checked)
                        filterType = 1;
                    if(exhuastSystem_valveFast.checked)
                        filterType = 2;
                    if(exhuastSystem_valveCombo.checked)
                        filterType = 3;

                    TestingManager.requestExhuast(filterType, exhuastSystem_frequency.text);
                }
            }
        }
    }
}
