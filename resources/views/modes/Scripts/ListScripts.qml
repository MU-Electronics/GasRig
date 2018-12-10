import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2

import '../../parts'

Item
{
    id: root

    width: parent.width
    height: parent.height

    Component.onCompleted: {
        ScriptEditorManager.refresh();
    }

    Column
    {
        anchors.fill: parent
        id : mainContainer

        ListView {
            width: parent.width
                   height: parent.height

            model: ScriptInfoObject

            AlertBox
            {
                id: script_basic_info_alert
                width: parent.width - 40
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.topMargin: 20
                type: "Warning"
                hide: (parent.count === 0) ? false : true;
                textContent: "There are no saved scripts in the system."

            }

            delegate: Item {
                height: 25
                width: 100
                Text { text: model.modelData.name + " " + model.modelData.desc }

                MouseArea {
                    anchors.fill: parent
                    onClicked: { ScriptEditorManager.refresh(); }
                }
            }
        }
    }
}
