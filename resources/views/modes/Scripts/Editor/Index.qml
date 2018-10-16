import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0


import "../../../parts"

Item
{
    width: parent.width
    height: 500

    Text {
        id: title
        Layout.fillWidth: true
        text: "Create custom script"
        wrapMode: Text.WordWrap
    }

    Details{
        id: detailsContainer

        width: parent.width - 32

        anchors.top: title.bottom
        anchors.topMargin: 20

        anchors.left: parent.left
        anchors.leftMargin: 16
    }


    RowLayout {

        width: parent.width - 32
        height: 500

        spacing: 16

        anchors.left: parent.left
        anchors.leftMargin: 16

        anchors.top: detailsContainer.bottom
        anchors.topMargin: 20

        ColumnLayout{
            // Current functions
            ScriptList{

            }

            // Drag area
            FunctionDropArea {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                height: 142
            }

        }


        ColumnLayout
        {
            Layout.minimumWidth: 30
            Layout.preferredWidth: 300
            Layout.maximumWidth: 400
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop

            SysFunction {
                Layout.fillWidth: true
                height: 80
                display: "Set Pressure"
                dropEnabled: true
            }
            SysFunction {
                Layout.fillWidth: true
                height: 80
                display: "Set Valve"
                dropEnabled: true
            }
        }
    }
}

