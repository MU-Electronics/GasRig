import QtQuick 2.9
import QtQml.Models 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2


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
        onFunctionAdded: {
            // Get the last functiom added
            var count = ScriptAddManager.countFunctions() - 1;

            // Get the id and options
            var id = ScriptAddManager.id(count);
            var options = ScriptAddManager.option(count);

            // Generate json config
            var data = {"id": id, "options": options};

            // Append new function to model
            functionlist.append(data);
        }
        onFunctionRemoved: {
            console.log(i);
        }
    }

    ListModel {
        id: functionlist
    }

    FluidControls.Card {
        id: mainContent
        width: parent.width
        height: parent.height
        Column {
            id: list
            spacing: 0

            width: parent.width
            height: parent.height

            Rectangle {
                id: title
                color: Material.color(Material.Grey, Material.Shade50)
                height: 50
                width: parent.width
                z:50

                FluidControls.TitleLabel
                {
                    text: "Order and Configuration of Execution"
                    horizontalAlignment: Text.AlignHCenter
                    anchors.centerIn: parent
                    color: Material.color(Material.Grey, Material.Shade700)
                }
            }

            ScrollView {
                width: parent.width
                height: mainContent.height - title.height - 50

                contentWidth: parent.width
                contentHeight: listView.height + 10
                z:5
                ListView {
                    id: listView
                    model: functionlist
                    Label {
                        anchors.fill: parent
                        horizontalAlignment: Qt.AlignHCenter
                        verticalAlignment: Qt.AlignVCenter
                        visible: parent.count == 0
                        text: qsTr("No functions have been added yet.")
                        font.bold: true
                    }
                    delegate: DraggableFunction {
                        Rectangle {
                            // height: textLabel.height * 2
                            height: 50
                            width: listView.width
                            color: "white"

                            Row{
                                width: parent.width
                                height: 50
                                spacing: 0

                                //Layout.alignment: Qt.AlignLeft

                                Rectangle{
                                    color: "#dbdbdb"
                                    height: 50
                                    width: 30
                                    //Layout.maximumWidth: 50
                                    Text{
                                        text: index + 1
                                        wrapMode: Text.WordWrap
                                        anchors.fill: parent
                                        horizontalAlignment: Qt.AlignHCenter
                                        verticalAlignment: Qt.AlignVCenter
                                        font.pointSize: 11;
                                        style: Text.Raised;
                                        styleColor: "#d6d6d6"
                                        font.weight: Font.DemiBold
                                        color: "#606060"
                                    }
                                }
                                Rectangle{
                                    color: "#eeeeee"
                                    height: 50
                                    width: 130
                                    //Layout.maximumWidth: 130
                                    Text{
                                        id: modelid
                                        text: model.id
                                        wrapMode: Text.WordWrap
                                        // anchors.fill: parent
                                        height: 20
                                        width: parent.width
                                        horizontalAlignment: Qt.AlignHCenter
                                        // verticalAlignment: Qt.AlignVCenter
                                        anchors.top: parent.top
                                        anchors.topMargin: 5
                                        font.pointSize: 10;
                                        style: Text.Raised;
                                        styleColor: "#d6d6d6"
                                        font.weight: Font.DemiBold
                                        color: "#606060"
                                    }
                                    Button{
                                        id: remove
                                        text: "remove"
                                        height: 25
                                        width: parent.width
                                        anchors.top: modelid.bottom
                                        background: Rectangle {
                                            implicitWidth: parent.width
                                            implicitHeight: 25
                                            color: remove.down ?  Material.color(Material.Red, Material.Shade700) :  Material.color(Material.Red, Material.Shade500)
                                            radius: 0
                                        }
                                        contentItem: Text {
                                            text: remove.text
                                            font.pointSize: 10
                                            font.weight: Font.DemiBold
                                            color: remove.down ? "#f9f9f9" : "#f9f9f9"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                            style: Text.Raised;
                                            styleColor: "#e2e2e2"
                                        }
                                        onClicked:
                                        {
                                            // Remove from view manager container
                                            ScriptAddManager.removeFunction(index);

                                            // Remove form list
                                            functionlist.remove(index);
                                        }
                                    }
                                }

                                Text{
                                    text: model.options["pressure"]
                                    width: 100
                                    Layout.maximumWidth: 50
                                }

                            }

                            // Bottom line border
                            Rectangle {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                    bottom: parent.bottom
                                }
                                height: 1
                                // color: "lightgrey"
                                color: "#dbdbdb"
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
