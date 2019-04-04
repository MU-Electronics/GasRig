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

        property var noInColoum: 3
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
            ListElement { title: qsTr("Pressure Sensor"); desc: qsTr("This shows the status of the connection to the pressure sensor."); hardware: "PressureSensor"; config: ""; type: "bus" }
            ListElement { title: qsTr("LabJack"); desc: qsTr("This shows the status of the connection to the LabJack."); hardware: "LabJack"; config: ""; type: "bus" }
            ListElement { title: qsTr("Vac Station"); desc: qsTr("This shows the status of the connection to the vacuum station."); hardware: "VacStation"; config: ""; type: "bus" }
            ListElement { title: qsTr("Flow Controller"); desc: qsTr("This shows the status of the connection to flow controller one."); hardware: "FlowController"; config: "valve_8"; type: "bus" }
            ListElement { title: qsTr("Fast Exhuast"); desc: qsTr("Allows exhuast & pumping of system faster"); hardware: ""; config: "valve_4"; type: "config" }
            ListElement { title: qsTr("Vacuum Output"); desc: qsTr("Dedicated output for pumping station"); hardware: ""; config: "valve_6"; type: "config" }
        }
        delegate: Rectangle
        {
            id: rectangleHolder

            color: "transparent"

            width: (window.width / layout.noInColoum) - (200 / layout.noInColoum)
            height: (window.height / layout.noInRow) - 48

            state: {
                if(model.hardware === "")
                {
                    // Config setting
                    var enabled = GlobalManager.rigSettings[model.config];

                    if(enabled)
                        return 1;

                    return 0;
                }
                else
                {
                    if(model.config === "")
                        return ConnectionStatusManager.hardwareConnection[model.hardware];

                    // Config setting
                    var enabled = GlobalManager.rigSettings[model.config];

                    if(enabled)
                        return ConnectionStatusManager.hardwareConnection[model.hardware];

                    return 0;
                }
            }

            states: [
                // Not connected
                State {
                    name: "0"
                    PropertyChanges { target: connectionStatusIcon; name: "navigation/cancel"; visible: true; color: Material.color(Material.Red, Material.Shade500)} //done
                    PropertyChanges { target: connectionStatusLoading; visible: false; }
                },
                // Connected
                State {
                    name: "1"
                    PropertyChanges { target: connectionStatusIcon; name: "action/check_circle"; visible: true; color: Material.color(Material.Green, Material.Shade500)} //done
                    PropertyChanges { target: connectionStatusLoading; visible: false; }
                },
                // Timed out
                State {
                    name: "2"
                    PropertyChanges { target: connectionStatusIcon; name: "image/timelapse"; visible: true; color: Material.color(Material.Orange, Material.Shade500)} //navigation/close
                    PropertyChanges { target: connectionStatusLoading; visible: false; }
                },
                // Error connecting
                State {
                    name: "3"
                    PropertyChanges { target: connectionStatusIcon; name: "alert/warning"; visible: true; color: Material.color(Material.Orange, Material.Shade500)} //navigation/close
                    PropertyChanges { target: connectionStatusLoading; visible: false; }
                },
                // Loading circle
                State {
                    name: "4"
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

                    spacing: 20

                    width: iconConnect.width

                    anchors.top: connectionStatusIcon.bottom
                    anchors.topMargin: 20

                    FluidControls.BodyLabel {
                        text: model.desc
                        wrapMode: Text.WordWrap
                        horizontalAlignment: Text.AlignHCenter
                        width: parent.width
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        id: explainConnection
                    }

                    Row {
                        id: helpReconnectButtons
                        spacing: 20
                        anchors.left: parent.left
                        anchors.leftMargin: (rectangleHolder.width - 230) / 2
                        width:230
                        opacity: (((model.type === "bus" && model.config === "")) || (model.type === "bus" && GlobalManager.rigSettings[model.config])) ? 1 : 0
                        height: (((model.type === "bus" && model.config === "")) || (model.type === "bus" && GlobalManager.rigSettings[model.config])) ? 50 : 0
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
                                ConnectionStatusManager.request_reconnect(model.hardware)
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
                   "<p>If all else failed contact Electronic services, The University of Manchester. </p>")

        standardButtons: Dialog.Cancel
    }
}


