import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2

import '../../../parts'

FluidControls.Card {
    id: item

    height: script_basic_info_alert.height + script_basic_info.height + 20
    width: parent.width

    property int listFunctionCount: 0

    Connections {
        target: ScriptAddManager
        onFunctionAdded: {
            item.listFunctionCount++;
        }
        onFunctionRemoved: {
            listFunctionCount--;
        }
    }

    AlertBox
    {
        id: script_basic_info_alert
        width: parent.width
        type: "Warning"
        textContent: "Unable to save script, try a different name."
        hide: true
    }

    RowLayout
    {
        id: script_basic_info
        spacing: 10
        anchors.top: script_basic_info_alert.bottom
        height: 60
        width: parent.width - 40

        ColumnLayout
        {
            spacing: 5
            Layout.maximumWidth: 250
            Layout.leftMargin: 20
            Layout.topMargin: 10
            Text {
                anchors.fill: parent
                text: "Name"
                wrapMode: Text.WordWrap
            }
            TextField {
                id: set_scriptname
                Layout.fillWidth: true
                placeholderText: qsTr("Enter name of script")
            }
        }

        ColumnLayout
        {
            spacing: 5
            Layout.maximumWidth: 550
            Layout.topMargin: 10
            Text {
                anchors.fill: parent
                text: "Description"
                wrapMode: Text.WordWrap
            }
            TextField{
                id: set_description
                Layout.fillWidth: true
                placeholderText: qsTr("Enter description of script's functionality")
            }
        }

        Button
        {
            Layout.topMargin: 10
            text: qsTr("Save Script")
            enabled: (set_scriptname.text && set_description.text && item.listFunctionCount !== 0)
            visible: true
            width: 150
            Material.background: Material.color(Material.Red, Material.Shade500)
            Material.foreground:  Material.color(Material.Grey, Material.Shade100)
            onClicked:
            {
                var scriptAddResult = ScriptAddManager.save(set_scriptname.text, set_description.text);

                // Lazy! check if success
                if(scriptAddResult)
                {
                    stackView.replace("qrc:/views/modes/Scripts/Index.qml");
                }
                else
                {
                    script_basic_info_alert.hide = false;
                }
            }
        }
    }
}
