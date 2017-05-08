import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3
import Fluid.Controls 1.0

TabbedPage {
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
                onTriggered: Qt.quit();
            }
        ]

    }
