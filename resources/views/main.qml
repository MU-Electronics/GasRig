import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3
import Fluid.Controls 1.0

ApplicationWindow {
    id: window

    flags: Qt.FramelessWindowHint | Qt.Window

    visible: true

    visibility: "FullScreen"

    title: qsTr("JFET Services: Gas Rig")

    appBar.maxActionCount: 1

    Material.primary: Material.LightBlue
    Material.accent: Material.Blue

    Universal.accent: Universal.Cobalt

    initialPage: TabbedPage {
        title: window.title

        leftAction: Action {
            text: qsTr("JFET Services")
            iconSource: "qrc:/assets/images/favicon.svg"
        }

        actions: [
            Action {
                text: qsTr("Help")
                iconName: "action/help"
                tooltip: qsTr("Show a dummy error")
                onTriggered: console.log("Dummy error")
            },
            Action {
                text: qsTr("Settings")
                iconName: "action/settings"
                tooltip: qsTr("Settings")
                onTriggered: console.log("Settings clicked")
            },
            Action {
                text: qsTr("Close Application")
                iconName: "navigation/close"
                tooltip: qsTr("Pick a color")
                onTriggered: console.log("close")
            }
        ]

    }



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
                    listPane.enabled = false
                }
            }

            //ScrollBar.vertical: ScrollBar {}
        }
    }

    StackView {
        id: stackView
        anchors {
            left: listPane.right
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        initialItem: SelectMode{}
    }

}
