import QtQuick 2.2

Rectangle {
    id: item
    property string display
    property bool dropEnabled: true
    color: dropArea.containsDrag ? "#CFC" : "#EEE"
    ColorAnimation on color {
        id: rejectAnimation
        from: "#FCC"
        to: "#EEE"
        duration: 1000
    }
    Text {
        anchors.fill: parent
        text: item.display
        wrapMode: Text.WordWrap
    }
    DropArea {
        id: dropArea
        anchors.fill: parent
        keys: ["text/plain"]
        onEntered: if (!item.dropEnabled) {
            drag.accepted = false
            rejectAnimation.start()
        }
        onDropped: if (drop.hasText && item.dropEnabled) {
            if (drop.proposedAction == Qt.MoveAction || drop.proposedAction == Qt.CopyAction) {
                item.display += drop.text
                drop.acceptProposedAction()
            }
        }
    }
}
