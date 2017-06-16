import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore

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
                text: qsTr("Flow")
            }
            TabButton {
                text: qsTr("Expansion")
            }
        }

        StackLayout {
            id: stackViewTesting
            width: parent.width
            currentIndex: testingModeTabBar.currentIndex
            anchors.top: testingModeTabBar.bottom
            anchors.topMargin: 20
            anchors.left: testingModeTabBar.left
            anchors.leftMargin: 10
            Item {
                id: valveTab
                width: parent.width
                Valve{}
            }
            Item {
                id: vacuumTab
                width: parent.width
                Vacuum{}
            }
            Item {
                id: flowTab
                width: parent.width
                FlowController{}
            }
            Item {
                id: expansionTab
                width: parent.width
                Expansion{}
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
            width: ((parent.width)) - 20
            height: parent.height
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 20
        }
    }

    FluidControls.InfoBar
    {
        id: testingTerminalBar
        duration: 5000
    }
}
