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
        model: ListModel {
            id: toMonitor
            ListElement { title: qsTr("Pressure Sensor"); desc: qsTr("This shows the status of the connection to the pressure sensor."); status: 3 }
            ListElement { title: qsTr("LabJack"); desc: qsTr("This shows the status of the connection to the LabJack."); status: 3 }
            ListElement { title: qsTr("Vac Station"); desc: qsTr("This shows the status of the connection to the vacuum station.") ; status: 3 }
            ListElement { title: qsTr("Flow Controller 1"); desc: qsTr("This shows the status of the connection to flow controller one."); status: 3  }
            ListElement { title: qsTr("Flow Controller 2"); desc: qsTr("This shows the status of the connection to flow controller two."); status: 3 }
            ListElement { title: qsTr("System Condition"); desc: qsTr("This shows whether the gas rig is in a state that it can be used."); status: 3 }
            ListElement { title: qsTr("12V Supply"); desc: qsTr("This shows whether the 12V internal power supply is functioning"); status: 3 }
            ListElement { title: qsTr("24V Supply"); desc: qsTr("This shows whether the 12V internal power supply is functioning") ; status: 3 }
        }
        delegate: Rectangle
        {
            id: rectangleHolder

            color: "transparent"

            width: (window.width / layout.noInColoum) - (200 / layout.noInColoum)
            height: (window.height / layout.noInRow) - 48

            state: model.status

            states: [
                State {
                    name: "1"
                    PropertyChanges { target: connectionStatusIcon; name: "action/done"; visible: true; color: Material.color(Material.Green, Material.Shade500)}
                    PropertyChanges { target: connectionStatusLoading; visible: false; }
                },
                State {
                    name: "2"
                    PropertyChanges { target: connectionStatusIcon; name: "navigation/close"; visible: true; }
                    PropertyChanges { target: connectionStatusLoading; visible: false; }
                },
                State {
                    name: "3"
                    PropertyChanges { target: connectionStatusIcon; name: "action/cached"; visible: false; }
                    PropertyChanges { target: connectionStatusLoading; visible: true; }
                }
            ]

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
                        text: model.title
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

                    visible: false
                }

                BusyIndicator {
                    id: connectionStatusLoading
                    running: true
                    visible: false

                    width: 75
                    height: 75

                    anchors.left: parent.left
                    anchors.leftMargin: (rectangleHolder.width - 75) / 2
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
                        id: helpReconnectButtons
                        spacing: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        Button {
                            text: qsTr("Help")
                            onClicked: {
                                toMonitor.set(index, {"status": 1})
                                alert.open()
                            }
                        }

                        Button {
                            text: qsTr("Re-Connecting")
                            onClicked: {
                                toMonitor.set(index, {"status": 3})
                            }
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
        text: qsTr("<p>We tried to connect to the device but it seems that the connection has failed.</p><br/>" +
                   "<p>Try rebooting the PC and reloading the application, if that does not work check the vendor and product ID for the device in the settings for the applcation. </p><br/>" +
                   "<p>If all else failed contact JFET services, The University of Manchester. </p>")

        standardButtons: Dialog.Cancel
    }
}


