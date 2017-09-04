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
            textContent: qsTr("This function will set a high pressure within the system. Ensure the rig is correctly setup before running.")
        }

        Row
        {
            anchors.left: parent.left
            spacing: 10
            width: parent.width
            height: 50
            Text {
                text: qsTr("Pressure (in bar): ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: parent.height
            }
            TextField
            {
                id: setHighPressure_pressure
                validator: IntValidator { bottom:0; top: 90 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 170
                placeholderText: "10"
            }
        }
        Row{
            spacing: 20
            opacity: (!setHighPressure_pressure.text) ? 0 : 1;
            height: (!setHighPressure_pressure.text) ? 0 : 50;
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
                onClicked:
                {
                    TestingManager.requestHighPressure(setHighPressure_pressure.text);
                }
            }
        }
    }
}