import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3
import Fluid.Controls 1.0
import "../modes"

Pane {
    id: listPane
    anchors {
        left: parent.left
        top: parent.top
        bottom: parent.bottom
    }
    width: 200
    padding: 0
    z: 2

    Material.background: "white"
    Material.elevation: 1

    Universal.background: Universal.accent

    ListItem {
        id: connectionTab
        text: qsTr("Connection status")
        highlighted: true
        onClicked: {
            // Remove highligh from mode selection
            listView.currentIndex = -1

            //Add highligh to this tab
            connectionTab.highlighted = false

            // Push the view update
            stackView.push("qrc:/views/modes/ConnectionStatus.qml")

            //listPane.enabled = false
        }
    }

    ListView {
        id: listView
        anchors.fill: parent
        anchors.topMargin: 50
        currentIndex: -1
        model: ListModel {
            ListElement { title: qsTr("Pressurise Cell"); source: "qrc:/views/modes/PressuriseCell.qml" }
            ListElement { title: qsTr("De-Pressurise Cell"); source: "qrc:/views/modes/DepressuriseCell.qml" }
            ListElement { title: qsTr("Cycle Pressure"); source: "qrc:/views/modes/CyclePressure.qml" }
            ListElement { title: qsTr("Continuous Flow"); source: "qrc:/views/modes/ContinuousFlow.qml"  }
            ListElement { title: qsTr("Vacuum Station"); source: "qrc:/views/modes/VacuumStation.qml" }
            ListElement { title: qsTr("Custom Experiment"); source: "qrc:/views/modes/CustomExperiment.qml" }
            ListElement { title: qsTr("Safe Manual Control"); source: "qrc:/views/modes/SafeManualControl.qml" }
            ListElement { title: qsTr("Remote Control"); source: "qrc:/views/modes/RemoteControl.qml"  }
            ListElement { title: qsTr("Testing Mode"); source: "qrc:/views/modes/TestingMaintenance/Index.qml"  }
        }
        header: Subheader {
            text: qsTr("Select Mode")
        }
        delegate: ListItem {
            text: model.title
            highlighted: ListView.isCurrentItem
            anchors.left: parent.left
            anchors.leftMargin: 25
            onClicked: {
                //Remove highligh from connection status
                connectionTab.highlighted = false

                // Add highligh to mode
                listView.currentIndex = index

                // Push the view update
                stackView.push(model.source)
                //listPane.enabled = false
            }
        }
        ScrollBar.vertical: ScrollBar {}
    }
}
