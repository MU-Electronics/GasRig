import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore

// Import tabs
import "Valve" as Valve

Item
{
    // Show reply to requests in the terminal panel
    function replyToTerminal(reply)
    {
        testingTerminalBar.open(reply)
    }

    // Connect all replies to terminal bar function
    Connections
    {
        target: TestingManager
        onEmit_testingMaintenanceReply: {
          testingTerminalBar.open(reply)
        }
    }

    Item
    {
        id: tabController
        width: (parent.width) / 2
        anchors.left: parent.left

        TabBar {
            id: testingModeTabBar
            width: parent.width
            z: 5

            TabButton {
                text: qsTr("Valves")
            }
            TabButton {
                text: qsTr("Vacuum")
            }
            TabButton {
                id: flowTabButton
                text: qsTr("Flow")
                opacity: GlobalManager.rigSettings["valve_8"]
                enabled: GlobalManager.rigSettings["valve_8"]

                Component.onCompleted: {
                    if(!GlobalManager.rigSettings["valve_8"])
                        flowTabButton.width = 0
                }
            }
            TabButton {
                text: qsTr("Expansion")
            }
        }

        BusyIndicator {
            id: rigStatusLoading
            running: ((valveTab.status == Loader.Loading)||(vacuumTab.status == Loader.Loading)||(flowTab.status == Loader.Loading)||(expansionTab.status == Loader.Loading)) ? true : false
            width: 100
            height: 100
            anchors.top: testingModeTabBar.bottom
            anchors.topMargin: ((window.height-150) / 2) - 50
            anchors.left: testingModeTabBar.left
            anchors.leftMargin: (parent.width / 2) - 50
            visible: ((valveTab.status == Loader.Loading)||(vacuumTab.status == Loader.Loading)||(flowTab.status == Loader.Loading)||(expansionTab.status == Loader.Loading)) ? true : false
        }

        StackLayout {
            id: stackViewTesting
            width: parent.width
            currentIndex: testingModeTabBar.currentIndex
            anchors.top: testingModeTabBar.bottom
            anchors.topMargin: 20
            anchors.left: testingModeTabBar.left
            anchors.leftMargin: 10

            Loader {
                id: valveTab
                width: parent.width
                source: "Valve/Index.qml"
                active: (testingModeTabBar.currentIndex === 0) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && testingModeTabBar.currentIndex == 0) ? true : false
                onLoaded: {
                    item.width = stackViewTesting.width - 10
                }
            }
            Loader {
                id: vacuumTab
                width: parent.width
                source: "Vacuum.qml"
                active: (testingModeTabBar.currentIndex === 1) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && testingModeTabBar.currentIndex == 1) ? true : false
                onLoaded: {
                    item.width = stackViewTesting.width - 10
                }
            }
            Loader {
                id: flowTab
                width: parent.width
                source: "FlowController.qml"
                active: (testingModeTabBar.currentIndex === 2) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && testingModeTabBar.currentIndex == 2) ? true : false
                onLoaded: {
                    item.width = stackViewTesting.width - 10
                }
            }
            Loader {
                id: expansionTab
                width: parent.width
                source: "Expansion.qml"
                active: (testingModeTabBar.currentIndex === 3) ? true : false
                asynchronous: true
                visible: (status == Loader.Ready && testingModeTabBar.currentIndex == 3) ? true : false
                onLoaded: {
                    item.width = stackViewTesting.width - 10
                }
            }
        }
    }
    Rectangle
    {
        id: splitborder
        anchors.left: tabController.right
        width: 6
        height: parent.height
        color: Material.color(Material.Grey, Material.Shade300)
    }
    Rectangle
    {
        color: Material.color(Material.Grey, Material.Shade200)
        width: ((parent.width) / 2) - 6
        height: parent.height
        anchors.right: parent.right
        anchors.top: parent.top
        RigStatus{
            width: ((parent.width)) //- 20
            height: parent.height
            anchors.right: parent.right
            anchors.top: parent.top
            //anchors.topMargin: 20
        }
    }

    FluidControls.InfoBar
    {
        id: testingTerminalBar
        duration: 5000
    }
}
