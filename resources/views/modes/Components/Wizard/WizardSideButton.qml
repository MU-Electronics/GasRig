import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

Button {
    id: buttonRoot
    text: ""
    width: parent.width
    enabled: true

    property var backgroundInit: Material.color(Material.Blue, Material.Shade400)
    property var backgroundPressed: Material.color(Material.Blue, Material.Shade600)
    property var textColorInit: "#ffffff"
    property var textColorPressed: "#ffffff"

    property var backgroundCondtional: (buttonRoot.enabled) ? backgroundInit : Material.color(Material.Grey, Material.Shade400)

    contentItem: Label {
        text: buttonRoot.text
        font: buttonRoot.font
        verticalAlignment: Text.AlignVCenter
        color: buttonRoot.down ? buttonRoot.textColorPressed : buttonRoot.textColorInit
        horizontalAlignment: Text.AlignHCenter
    }
    background: Rectangle {
        width: parent.width
        implicitHeight: 40
        color: buttonRoot.down ? buttonRoot.backgroundPressed : buttonRoot.backgroundCondtional
    }
}

