import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Universal 2.0
import QtQuick.Layouts 1.3
import Fluid.Controls 1.0
import QtQuick.Window 2.2
import "template"
import "modes"
import "management"
import "management/TestingMaintenance"

ApplicationWindow {
    id: window

    // Ensure there is no frame around the program
    flags: Qt.FramelessWindowHint | Qt.Window

    // Ensure the window is visable
    visible: true

    // We want a minmum width and height set
    minimumWidth: 1280
    minimumHeight: 1024

    // Ensure we are in full screen mode
    visibility: Window.FullScreen
    //visibility: "FullScreen"

    // Title for the program
    title: qsTr("Electronic Services: Gas Rig Controller")

    appBar.maxActionCount: 1

    // Set the default colours
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
