import QtQuick 2.9
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import "Editor" as Editor

Item
{
    id: root

    //width: parent.width
    // height: parent.height


    TabBar {
        id: tabbar
        width: parent.width
        z: 5

        TabButton {
            text: qsTr("List Scripts")
        }
        TabButton {
            text: qsTr("Add New Script")
        }
    }

    StackLayout {
        id: stackViewTesting
        width: parent.width
        height: parent.height
        currentIndex: tabbar.currentIndex
        anchors.top: tabbar.bottom
        anchors.topMargin: 20

        Loader {
            id: listScripts
            width: parent.width
            source: "ListScripts.qml"
            active: (tabbar.currentIndex === 0) ? true : false
            asynchronous: true
            visible: (status == Loader.Ready && tabbar.currentIndex == 0) ? true : false
            onLoaded: {
//                item.anchors.top = tabbar.bottom
//                item.anchors.topMargin = 20
                item.height = parent.height
                item.width = parent.width
            }
        }

        Loader {
            id: addScripts
            width: parent.width
            height: parent.height
            source: "Editor/Index.qml"
            active: (tabbar.currentIndex === 1) ? true : false
            asynchronous: true
            visible: (status == Loader.Ready && tabbar.currentIndex == 1) ? true : false
            onLoaded: {
//                item.anchors.top = tabbar.bottom
//                item.anchors.topMargin = 20
                item.height = parent.height
                item.width = parent.width
            }
        }
    }
}
