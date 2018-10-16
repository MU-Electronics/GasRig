import QtQuick 2.2

Item{
    id: item
    property bool dropEnabled: true
    property string draggedFunction: ""

    Rectangle{

        ColorAnimation on color {
            id: rejectAnimationBorder
            from: "#ffcece"
            to: "#eeeeee"
            duration: 1000
        }

        color: dropArea.containsDrag ? "#c4fcc4" : "#eeeeee"

        height: 100 + 16
        width: parent.width - 16

        Rectangle {
            id: dragArea

            height: 100
            width: parent.width - 16

            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.top: parent.top
            anchors.topMargin: 8

            color: dropArea.containsDrag ? "#d3ffd3" : "#f7f7f7" //cfc

            ColorAnimation on color {
                id: rejectAnimation
                from: "#ffe0e0"
                to: "#f7f7f7"
                duration: 1000
            }
            Text {
                anchors.fill: parent
                text: (item.draggedFunction === "") ? "Drag Function into Box" : ""
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 24;
                style: Text.Raised;
                styleColor: "#d6d6d6"
                font.weight: Font.DemiBold
                color: dropArea.containsDrag ? "#63a563" : "#b5b5b5"
            }
            Text {
                anchors.fill: parent
                text: item.draggedFunction
                wrapMode: Text.WordWrap
            }
            DropArea {
                id: dropArea
                anchors.fill: parent
                keys: ["text/plain"]
                onEntered: if (!item.dropEnabled) {
                    drag.accepted = false
                    rejectAnimation.start()
                    rejectAnimationBorder.start()
                    rejectAnimationText.start()
                }
                onDropped: if (drop.hasText && item.dropEnabled) {
                    if (drop.proposedAction == Qt.MoveAction || drop.proposedAction == Qt.CopyAction) {
                        item.draggedFunction = drop.text
                        drop.acceptProposedAction()
                    }
                }
            }
        }
    }
}

