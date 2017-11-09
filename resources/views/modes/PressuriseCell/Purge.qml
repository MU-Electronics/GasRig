import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore
import Fluid.Layouts 1.0 as FluidLayouts

// Global part files
import "../../parts"

// Imnport wizard parts
import "../parts/wizard"

// Import purge defaults
import "../parts/purge"

Item {

    id: root

    width: parent.width
    height: purgeCellWizard.height

    anchors.left: parent.left;
    anchors.leftMargin: 15

    // Is this the current stage?
    property bool shouldEnable: false

    // What state are we at during this stage?
    property string currentState: "0"

    WizardContainer{
        id: purgeCellWizard

        title: "Stage One: Purge Cell"

        stage: root.currentState
        shouldEnable: root.shouldEnable

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

            progressContainer: PurgeProgress{}
        }

        sideBarContainer: Item
        {
            width: parent.width
            height: purgeCellWizard.height

            WizardSideBar{
                stage: root.currentState

                editButton.onClicked: purgeCellWizard.paramDialogId.open()
                startButton.onClicked: console.log('Start was pressed')
                cancelButton.onClicked: purgeCellWizard.cancelDialogId.open()
                nextButton.onClicked: console.log('Next stage was pressed')
            }
        }

        paramDialogContainer: PurgeParamForm{
            id: paramForm

            // Set defaults
            numberCycles.value: 5
            vacBelow.value: 50
            useNitrogen.checked: true
            nitrogenPressure.value: 2.78
        }

        paramDialogOnAccepted: (function() { console.log("Update params e.g number cycles: " + paramForm.numberCycles.value) })

        paramDialogOnRejected: (function() { console.log("Reset component values") })

        cancelDialogOnAccepted: (function() { console.log("Cancel the current mode") })
    }

}
