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

    ColumnLayout
    {
        width: parent.width
        Layout.minimumHeight: a.height + b.height + c.height + d.height

        anchors.top: root.top
        spacing: 0

        RowLayout{
            id: a
            width: parent.width
            Layout.maximumHeight: 45
            spacing: 5

            Layout.alignment: Qt.AlignTop

            Rectangle{
                color: "#eeeeee"
                height: 50
                width: 120
                Text{
                    text: "Pressuirse"
                    anchors.fill: parent
                    wrapMode: Text.WordWrap
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    font.pointSize: 12;
                    style: Text.Raised;
                    styleColor: "#d6d6d6"
                    font.weight: Font.DemiBold
                    color: "#606060"
                }
            }

            Text {
                text: qsTr("  Pressure (mBar): ")
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                horizontalAlignment : Text.AlignRight
                height: parent.height
            }

            TextField
            {
                id: setHighPressure_pressure
                validator: IntValidator { bottom:20; top: 90000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: parent.width - 70 - 10
                font.pixelSize: 12
                placeholderText: "Minimum: 20 mBar; Maximum: 90000 mBar"
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
                text: qsTr(" Enable output: ")
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: 150
            }
            CheckBox {
                id: setUseVac_openOutputValve
                checked: true
                text: (setUseVac_openOutputValve.checked) ? "At step intervals    " : "Never opens"
            }
            Text {
                text: qsTr("Valve 1 step size (mBar): ")
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: 150
                visible: (setUseVac_openOutputValve.checked) ? true : false
            }
            TextField
            {
                id: setStepSize_pressure
                validator: IntValidator { bottom:100; top: 6000 }
                inputMethodHints: Qt.ImhDigitsOnly
                height: parent.height
                width: 50
                text: "2000"
                placeholderText: "100 to 6000"
                font.pixelSize: 12
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
                text: qsTr(" Select Input: ")
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                Layout.minimumWidth: 60
            }
            ComboBox {
                id: setInputValve_pressure
                height: parent.height
                Layout.minimumWidth: 235
                textRole: "key"
                model: ListModel {
                    ListElement { key: "Valve 7 (High Pressure In)"; value: true }
                    ListElement { key: "Valve 9 (Nitrogen In)"; value: false }
                }
            }
            Text {
                text: qsTr("  Vac down:")
                color: "#777777"
                font.pixelSize: 13
                verticalAlignment : Text.AlignVCenter
                height: parent.height
                width: 150
            }
            CheckBox {
                id: setUseVac_pressure
                checked: false
                text: (setUseVac_pressure.checked) ? "Init pressure < 1.5bar" : ""
            }
        }

        Rectangle{
            color: "#eeeeee"
            height: 45
            Layout.maximumHeight: 45
            width: parent.width
            Layout.topMargin: 10
            RowLayout
            {
                id: d
                spacing: 10
                width: 220
                anchors.right: parent.right
                anchors.rightMargin: 10
                Layout.maximumWidth: 180
                Layout.maximumHeight: 45
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 20
                Button
                {
                    text: qsTr("Add")
                    enabled: true
                    visible: true
                    width: 60
                    Layout.alignment: Qt.AlignRight
                    Material.background: Material.color(Material.Orange, Material.Shade500)
                    Material.foreground:  Material.color(Material.Grey, Material.Shade100)
                    onClicked:
                    {
                        ScriptAddManager.addHighPressure(setHighPressure_pressure.text, setUseVac_pressure.checked, setStepSize_pressure.text, setInputValve_pressure.model.get(setInputValve_pressure.currentIndex).value, setUseVac_openOutputValve.checked);
                    }
                }

                Button
                {
                    text: qsTr("Remove")
                    enabled: true
                    visible: true
                    width: 60
                    Layout.alignment: Qt.AlignRight
                    Material.background: Material.color(Material.Grey, Material.Shade500)
                    Material.foreground:  Material.color(Material.Grey, Material.Shade100)
                    onClicked:
                    {
                        // Blank the box
                        item.draggedFunction = ""

                        // Delete current instance
                        item.loadedoption.destroy();
                    }
                }
            }
        }
    }






}
