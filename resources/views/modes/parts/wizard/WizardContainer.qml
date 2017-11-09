
import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore

Item {

    id: wizardContainerRoot

    // Properties for wizard container layout
    property alias title: wizardContainerTitleBarHeading.text
    property alias sideBarContainer: sideBarPlaceHolder.data
    property alias topContainer: topContainerPlaceHolder.data
    property alias bottomContainer: bottomContainerPlaceHolder.data

    // Property to set the current statuses of the container
    property int stage: 0
    property bool shouldEnable: false

    // Properties for param Dialog
    property alias paramDialogId: paramDialog
    property alias paramDialogContainer: paramDialogContainerPlaceHolder.data
    property var paramDialogOnAccepted: (function() { console.log("Accepted params") })
    property var paramDialogOnRejected: (function() { console.log("Rejected params") })

    // Properties for cancel Dialog
    property alias cancelDialogId: cancelDialog
    property alias cancelDialogContainer: cancelDialogContainerPlaceHolder.data
    property var cancelDialogOnAccepted: (function() { console.log("Accepted cancel") })
    property var cancelDialogOnRejected: (function() { console.log("Rejected cancel") })

    // Properties for container size
    property var containerHeight: 200
    property var topContainerHeight: containerHeight / 2
    property var bottomContainerHeight: (containerHeight / 2) - 2

    width: parent.width - 30
    height: 40 + wizardContainerSettingsForm.height
    opacity: (wizardContainerRoot.shouldEnable) ? 1 : 0.7

    FluidControls.Card
    {
        width: parent.width
        height: (wizardContainerRoot.shouldEnable) ? parent.height : 40

        Rectangle
        {
            width: parent.width
            height: 40
            color: Material.color(Material.Grey, Material.Shade50)

            FluidControls.TitleLabel
            {
                id: wizardContainerTitleBarHeading

                text: "Purge Cell"
                horizontalAlignment: Text.AlignHCenter

                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: 5

                color: Material.color(Material.Grey, Material.Shade700)
            }

            Column
            {
                id: wizardContainerSettingsForm

                anchors.top: wizardContainerTitleBarHeading.bottom

                spacing: 3;
                width: parent.width

                height: (wizardContainerRoot.shouldEnable) ? wizardContainerRoot.containerHeight : 0
                visible: (wizardContainerRoot.shouldEnable) ? 1 : 0

                Row{
                    width: parent.width
                    height: parent.height

                    Column{
                        width: parent.width - 150
                        height: parent.height
                        Rectangle{
                            width: parent.width
                            height: wizardContainerRoot.topContainerHeight

                            Item{
                                id: topContainerPlaceHolder
                                width: parent.width
                                height: parent.height
                            }
                        }
                        Rectangle{
                            width: parent.width
                            color: '#e2e2e2'
                            height: 2
                        }
                        Rectangle{
                            width: parent.width
                            height: wizardContainerRoot.bottomContainerHeight
                            Row
                            {
                                width: parent.width
                                height: parent.height
                                Item{
                                    id: bottomContainerPlaceHolder
                                    width: parent.width - 100
                                    height: parent.height
                                }
                                Rectangle{
                                    width: 2
                                    color: '#e2e2e2'
                                    height: parent.height
                                }
                                Item{
                                    id: modeStageStatus

                                    width: 98
                                    height: parent.height

                                    state: wizardContainerRoot.stage
                                    states: [
                                        // Awaiting users input
                                        State {
                                            name: "0"
                                            PropertyChanges { target: statusIcon; name: "social/person"; visible: true; color: Material.color(Material.Orange, Material.Shade500)} //done
                                            PropertyChanges { target: statusText; text: "Waiting User"; color: Material.color(Material.Orange, Material.Shade700)}
                                            PropertyChanges { target: statusIcon_inProgress; visible: false; }
                                        },
                                        // In process
                                        State {
                                            name: "1"
                                            PropertyChanges { target: statusIcon; name: "action/cached"; visible: false; color: Material.color(Material.Blue, Material.Shade500) } //done
                                            PropertyChanges { target: statusText; text: "In Progress"; color: Material.color(Material.Blue, Material.Shade700)}
                                            PropertyChanges { target: statusIcon_inProgress; visible: true; }
                                        },
                                        // Finished awaiting user action
                                        State {
                                            name: "2"
                                            PropertyChanges { target: statusIcon; name: "action/assignment_turned_in"; visible: true; color: Material.color(Material.Green, Material.Shade500)} //done
                                            PropertyChanges { target: statusText; text: "Finished Awaiting User"; color: Material.color(Material.Green, Material.Shade700)}
                                            PropertyChanges { target: statusIcon_inProgress; visible: false; }
                                        },
                                        // Error
                                        State {
                                            name: "3"
                                            PropertyChanges { target: statusIcon; name: "navigation/cancel"; visible: true; color: Material.color(Material.Red, Material.Shade500)} //done
                                            PropertyChanges { target: statusText; text: "Error Occurred"; color: Material.color(Material.Red, Material.Shade700)}
                                            PropertyChanges { target: statusIcon_inProgress; visible: false; }
                                        }
                                    ]

                                    FluidControls.Icon
                                    {
                                        id: statusIcon

                                        name: "image/timelapse"

                                        size: 100
                                        color: Material.color(Material.Red, Material.Shade500)

                                        width: 60
                                        height: 60

                                        anchors.left: modeStageStatus.left
                                        anchors.leftMargin: 20
                                        anchors.top: modeStageStatus.top
                                        anchors.topMargin: (parent.height - height - statusText.height) / 2

                                        visible: true
                                    }

                                    BusyIndicator {
                                        id: statusIcon_inProgress
                                        running: true
                                        visible: false

                                        width: 60
                                        height: 60

                                        anchors.left: modeStageStatus.left
                                        anchors.leftMargin: 20
                                        anchors.top: modeStageStatus.top
                                        anchors.topMargin: 5
                                    }

                                    Text{
                                        id:statusText

                                        text: "Error"
                                        color: "#000000"

                                        width: parent.width
                                        height: 35

                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        wrapMode: Text.WordWrap

                                        anchors.top: modeStageStatus.top
                                        anchors.topMargin: statusIcon.anchors.topMargin + statusIcon.height
                                    }
                                }
                            }


                        }
                    }
                    Rectangle{
                        width: 3
                        height: parent.height
                        color: '#e2e2e2'
                    }
                    Rectangle{
                        width: 147
                        height: parent.height
                        color: '#f9f9f9'
                        Item{
                            id: sideBarPlaceHolder
                            width: parent.width
                        }
                    }
                }
            }
        }
    }


    FluidControls.AlertDialog
    {
        id: cancelDialog

        // Size of the alert box
        width: (window.width / 2)
        height: (window.height / 4)

        // Location of alert box in window
        x: ((window.width - width) / 2) - 220 // X relative to parent so remove side bar width
        y: ((window.height - height) / 2) - 50 // Y relative to parent so remove top bar height

        // Define the buttons we want showing
        standardButtons: Dialog.Cancel | Dialog.Ok

        // When accepted we need to update the variables
        onAccepted: wizardContainerRoot.cancelDialogOnAccepted()

        // When rejected we need to reset the controls back to the current set values
        onRejected: wizardContainerRoot.cancelDialogOnRejected()

        title: "Cancel Mode"

        // Content within the alert box
        content: Item{
            id: cancelDialogContainerPlaceHolder
            width: parent.width
            height: parent.height
            Text{
                width: parent.width
                wrapMode: Text.WordWrap
                text: qsTr( '<p>By canceling the mode you will canceling the entire process!</p>
                             <p>The mode will shut down but you will be responsable for making the rig safe, for example, exhuasting pressures safely</p>')
            }
        }
    }


    FluidControls.AlertDialog
    {
        id: paramDialog

        // Size of the alert box
        width: (window.width / 2)
        height: (window.height / 2)

        // Location of alert box in window
        x: ((window.width - width) / 2) - 220 // X relative to parent so remove side bar width
        y: ((window.height - height) / 2) - 50 // Y relative to parent so remove top bar height

        // Define the buttons we want showing
        standardButtons: Dialog.Cancel | Dialog.SaveAll

        // When accepted we need to update the variables
        onAccepted: wizardContainerRoot.paramDialogOnAccepted()

        // When rejected we need to reset the controls back to the current set values
        onRejected: wizardContainerRoot.paramDialogOnRejected()

        title: "Edit Parameters"

        // Content within the alert box
        content: Item{
            id: paramDialogContainerPlaceHolder
            width: parent.width
            height: parent.height
        }
    }

}
