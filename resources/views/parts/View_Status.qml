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

        BusyIndicator {
            id: rigStatusLoading
            running: ((systemStatus.status == Loader.Loading)||(vacuumGraph.status == Loader.Loading)||(pressureGraph.status == Loader.Loading)||(extraTab.status == Loader.Loading)) ? true : false
            width: 100
            height: 100
            anchors.top: statusTabBar.bottom
            anchors.topMargin: ((window.height-150) / 2) - 50
            anchors.left: statusTabBar.left
            anchors.leftMargin: (parent.width / 2) - 50
            visible: ((systemStatus.status == Loader.Loading)||(vacuumGraph.status == Loader.Loading)||(pressureGraph.status == Loader.Loading)||(extraTab.status == Loader.Loading)) ? true : false
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

            Loader {
                id: systemStatus
                width: parent.width
                height: parent.height
                source: "system_status/SystemDiagram.qml"
                active: (statusTabBar.currentIndex === 0) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && statusTabBar.currentIndex == 0) ? true : false
                onLoaded: {
                    item.width = stackViewSystem.width
                    item.height = stackViewSystem.height
                }
            }
            Loader {
                id: vacuumGraph
                width: parent.width
                height: parent.height
                source: "system_status/VacuumDiagram.qml"
                active: (statusTabBar.currentIndex === 1) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && statusTabBar.currentIndex == 1) ? true : false
                onLoaded: {
                    item.width = stackViewSystem.width
                    item.height = stackViewSystem.height
                }
            }
            Loader {
                id: pressureGraph
                width: parent.width
                height: parent.height
                active: (statusTabBar.currentIndex === 2) ? true : false
                //source: "system_status/VacuumDiagram.qml"
                asynchronous: true
                visible: (status == Loader.Ready && statusTabBar.currentIndex == 2) ? true : false
                onLoaded: {
                    item.width = stackViewSystem.width
                    item.height = stackViewSystem.height
                }
            }
            Loader {
                id: extraTab
                width: parent.width
                height: parent.height
                active: (statusTabBar.currentIndex === 3) ? true : false
                //source: "system_status/VacuumDiagram.qml"
                asynchronous: true
                visible: (status == Loader.Ready && statusTabBar.currentIndex == 3) ? true : false
                onLoaded: {
                    item.width = stackViewSystem.width
                    item.height = stackViewSystem.height
                }
            }
        }
    }



}

