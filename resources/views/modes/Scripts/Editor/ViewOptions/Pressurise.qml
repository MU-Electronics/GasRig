import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2

Item
{
    id: root

    width: parent.width
    height: parent.height

    property var options

    ColumnLayout
    {
        width: parent.width
        Layout.minimumHeight: a.height + b.height + c.height

        anchors.top: root.top
        spacing: 0

        RowLayout{
            id: a
            width: parent.width
            Layout.maximumHeight: 45
            spacing: 5

            Layout.alignment: Qt.AlignTop

            Text {
                text: qsTr("Pressure (mBar): " + options["pressure"])
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                horizontalAlignment : Text.AlignRight
                height: parent.height
            }
        }

        RowLayout
        {
            id: b
            spacing: 5
            width: parent.width
            Layout.maximumHeight: 45
            Layout.alignment: Qt.AlignTop
            Text {
                text: (options["openOutputValve"] === true) ? "Output Enabled" : "Output Disabled"
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: 150
            }
            Text {
                text: qsTr("Valve 1 step size: " + options["stepSize"] + " mbar" )
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: 150
                visible: (setUseVac_openOutputValve.checked) ? true : false
            }
        }

        RowLayout
        {
            id: c
            spacing: 5
            width: parent.width
            Layout.maximumHeight: 45
            Layout.alignment: Qt.AlignTop
            Text {
                text: (options["inputValve"] === true) ? "Input: High Pressure" : "Input: Nitrogren"
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                Layout.minimumWidth: 60
            }
            Text {
                text: (options["initVacDown"] === true) ? "Vac down enabled" : "Vac down disabled"
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: 150
            }
        }
    }
}
