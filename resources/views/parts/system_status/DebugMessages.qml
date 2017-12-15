import QtQuick 2.7

import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

Item {
    width: parent.width
    height: parent.height
    Rectangle{
        id: logLocation
        width: parent.width + 30
        height: logLocationText.height
        color: Material.color(Material.Grey, Material.Shade300)
        z: 2
        anchors.left: parent.left
        anchors.leftMargin: -10
        anchors.rightMargin: - 10
        anchors.top: parent.top
        anchors.topMargin: - 10
        Text{
            id: logLocationText
            text: "<b>Log Location:</b> " + SystemStatusManager.logLocation
            wrapMode: Text.Wrap
            textFormat: Text.RichText
            width: parent.width - 20
            padding: 10
        }
    }
    Flickable {
        width: parent.width
        height: parent.height - 20 - logLocation.height
        contentHeight:messageList.height + 50
        contentWidth: parent.width
        anchors.top: logLocation.bottom
        anchors.topMargin: 20
        Text{
            id: messageList
            width: parent.width - 20
            textFormat: Text.RichText
            text: SystemStatusManager.debugMessages;
            wrapMode: Text.Wrap
        }
    }
}
