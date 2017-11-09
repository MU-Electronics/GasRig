import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import Fluid.Layouts 1.0 as FluidLayouts

// Global part files
import "../../parts"

// Mode part files
import "../parts"

Item {

    id: root

    width: parent.width
    height: purgeCellWizard.height

    anchors.left: parent.left;
    anchors.leftMargin: 15

    property bool shouldEnable: false

    property var currentState: 1

    WizardContainer{
        id: purgeCellWizard

        title: "Stage One: Purge Cell"

        stage: root.currentState
        shouldEnable: true

        topContainer: Item{

            width: parent.width

            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10

            WizardContainerTitle{
                id: paramContainerTitle
            }

            WizardParamsList{
                anchors.top: paramContainerTitle.bottom
                anchors.topMargin: 5

                model: ListModel {
                    ListElement { title: qsTr("Vac down to: "); value: "10 mBar" }
                    ListElement { title: qsTr("Number of cycles: "); value: "25" }
                    ListElement { title: qsTr("Use Nitrogen: "); value: "Yes" }
                    ListElement { title: qsTr("Pressure: "); value: "5.25 Bar" }
                }
            }
        }

        bottomContainer: WizardJobStatus{
            state: root.currentState

            progressContainer: Text{
                text: 'Container to show progress';
                anchors.top: parent.top
                anchors.topMargin: 30
            }
        }

        sideBarContainer: Item
        {
            width: parent.width
            height: purgeCellWizard.height

            WizardSideBar{

                stage: root.currentState

                editParamsPressed: ( function(){ purgeCellWizard.paramDialogId.open() } )
                startPressed: ( function(){ console.log('Start was pressed') } )
                cancelPressed: ( function(){ purgeCellWizard.cancelDialogId.open() } )
                nextPressed: ( function(){ console.log('Next stage was pressed') } )
            }
        }

        paramDialogContainer: Column
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

        paramDialogOnAccepted: (function() { console.log("Update params") })

        paramDialogOnRejected: (function() { console.log("Reset component values") })

        cancelDialogOnAccepted: (function() { console.log("Cancel the current mode") })
    }

}
