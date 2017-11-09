import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Core 1.0 as FluidCore

Item{

    id: rootJobStatus

    property alias progressContainer: progressPlaceholder.data

    width: parent.width
    height: parent.height

    anchors.top: parent.top
    anchors.left: parent.left

    state: "0"

    states: [
        // Initial stage view
        State {
            name: "0"
            PropertyChanges { target: initialContainer; visible: true; }
            PropertyChanges { target: progressContainer; visible: false; }
        },
        // In progress view
        State {
            name: "1"
            PropertyChanges { target: initialContainer; visible: false; }
            PropertyChanges { target: progressContainer; visible: true; }
            PropertyChanges { target: progressTitle; title: qsTr('In Progress:'); titleSmall: qsTr('Please wait...')}
        },
        // Finished view
        State {
            name: "2"
            PropertyChanges { target: initialContainer; visible: false; }
            PropertyChanges { target: progressContainer; visible: true; }
            PropertyChanges { target: progressTitle; title: qsTr('Finished:'); titleSmall: qsTr('Select next action') }
        },
        // Error view
        State {
            name: "3"
            PropertyChanges { target: initialContainer; visible: false; }
            PropertyChanges { target: progressContainer; visible: true; }
            PropertyChanges { target: progressTitle; title: qsTr('Error:'); titleSmall: qsTr('Select next action') }
        }
    ]

    Item{
        id: initialContainer
        width: parent.width
        height: parent.height
        Text{
            id: initTitleText
            width: parent.width
            height: parent.height
            text: qsTr('Selection action to begin')
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 18
            bottomPadding: 40
        }
        Text{
            text: qsTr('Start to being the current stage or edit parameters first if required; click next stage to skip this step.')
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            height: parent.height
            topPadding: 35
            wrapMode: Text.WordWrap
        }
    }

    Item{
        id: progressContainer

        width: parent.width
        height: parent.height

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 10

        WizardContainerTitle{
            id: progressTitle
        }

        Item{
            id: progressPlaceholder

            width: parent.width
            height: parent.height
        }
    }
}
