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
        enabled: (MachineStatusManager.controllable["safeValveMachine"] === 0) ? 0 : 1 ;

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
                property var buttons: [
                    {
                        number: "1",
                        name: "Valve 1",
                        enabled: true
                    },
                    {
                        number: "2",
                        name: "Valve 2",
                        enabled: true
                    },
                    {
                        number: "3",
                        name: "Valve 3",
                        enabled: true
                    },
                    {
                        number: "4",
                        name: "Valve 4",
                        enabled: GlobalManager.rigSettings["valve_4"]
                    },
                    {
                        number: "5",
                        name: "Valve 5",
                        enabled: true
                    },
                    {
                        number: "6",
                        name: "Valve 6",
                        enabled: GlobalManager.rigSettings["valve_6"]
                    },
                    {
                        number: "7",
                        name: "Valve 7",
                        enabled: true
                    },
                    {
                        number: "8",
                        name: "Valve 8",
                        enabled: GlobalManager.rigSettings["valve_8"]
                    },
                    {
                        number: "9",
                        name: "Valve 9",
                        enabled: true
                    }
                ]

                width: parent.width

                cellWidth: parent.width / noInColoum
                cellHeight: 90

                anchors.fill: parent

                model: buttons

                delegate: Rectangle
                {
                    property var element: valveLayout.buttons[model.index]

                    width: (element.enabled) ? valveLayout.cellWidth - 10 : 0;
                    height: valveLayoutContainer.height / 2

                    opacity: element.enabled

                    Button
                    {
                        text: element.name
                        objectName: "valveButton"
                        Material.background: if(SystemStatusManager.valveState[element.number] === 1){ Material.color(Material.Red, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                        Material.foreground: if(SystemStatusManager.valveState[element.number] === 1){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                        onClicked:
                        {
                            // Toggle the state
                            var toggle = !SystemStatusManager.valveState[element.number];

                            if(valveSafeMode.checked)
                            {
                                // Set vac pump
                                TestingManager.requestValveStateSafe(element.number, toggle);
                            }
                            else
                            {
                                // Set vac pump
                                TestingManager.requestValveState(element.number, toggle);
                            }
                        }
                    }
                }
            }
        }
    }
}
