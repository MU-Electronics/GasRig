import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore

import "system_status"


Item
{
    width: parent.width
    height: parent.height
    Item
    {
        id: tabController
        width: (parent.width)
        height: parent.height
        anchors.left: parent.left

        TabBar {
            id: statusTabBar
            width: parent.width
            z: 5

            TabButton {
                text: qsTr("System")
            }
            TabButton {
                text: qsTr("Vacuum")
            }
            TabButton {
                text: qsTr("Pressure")
            }
            TabButton {
                text: qsTr("Extra")
            }
        }

        StackLayout {
            id: stackViewSystem
            width: parent.width - 10
            height: parent.height
            currentIndex: statusTabBar.currentIndex
            anchors.top: statusTabBar.bottom
            anchors.topMargin: 10
            anchors.left: statusTabBar.left
            anchors.leftMargin: 10
            Item {
                id: systemStatus
                width: (parent.width)
                height: parent.height
                SystemDiagram
                {
                    height: parent.height
                    width: (parent.width)
                    anchors.top: parent.top
                    anchors.topMargin: 20
                }
            }
            Item {
                id: vacuumGraph
                VacuumDiagram
                {
                    height: parent.height
                    width: (parent.width)
                    anchors.top: parent.top
                    anchors.topMargin: 20
                }
            }
            Item {
                id: pressureGraph
            }
            Item {
                id: extraTab
            }
        }
    }



}

