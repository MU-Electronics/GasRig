import QtQuick 2.0

Rectangle
{
    color: "#d4d5d8"
    border.color: "#c8cace"
    border.width: 5
    width: 145
    height: 60

    Text{
        text: "Pressure Sensor"
        color: "#5e5f63"
        font.bold: true
        font.pixelSize: 15
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 12
    }

    Text{
        text: SystemStatusManager.pressureSensor["pressure_round_3"]+ " Bar"
        width: 145
        horizontalAlignment: Text.AlignHCenter
        color: "#5e5f63"
        anchors.top: parent.top
        anchors.topMargin: 25
    }

}
