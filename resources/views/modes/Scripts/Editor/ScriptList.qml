import QtQuick 2.9
import QtQml.Models 2.3
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Material 2.1


import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

Item {
    id: root

    width: parent.width
    height: 400

    Connections {
        target: ScriptAddManager
        onFunctionListChanged: {
            var options = ScriptAddManager.option(0);
            console.log(options["pressure"]);
        }
    }

    ListModel {
        id: functionlist
        ListElement {
            text: "Set valve 1"
        }
        ListElement {
            text: "Set pressure"
        }
        ListElement {
            text: "Set valve 4"
        }
    }

    FluidControls.Card {
        id: mainContent
        width: parent.width
        height: parent.height
        ColumnLayout {
            //anchors.fill: parent
            spacing: 0

            width: parent.width
            height: parent.height

            Rectangle {
                color: Material.color(Material.Grey, Material.Shade50)
                height: 50
                Layout.fillWidth: parent.width

                FluidControls.TitleLabel
                {
                    text: "Order and Configuration of Execution"
                    horizontalAlignment: Text.AlignHCenter
                    anchors.centerIn: parent
                    color: Material.color(Material.Grey, Material.Shade700)
                }
            }

            ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ListView {
                    id: listView
                    model: functionlist
                    delegate: DraggableFunction {
                        Rectangle {
                            height: textLabel.height * 2
                            width: listView.width
                            color: "white"

                            Text {
                                id: textLabel
                                anchors.centerIn: parent
                                text: model.text
                            }

                            // Bottom line border
                            Rectangle {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                    bottom: parent.bottom
                                }
                                height: 1
                                color: "lightgrey"
                            }
                        }

                        draggedItemParent: mainContent

                        onMoveItemRequested: {
                            functionlist.move(from, to, 1);
                        }
                    }
                }
            }
        }
    }
}
