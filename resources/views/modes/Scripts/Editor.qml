import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0


import "../../parts"

Item
{
    width: parent.width
    height: 500

    ColumnLayout {

        anchors.fill: parent
        anchors.margins: 8

        Text {
            Layout.fillWidth: true
            text: "Drag text into, out of, and between the boxes below."
            wrapMode: Text.WordWrap
        }

        SysFunction {
            Layout.fillWidth: true
            height: 142
            display: "Sample Text"
        }

        SysFunction {
            Layout.fillWidth: true
            height: 142
            display: "Option/ctrl drag to copy instead of move text."
        }

        SysFunction {
            Layout.fillWidth: true
            height: 142
            dropEnabled: false
            display: "Drag out into other applications."
        }
    }
}

