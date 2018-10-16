import QtQuick 2.2

Rectangle {
    id: item
    color: "#EEE"

    height: 50
    width: parent.width

    Text {
        anchors.fill: parent
        text: "@todo some form to fill in general details of script"
        wrapMode: Text.WordWrap
    }

}
