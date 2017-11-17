import QtQuick 2.6
import QtQuick.Controls 2.1
import QtQuick.Controls.Material 2.0

Column
{
    width: parent.width
    height: parent.height

    property alias numberCycles: setNumberCycles_pressureCell
    property alias vacBelow: setVacBelow_pressureCell
    property alias useNitrogen: setUseNitrogen_pressureCell
    property alias nitrogenPressure: setNitrogenPressure_pressureCell

    // Vac below param
    Row
    {
        spacing: 10
        width: parent.width
        height: 50
        Text {
            text: qsTr("Vac Below: ")
            color: "#595959"
            visible: parent.opacity
            font.pixelSize: 16
            verticalAlignment : Text.AlignVCenter
            height: parent.height
        }
        Slider
        {
            id: setVacBelow_pressureCell
            width: parent.width - 220
            from: 5
            value: 10
            to: 200
        }
        Text{
            text: setVacBelow_pressureCell.value.toFixed(2) + " " + qsTr("mBar")
            anchors.top: parent.top
            anchors.topMargin: 10
            font.pointSize: 16
            color: "#595959"
        }
    }
    // Should be nitrogen param
    Row{
        spacing: 10
        width: parent.width
        height: 50
        Text {
            text: qsTr("Use Nitrogen: ")
            color: "#595959"
            visible: parent.opacity
            font.pixelSize: 16
            verticalAlignment : Text.AlignVCenter
            height: parent.height
        }
        CheckBox {
            id: setUseNitrogen_pressureCell
            checked: false
        }
    }
    // What nitrogen pressure param
    Row{
        spacing: 10

        width: parent.width
        anchors.left: parent.left;
        anchors.leftMargin: 50

        opacity: (setUseNitrogen_pressureCell.checked) ? 1 : 0;
        height: (setUseNitrogen_pressureCell.checked) ? 50 : 0;

        Text {
            text: qsTr("Nitrogen Pressure: ")
            color: "#595959"
            visible: parent.opacity
            font.pixelSize: 16
            verticalAlignment : Text.AlignVCenter
            height: parent.height
        }
        Slider
        {
            id: setNitrogenPressure_pressureCell
            width: parent.width - 380
            height: parent.height
            from: 0.5
            value: 2
            to: 10
        }
        Text{
            text: setNitrogenPressure_pressureCell.value.toFixed(2) + " " + qsTr("Bar")
            anchors.top: parent.top
            anchors.topMargin: 10
            font.pointSize: 16
            color: "#595959"
            height: parent.height
        }
    }
    // How many cyles param
    Row{
        spacing: 10
        width: parent.width
        height: 50

        Text {
            text: qsTr("Number of Cycles: ")
            color: "#595959"
            visible: parent.opacity
            font.pixelSize: 16
            verticalAlignment : Text.AlignVCenter
            height: parent.height
        }
        SpinBox {
            id: setNumberCycles_pressureCell
            value: 20
            from: 1
            to: 1000
            stepSize: 1
        }
    }
}
