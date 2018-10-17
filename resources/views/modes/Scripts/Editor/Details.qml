import QtQuick 2.0
import QtQuick.Controls 2.1
import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import QtQuick.Controls.Material 2.0
import Fluid.Core 1.0 as FluidCore
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.2

FluidControls.Card {
    id: item

    height: 80
    width: parent.width

    RowLayout
    {
        spacing: 10
        anchors.margins: 10
        anchors.fill: parent
        ColumnLayout
        {
            spacing: 5
            Layout.maximumWidth: 250
            Text {
                anchors.fill: parent
                text: "Name"
                wrapMode: Text.WordWrap
            }
            TextField {
                Layout.fillWidth: true
                placeholderText: qsTr("Enter name of script")
            }
        }

        ColumnLayout
        {
            spacing: 5
            Layout.maximumWidth: 450
            Text {
                anchors.fill: parent
                text: "Description"
                wrapMode: Text.WordWrap
            }
            TextField{
                Layout.fillWidth: true
                placeholderText: qsTr("Enter description of script's functionality")
            }
        }

        Button
        {
            text: qsTr("Save Script")
            enabled: true
            visible: true
            width: 150
            Material.background: Material.color(Material.Red, Material.Shade500)
            Material.foreground:  Material.color(Material.Grey, Material.Shade100)
            onClicked:
            {

            }
        }

        Button
        {
            text: qsTr("Cancell")
            enabled: true
            visible: true
            width: 150
            Material.background: Material.color(Material.Blue, Material.Shade500)
            Material.foreground:  Material.color(Material.Grey, Material.Shade100)
            onClicked:
            {

            }
        }
    }
}
