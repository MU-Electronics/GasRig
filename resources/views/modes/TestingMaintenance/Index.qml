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
            id: bar
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
            currentIndex: bar.currentIndex
            Item {
                id: homeTab
            }
            Item {
                id: discoverTab
            }
            Item {
                id: activityTab
            }
        }
    }
    Rectangle {
        id: splitborder
        anchors.left: tabController.right
        width: 6
        height: parent.height
        color: Material.color(Material.Grey, Material.Shade200)
    }
    Item{
        width: ((parent.width) / 2) - 26
        anchors.right: parent.right
        Text{ text: "Hey" }
    }
}
