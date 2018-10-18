import QtQuick 2.9
import QtQuick.Controls.Material 2.2


Rectangle {
    id: item
    property string display
    property string desc: ""
    property string functionOption: "NotSupported"
    property bool dropEnabled: false
    //color: dropArea.containsDrag ? "#CFC" : "#EEE"
    color: "#EEE"

    height: 80

    ColorAnimation on color {
        id: rejectAnimation
        from: "#FCC"
        to: "#EEE"
        duration: 1000
    }
    Rectangle{
        id: title
        width: parent.width
        height: 35
        color: "#e0e0e0"
        Text {
            anchors.fill: parent
            anchors.topMargin: 8
            text: item.display
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 11
            style: Text.Raised;
            styleColor: "#d6d6d6"
            font.weight: Font.DemiBold
            color: Material.color(Material.Grey, Material.Shade700)
        }
    }

    Text {
        anchors.fill: parent
        anchors.topMargin: 35
        height: item.height - title.height
        text: item.desc
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 9
        font.weight: Font.DemiBold
        color: Material.color(Material.Grey, Material.Shade800)
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
        Drag.mimeData: { "text/plain": item.functionOption }
        Drag.dragType: Drag.Automatic
        // Drag.onDragFinished: if (dropAction == Qt.MoveAction) item.display = ""
    }
}
