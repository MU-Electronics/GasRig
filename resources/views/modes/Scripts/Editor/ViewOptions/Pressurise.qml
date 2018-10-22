import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0

Item
{
    id: root

    width: parent.width - 15
    height: parent.height

    property var options
    property int rowheight: 15

    Column
    {
        width: parent.width
        height: a.height + b.height + c.height

        anchors.top: root.top
        spacing: 0

        Row {
            id: a
            width: root.width
            height: root.rowheight
            spacing: 10

            Rectangle
            {
                width: 135
                height: root.rowheight
                Text {
                    text: qsTr("Pressure: " + options["pressure"] + " mBar")
                    color: "#777777"
                    leftPadding: 5
                    font.pixelSize: 13
                    verticalAlignment : Text.AlignVCenter
                    horizontalAlignment : Text.AlignLeft
                }
            }
            Rectangle
            {
                width: 200
                height: root.rowheight
                // color: "#dddddd"
                Text {
                    text: (options["inputValve"] === true) ? "Input: V7 (High Pressure In)" : "Input: V9 (Nitrogen In)"
                    color: "#777777"
                    leftPadding: 5
                    font.pixelSize: 13
                    verticalAlignment : Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }
            }
        }

        Row
        {
            id: b
            spacing: 5
            width: parent.width
            height: root.rowheight
            Text {
                text: {
                    if(options["openOutputValve"] === true)
                        return "Output (V1) enabled with a step size of " + options["stepSize"] + " mbar";

                    return "Output (V1) disabled"
                }
                leftPadding: 5
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: root.rowheight
                width: 150
            }
        }

        Row
        {
            id: c
            spacing: 5
            width: parent.width
            height: root.rowheight
            Text {
                text: (options["initVacDown"] === true) ? "Initial vac down enabled" : "Initial vac down disabled"
                color: "#777777"
                leftPadding: 5
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: root.rowheight
                width: 150
            }
        }
    }
}
