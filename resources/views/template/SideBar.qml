import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3
import Fluid.Controls 1.0
import "../modes"

Pane {
    id: listPane


    Connections {
        target: GlobalManager
        onCurrentNavigationChanged: {
            // Removed hightling of old page
            if(GlobalManager.currentNavigation == 1)
            {
                connectionTab.highlighted = false
                modeNavigation.currentIndex = -1
            }
        }
    }

    property int currentNavigationWindow: 2

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
            modeNavigation.currentIndex = -1

            //Add highligh to this tab
            connectionTab.highlighted = true

            // Push the view update
            stackView.push("qrc:/views/management/ConnectionStatus.qml")

            // Update current nav
            GlobalManager.currentNavigation = 2;
        }
    }

    ListView {
        id: modeNavigation
        anchors.fill: parent
        anchors.topMargin: 50
        currentIndex: -1
        enabled: ConnectionStatusManager.hardwareConnection["all_connections"];
        model: ListModel {
            ListElement { title: qsTr("Pressurise Cell"); source: "qrc:/views/modes/PressuriseCell.qml" }
            ListElement { title: qsTr("De-Pressurise Cell"); source: "qrc:/views/modes/DepressuriseCell.qml" }
            ListElement { title: qsTr("Vacuum Station"); source: "qrc:/views/modes/VacuumStation.qml" }
            ListElement { title: qsTr("Change Gas"); source: "qrc:/views/modes/ChangeGas.qml" }
            ListElement { title: qsTr("Cycle Pressure"); source: "qrc:/views/modes/CyclePressure.qml" }
            ListElement { title: qsTr("Continuous Flow"); source: "qrc:/views/modes/ContinuousFlow.qml" }
            ListElement { title: qsTr("Remote Control"); source: "qrc:/views/modes/RemoteControl.qml"  }
        }
        header: Subheader {
            text: qsTr("Select Mode")
        }
        delegate: ListItem {
            text: model.title
            highlighted: ListView.isCurrentItem
            //anchors.left: parent.left
            //anchors.leftMargin: 25
            onClicked: {
                //Remove highligh from connection status
                connectionTab.highlighted = false

                // Add highligh to mode
                modeNavigation.currentIndex = index

                // Push the view update
                stackView.push(model.source)

                // Update current nav
                GlobalManager.currentNavigation = 2;
            }
        }
        ScrollBar.vertical: ScrollBar {}
    }
}
