import QtQuick 2.2

Rectangle {
    id: item
    property string display
    property bool dropEnabled: false
    //color: dropArea.containsDrag ? "#CFC" : "#EEE"
    color: "#EEE"
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
}
