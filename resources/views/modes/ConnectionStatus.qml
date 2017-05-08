import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore

Flickable
{
    clip: true
    contentHeight: Math.max(layout.implicitHeight, height)

    ScrollBar.vertical: ScrollBar {}

    FluidLayouts.AutomaticGrid
    {
        id: layout

        property var noInColoum: 4
        property var noInRow: 2

        anchors.fill: parent
        anchors.left: parent.left
        anchors.leftMargin: 10

        cellWidth: (window.width / noInColoum) - (200 / noInColoum)
        cellHeight: (window.height / layout.noInRow) - 48
        //cellHeight: 150

        // Pressure sensor, Labjack, Vac station, Flow controller 1, Flow controller 2, 12V supply, 24V supply
        model: 8
        delegate: Rectangle
        {
            id: rectangleHolder

            color: "transparent"

            width: (window.width / layout.noInColoum) - (200 / layout.noInColoum)
            height: (window.height / layout.noInRow) - 48

            FluidControls.Card
            {
                id: iconConnect

                width: parent.width - 20
                height: 300

                anchors.top: parent.top
                anchors.topMargin: 40

                Rectangle
                {
                    width: parent.width
                    height: 40
                    id: titleBar
                    color: Material.color(Material.Grey, Material.Shade50)

                    FluidControls.TitleLabel
                    {
                        text: qsTr("Pressure Sensor")
                        horizontalAlignment: Text.AlignHCenter

                        anchors.fill: parent
                        anchors.top: parent.top
                        anchors.topMargin: 5

                        color: Material.color(Material.Grey, Material.Shade700)
                    }
                }

                FluidControls.Icon
                {
                    id: connectionStatusIcon

                    name: "navigation/close"

                    size: 100
                    color: Material.color(Material.Red, Material.Shade500)

                    width: 100
                    height: 100

                    anchors.left: parent.left
                    anchors.leftMargin: (rectangleHolder.width - 100) / 2
                    anchors.top: titleBar.bottom
                    anchors.topMargin: 20
                }

                Column {
                    id: column

                    width: iconConnect.width

                    anchors.top: connectionStatusIcon.bottom
                    anchors.topMargin: 20

                    FluidControls.BodyLabel {
                        text: qsTr("We tried to connect to the {{ name }} but it seems " +
                                   "that the connection has failed. ")
                        wrapMode: Text.WordWrap
                        width: parent.width
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                    }

                    Row {
                        spacing: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        Button {
                            text: qsTr("Help")
                            onClicked: alert.open()
                        }

                        Button {
                            text: qsTr("Re-Connecting")
                        }
                    }
                }
            }
        }
    }

    FluidControls.AlertDialog
    {
        id: alert

        width: (window.width / 2)
        height: (window.height / 3)

        x: (parent.width - width) / 3
        y: (parent.height - height) / 2.5

        title: "Connection Failed"
        text: qsTr("We tried to connect to the {{ name }} but it seems " +
                   "that the connection has failed. " +
                   "Try rebooting the PC and reloading the application, " +
                   "if that does not work check the ID for the {{ name }} " +
                   "in the settings for the applcation. " +
                   "If all else failed contact JFET services, The University of Manchester. ")

        standardButtons: Dialog.Cancel
    }
}


