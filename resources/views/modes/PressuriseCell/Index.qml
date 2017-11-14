import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3

import Fluid.Controls 1.0 as FluidControls
import Fluid.Core 1.0 as FluidCore

import '../../parts'

Item
{


    Item
    {
        id: tabController
        width: (parent.width) / 2
        anchors.left: parent.left

        Purge{
            id: purgeContainer
            anchors.top: parent.top
            anchors.topMargin: 25
            shouldEnable: true
        }
        VacDown{
            anchors.top: purgeContainer.bottom
            anchors.topMargin: 25
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
        Item {
            width: ((parent.width)) //- 20
            height: parent.height
            anchors.right: parent.right
            anchors.top: parent.top
            View_Status{  }
        }
    }

    FluidControls.InfoBar
    {
        id: pressuriseCellTerminalBar
        duration: 5000
    }
}

