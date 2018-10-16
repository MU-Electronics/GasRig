import QtQuick 2.2

Rectangle {
    id: item
    property string display
    property alias dropEnabled: acceptDropCB.checked
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
        onEntered: if (!acceptDropCB.checked) {
            drag.accepted = false
            rejectAnimation.start()
        }
        onDropped: if (drop.hasText && acceptDropCB.checked) {
            if (drop.proposedAction == Qt.MoveAction || drop.proposedAction == Qt.CopyAction) {
                item.display += drop.text
                drop.acceptProposedAction()
            }
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: draggable
    }
    Item {
        id: draggable
        anchors.fill: parent
        Drag.active: mouseArea.drag.active
        Drag.hotSpot.x: 0
        Drag.hotSpot.y: 0
        Drag.mimeData: { "text/plain": item.display }
        Drag.dragType: Drag.Automatic
        // Drag.onDragFinished: if (dropAction == Qt.MoveAction) item.display = ""
    }
    CheckBox {
        id: acceptDropCB
        anchors.right: parent.right
        checked: true
        text: "accept drop"
    }
}
