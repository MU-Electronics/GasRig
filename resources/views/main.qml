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
    // Id for application window
    id: window

    // Ensure there is no frame around the program
    flags: Qt.FramelessWindowHint

    // Ensure the window is visable
    visible: true

    // Ensure a minimum width and height
    minimumWidth: 1280
    minimumHeight: 725

    // Ensure we are in full screen mode
    visibility: "Maximized"

    // Title for the program
    title: qsTr("Electronic Services: Gas Rig Controller")

    appBar.maxActionCount: 1

    // Set the default colours
    Material.primary: Material.LightBlue
    Material.accent: Material.Blue
    Universal.accent: Universal.Cobalt

    // Draw the title bar
    initialPage: TitleBar{}

    // Draw the side bar
    SideBar{ id: listPane }

    // Draw the stack / main view
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
