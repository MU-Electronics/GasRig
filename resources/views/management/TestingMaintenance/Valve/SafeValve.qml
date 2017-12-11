import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQml.Models 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../../parts"

/**
  * Set the valve status
  */
FluidControls.Card
{
    width: parent.width-10
    height: valveStatusContainer.height + 30

    padding: 5


    Column
    {
        id: valveStatusContainer
        spacing: 0
        width: parent.width
        enabled: MachineStatusManager.shouldEnable("safeValveMachine")

        AlertBox
        {
            id: valveAlert
            width: parent.width
            type: (valveSafeMode.checked) ? "Success" : "Danger";
            textContent: (valveSafeMode.checked) ? qsTr("Protected: Safe mode enabled, which limits the possiable combinations. Be carefully though damage can still be inflicted.") : qsTr("No-Protection: Be carefully there is no automated protection enabled! Enable safe mode which limits the possiable combinations.");
        }

        Row
        {
            CheckBox {
                id: valveSafeMode
                checked: true
                text: qsTr("Enable safe mode to limit valve combinations")
            }
        }

        Item
        {
            id: valveLayoutContainer

            width: parent.width
            height: 90

            FluidLayouts.AutomaticGrid
            {
                id: valveLayout


                property var noInColoum: 5
                property var noInRow: 2

                width: parent.width

                cellWidth: parent.width / noInColoum
                cellHeight: 90

                anchors.fill: parent

                model: ListModel {
                    id: valvesList
                    ListElement { number: "1"; name: "Valve 1" }
                    ListElement { number: "2"; name: "Valve 2" }
                    ListElement { number: "3"; name: "Valve 3" }
                    ListElement { number: "4"; name: "Valve 4" }
                    ListElement { number: "5"; name: "Valve 5" }
                    ListElement { number: "6"; name: "Valve 6" }
                    ListElement { number: "7"; name: "Valve 7" }
                    ListElement { number: "8"; name: "Valve 8" }
                    ListElement { number: "9"; name: "Valve 9" }
                }
                delegate: Rectangle
                {
                    width: valveLayout.cellWidth - 10
                    height: valveLayoutContainer.height / 2

                    Button
                    {
                        text: model.name
                        objectName: "valveButton"
                        Material.background: if(SystemStatusManager.valveState[model.number] === 1){ Material.color(Material.Red, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                        Material.foreground: if(SystemStatusManager.valveState[model.number] === 1){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                        onClicked:
                        {
                            // Toggle the state
                            var toggle = !SystemStatusManager.valveState[model.number];

                            if(valveSafeMode.checked)
                            {
                                // Set vac pump
                                TestingManager.requestValveStateSafe(model.number, toggle);
                            }
                            else
                            {
                                // Set vac pump
                                TestingManager.requestValveState(model.number, toggle);
                            }
                        }
                    }
                }
            }
        }
    }
}
