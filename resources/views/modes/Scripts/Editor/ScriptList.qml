import QtQuick 2.9
import QtQml.Models 2.3
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import 'ViewOptions' as ViewOptions

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
            // console.log(i);
        }
        onFunctionMoved: {
            functionlist.move(from, to, 1);
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
                width: parent.width - 15
                height: mainContent.height - title.height - 50

                contentWidth: parent.width
                contentHeight: listView.height + 10
                z:5

                ScrollBar.vertical: ScrollBar {
                    id: vbar2;
                    active: true
                    policy: ScrollBar.AlwaysOn
                    anchors.left: parent.right
                    height: parent.height + 51
                }

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
                            id: functioncontainer
                            height: 50
                            width: listView.width
                            color: "white"

                            property var viewpanel
                            property var viewpanelcomp

                            Component.onCompleted: {
                                functioncontainer.viewpanelcomp = Qt.createComponent("ViewOptions/"+model.id+".qml");

                                if (functioncontainer.viewpanelcomp.status === Component.Ready || functioncontainer.viewpanelcomp.status === Component.Error)
                                {
                                    finishCreation();
                                 }else{
                                    functioncontainer.viewpanelcomp.statusChanged.connect(finishCreation);
                                 }
                            }

                            function finishCreation() {
                                if (functioncontainer.viewpanelcomp.status === Component.Ready)
                                {
                                    functioncontainer.viewpanel = functioncontainer.viewpanelcomp.createObject(functionShowOptions, {"options": model.options});

                                    if (functioncontainer.viewpanel === null)
                                        console.log("Error creating image");
                                }
                                else if (functioncontainer.viewpanelcomp.status === Component.Error)
                                {
                                    console.log("Error loading component:", functioncontainer.viewpanelcomp.errorString());
                                }
                            }

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
                                Item{
                                    id: functionShowOptions
                                    height: 50
                                    width: parent.width - 30 - 130
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
                            // Move function in back end
                            ScriptAddManager.moveFunction(from, to);
                        }
                    }
                }
            }
        }
    }
}
