import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3
import Fluid.Controls 1.0
import "template"
import "modes"
import "management"
import "management/TestingMaintenance"

ApplicationWindow {
    id: window

    //flags: Qt.FramelessWindowHint | Qt.Window

    visible: true

    // End app will be full screen
    //visibility: "FullScreen"
    //Easy of debugging use maximized
    minimumWidth: 1280
    //minimumHeight: 1024
    minimumHeight: 768 // my laptops a bit smaller :S
    //visibility: "Maximized"

    title: qsTr("JFET Services: Gas Rig")

    appBar.maxActionCount: 1

    Material.primary: Material.LightBlue
    Material.accent: Material.Blue

    Universal.accent: Universal.Cobalt

    initialPage: TitleBar{}


    SideBar{ id: listPane }


    StackView {
        id: stackView
        anchors {
            left: listPane.right
            top: parent.top
            right: parent.right
            bottom: parent.bottom
        }
        initialItem: ConnectionStatus{}
    }

}
