import QtQuick 2.0

import QtQuick.Controls 2.1

Item {
    width: parent.width
    height: parent.height
    Flickable {
        width: parent.width
        height: parent.height
        contentHeight:messageList.height + 50
        contentWidth: parent.width
        anchors.fill: parent
        Text{
            id: messageList
            width: parent.width - 20
            textFormat: Text.RichText
            text: SystemStatusManager.debugMessages;
            wrapMode: Text.Wrap
        }
    }
}
