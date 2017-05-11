import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3


Item
{
    Item{
        id: tabController
        width: (parent.width) / 2
        anchors.left: parent.left
        TabBar {
            id: testingModeTabBar
            width: parent.width
            TabButton {
                text: qsTr("Valves")
            }
            TabButton {
                text: qsTr("Vacuum")
            }
            TabButton {
                text: qsTr("Flow")
            }
            TabButton {
                text: qsTr("Expansion")
            }
        }

        StackLayout {
            width: parent.width
            currentIndex: testingModeTabBar.currentIndex
            anchors.top: testingModeTabBar.bottom
            anchors.topMargin: 20
            anchors.left: testingModeTabBar.left
            anchors.leftMargin: 10
            Item {
                id: valveTab
                Valve{}
            }
            Item {
                id: vacuumTab
                Vacuum{}
            }
            Item {
                id: flowTab
                FlowController{}
            }
            Item {
                id: expansionTab
                Expansion{}
            }
        }
    }
    Rectangle {
        id: splitborder
        anchors.left: tabController.right
        width: 6
        height: parent.height
        color: Material.color(Material.Grey, Material.Shade300)
    }
    Rectangle{
        color: Material.color(Material.Grey, Material.Shade200)
        width: ((parent.width) / 2) - 6
        height: parent.height
        anchors.right: parent.right
        anchors.top: parent.top
        RigStatus{
            width: ((parent.width)) - 20
            height: parent.height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 20
        }
    }
}
