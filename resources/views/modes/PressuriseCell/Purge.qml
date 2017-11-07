import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item {

    width: parent.width - 30
    height: 80 + purgeSettingsForm.height

    anchors.left: parent.left;
    anchors.leftMargin: 15

    property bool shouldEnable: false

    FluidControls.Card
    {
        width: parent.width
        height: parent.height

        Rectangle
        {
            width: parent.width
            height: 40
            color: Material.color(Material.Grey, Material.Shade50)

            FluidControls.TitleLabel
            {
                id: purgeTitleBarHeading

                text: "Purge Cell"
                horizontalAlignment: Text.AlignHCenter

                anchors.fill: parent
                anchors.top: parent.top
                anchors.topMargin: 5

                color: Material.color(Material.Grey, Material.Shade700)
            }

            Column
            {
                id: purgeSettingsForm

                anchors.top: purgeTitleBarHeading.bottom
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 5

                spacing: 3;
                width: parent.width

                AlertBox
                {
                    width: parent.width - 10
                    type: "Notice"
                    textContent: qsTr("Configure the purging sequence, before starting the purge sequence")
                }

                Button {
                    text: "Click me"
                    onClicked: editParamsAertDialog.open()
                }
            }
        }
    }


    FluidControls.AlertDialog
    {
        id: editParamsAertDialog

        // Size of the alert box
        width: (window.width / 2)
        height: (window.height / 2)

        // Location of alert box in window
        x: ((window.width - width) / 2) - 220 // X relative to parent so remove side bar width
        y: ((window.height - height) / 2) - 50 // Y relative to parent so remove top bar height

        // Define the buttons we want showing
        standardButtons: Dialog.Cancel | Dialog.SaveAll

        // When accepted we need to update the variables
        onAccepted: {
            console.log('Appling new values')
        }

        // When rejected we need to reset the controls back to the current set values
        onRejected: {
            console.log('Discarding new values')
        }

        title: "Edit Purge Parameters"

        // Content within the alert box
        content: Column
        {
            width: parent.width
            height: parent.height

            // Vac below param
            Row
            {
                spacing: 10
                width: parent.width
                height: 50
                Text {
                    text: qsTr("Vac Below: ")
                    color: "#595959"
                    visible: parent.opacity
                    font.pixelSize: 16
                    verticalAlignment : Text.AlignVCenter
                    height: parent.height
                }
                Slider
                {
                    id: setVacBelow_pressureCell
                    width: parent.width - 220
                    from: 5
                    value: 10
                    to: 200
                }
                Text{
                    text: setVacBelow_pressureCell.value.toFixed(2) + " " + qsTr("mBar")
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    font.pointSize: 16
                    color: "#595959"
                }
            }
            // Should be nitrogen param
            Row{
                spacing: 10
                width: parent.width
                height: 50
                Text {
                    text: qsTr("Use Nitrogen: ")
                    color: "#595959"
                    visible: parent.opacity
                    font.pixelSize: 16
                    verticalAlignment : Text.AlignVCenter
                    height: parent.height
                }
                CheckBox {
                    id: setUseNitrogen_pressureCell
                    checked: false
                }
            }
            // What nitrogen pressure param
            Row{
                spacing: 10

                width: parent.width
                anchors.left: parent.left;
                anchors.leftMargin: 50

                opacity: (setUseNitrogen_pressureCell.checked) ? 1 : 0;
                height: (setUseNitrogen_pressureCell.checked) ? 50 : 0;

                Text {
                    text: qsTr("Nitrogen Pressure: ")
                    color: "#595959"
                    visible: parent.opacity
                    font.pixelSize: 16
                    verticalAlignment : Text.AlignVCenter
                    height: parent.height
                }
                Slider
                {
                    id: setNitrogenPressure_pressureCell
                    width: parent.width - 380
                    height: parent.height
                    from: 0.5
                    value: 2
                    to: 10
                }
                Text{
                    text: setNitrogenPressure_pressureCell.value.toFixed(2) + " " + qsTr("Bar")
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    font.pointSize: 16
                    color: "#595959"
                    height: parent.height
                }
            }
            // How many cyles param
            Row{
                spacing: 10
                width: parent.width
                height: 50

                Text {
                    text: qsTr("Number of Cycles: ")
                    color: "#595959"
                    visible: parent.opacity
                    font.pixelSize: 16
                    verticalAlignment : Text.AlignVCenter
                    height: parent.height
                }
                SpinBox {
                    id: setNumberCycles_pressureCell
                    value: 20
                    from: 1
                    to: 1000
                    stepSize: 1
                }
            }
        }
    }

}
