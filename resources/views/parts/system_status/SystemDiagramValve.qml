import QtQuick 2.0
import QtQuick.Controls 2.1


Rectangle
{
    id: valve

    property int number: 1
    property bool state: SystemStatusManager.valveState[valve.number]

    width: 50
    height: 50
    color: (state ? '#F44336' : '#4CAF50')
    border.color: (state? '#D32F2F' : '#388E3C')
    border.width: 5
    radius: 25

    Text{
        text: "V"+number
        color: "white"
        font.bold: true
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 17
    }

    Text{
        text: (state ? 'open' : 'closed')
        color: "white"
        font.bold: true
        font.pixelSize: 11
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 22
        anchors.left: parent.left
        anchors.leftMargin: (state ? '11' : '6')
    }
}
