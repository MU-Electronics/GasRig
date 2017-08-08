import QtQuick 2.0

Rectangle
{
    id: boxedText

    property string message
    property int texttop: 5
    property int textleft: 12

    color: "#d4d5d8"
    border.color: "#c8cace"
    border.width: 5

    Text{
        text: boxedText.message
        color: "#5e5f63"
        font.bold: true
        font.pixelSize: 15
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: boxedText.texttop
        anchors.left: parent.left
        anchors.leftMargin: boxedText.textleft
    }

}
