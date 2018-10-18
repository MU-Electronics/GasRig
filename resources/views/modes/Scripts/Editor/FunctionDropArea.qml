import QtQuick 2.9

import "FunctionOptions" as FunctionOptions

Item{
    id: item
    property bool dropEnabled: true
    property string draggedFunction: ""
    property var loadedoption: null
    height: parent.height
    width: parent.width
    Rectangle{

        ColorAnimation on color {
            id: rejectAnimationBorder
            from: "#ffcece"
            to: "#eeeeee"
            duration: 1000
        }

        color: dropArea.containsDrag ? "#c4fcc4" : "#eeeeee"

        height: parent.height
        width: parent.width

        Rectangle {
            id: dragArea

            height: parent.height - 16
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
                font.pointSize: 18  ;
                style: Text.Raised;
                styleColor: "#d6d6d6"
                font.weight: Font.DemiBold
                color: dropArea.containsDrag ? "#63a563" : "#b5b5b5"
            }
            Item {
                id: optionContainer
                anchors.fill: parent
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
                    if (drop.proposedAction == Qt.MoveAction || drop.proposedAction == Qt.CopyAction)
                    {
                        // Turn box green
                        drop.acceptProposedAction()

                        // Create the relivant view
                        if(item.loadedoption != null)
                            item.loadedoption.destroy();

                        var component = Qt.createComponent("FunctionOptions/"+drop.text+".qml");
                        item.loadedoption = component.createObject(optionContainer);
                        // item.loadedoption.x = (item.loadedoption.width + 10) * i;
                        // item.draggedFunction = drop.text
                    }
                }
            }
        }
    }
}

