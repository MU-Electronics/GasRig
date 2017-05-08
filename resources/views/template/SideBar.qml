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

    ListView {
        id: listView
        anchors.fill: parent
        currentIndex: 0
        model: ListModel {
            ListElement { title: qsTr("Sealed Pressure"); source: "qrc:/views/SelectMode.qml" }
            ListElement { title: qsTr("Cycle Pressure"); source: "qrc:/views/SelectMode.qml" }
            ListElement { title: qsTr("Continuous Flow"); source: "qrc:/views/SelectMode.qml" }
            ListElement { title: qsTr("Vacuum Station"); source: "qrc:/views/modes/VacuumStation.qml" }
            ListElement { title: qsTr("Custom Experiment"); source: "qrc:/views/SelectMode.qml" }
            ListElement { title: qsTr("Safe Manual Control"); source: "qrc:/views/SelectMode.qml" }
            ListElement { title: qsTr("Remote Control"); source: "qrc:/views/SelectMode.qml" }
            ListElement { title: qsTr("Testing & Maintenance"); source: "qrc:/views/SelectMode.qml" }
        }
        header: Subheader {
            text: qsTr("Select Mode")
        }
        delegate: ListItem {
            text: model.title
            highlighted: ListView.isCurrentItem
            onClicked: {
                listView.currentIndex = index
                stackView.push(model.source)
                //listPane.enabled = false
            }
        }
        //ScrollBar.vertical: ScrollBar {}
    }
}
