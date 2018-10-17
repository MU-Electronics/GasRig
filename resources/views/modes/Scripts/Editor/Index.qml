import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2



import "../../../parts"

Item
{
    id: root

    width: parent.width
    height: parent.height

//    Text {
//        id: title
//        Layout.fillWidth: true
//        text: "Create custom script"
//        wrapMode: Text.WordWrap
//    }

    Details{
        id: detailsContainer

        width: root.width - 32

        anchors.top: title.bottom
        anchors.topMargin: 20

        anchors.left: parent.left
        anchors.leftMargin: 16
    }


    RowLayout {

        width: root.width - 32
        height: root.height - detailsContainer.height - title.height - 100

        spacing: 16

        anchors.left: parent.left
        anchors.leftMargin: 16

        anchors.top: detailsContainer.bottom
        anchors.topMargin: 20

        ColumnLayout{
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignTop
            // Current functions
            ScriptList{
                Layout.fillWidth: true
               // Layout.fillHeight: true
                Layout.alignment: Qt.AlignTop
                height: root.height - 195
            }
        }

        ColumnLayout
        {
            id: functionconfig

            Layout.minimumWidth: 600
            Layout.preferredWidth: 600
            Layout.maximumWidth: 600
            Layout.alignment: Qt.AlignTop

            FunctionDropArea {
                id: functiondrop
                Layout.fillWidth: true
                Layout.minimumHeight: 200
                Layout.preferredHeight: 200
                Layout.alignment: Qt.AlignTop
            }

            Flickable {
                id: functionflickable
                width: 400
                height: root.height - functiondrop.height - 400
                Layout.fillWidth: true
                Layout.fillHeight: true

                contentWidth: functionflickable.width - 20
                contentHeight: functionlist.height * 2

                focus: true
                clip: true

                Keys.onUpPressed: scrollBar.decrease()
                Keys.onDownPressed: scrollBar.increase()

                ScrollBar.vertical: ScrollBar {
                    id: vbar;
                    active: true
                    //anchors.left: parent.right
                    policy: ScrollBar.AlwaysOn
                }

                FunctionList
                {
                    id: functionlist
                }
            }
        }
    }
}

