import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item {

    id: vacDownRoot

    width: parent.width - 30

    anchors.left: parent.left;
    anchors.leftMargin: 15

    property bool shouldEnable: false

    opacity: (vacDownRoot.shouldEnable) ? 1 : 0.7


    FluidControls.Card
    {
        width: parent.width
        height: (vacDownRoot.shouldEnable) ? (80 + vacuumForm.height) : 40

        Rectangle
        {
            width: parent.width
            height: 40
            color: Material.color(Material.Grey, Material.Shade50)

            FluidControls.TitleLabel
            {
                id: vacTitleBarHeading

                text: "Vacuun Down Cell"
                horizontalAlignment: Text.AlignHCenter

                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: 5

                color: Material.color(Material.Grey, Material.Shade700)
            }

            Column
            {
                id: vacuumForm

                anchors.top: vacTitleBarHeading.bottom
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 5
                spacing: 3;
                width: parent.width -10

                visible: (vacDownRoot.shouldEnable) ? 1 : 0

                AlertBox
                {
                    width: parent.width
                    type: "Notice"
                    textContent: qsTr("Configure the vac down sequence, before starting the vac down")
                }
                Row
                {
                    spacing: 20;
                    TextField
                    {
                        id: vacDown_time
                        placeholderText: qsTr("Time in minutes")
                        validator: IntValidator { bottom:0; top: 10000}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: vacuumForm.width - 110
                        enabled: (vacDownRoot.shouldEnable) ? 1 : 0;
                    }

                    CheckBox {
                        id: vacDown_turbo
                        checked: false
                        text: qsTr("Turbo")
                        enabled: (vacDownRoot.shouldEnable) ? 1 : 0;
                    }
                }
            }
        }
    }

}







