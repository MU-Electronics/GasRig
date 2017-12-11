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
  * Exhuast sysem
  */
FluidControls.Card
{
    id: exhuastSystem
    padding: 5
    width: parent.width - 10
    height: exhuastSystemContainer.height + 30

    Column
    {
        width: parent.width
        spacing: 20
        id: exhuastSystemContainer
        enabled: MachineStatusManager.shouldEnable("ventMachine")
        AlertBox
        {
            width: exhuastSystem.width - 10
            type: "Warning"
            textContent: qsTr("This function will vent pressures safely to the exhuast port. Ensure the exhuast port is conneted to the correct and safe location first.")
        }
        Row
        {
            anchors.left: parent.left
            spacing: 10
            width: parent.width
            Text {
                text: qsTr("Location to vent: ")
                color: "#777777"
                visible: parent.opacity
                font.pixelSize: 16
                verticalAlignment : Text.AlignVCenter
                height: 40
            }
            Column{
                CheckBox {
                    id: exhuastSystem_ventLocation_rig
                    checked: true
                    enabled: false
                    text: qsTr("Inner rig and exhuast cavity")
                }
                CheckBox {
                    id: exhuastSystem_ventLocation_output
                    checked: false
                    text: qsTr("Output")
                }
                CheckBox {
                    id: exhuastSystem_ventLocation_vacuumOutput
                    checked: false
                    text: qsTr("Vacuum Output")
                }
                CheckBox {
                    id: exhuastSystem_ventLocation_flow
                    checked: false
                    text: qsTr("Flow cavity")
                }
                CheckBox {
                    id: exhuastSystem_ventLocation_nitrogenPipes
                    checked: false
                    text: qsTr("Nitrogen flex pipes")
                }
                CheckBox {
                    id: exhuastSystem_ventLocation_multiPipes
                    checked: false
                    text: qsTr("Multi gas flex pipe")
                }
                CheckBox {
                    id: exhuastSystem_ventLocation_flowOnePipes
                    checked: false
                    text: qsTr("Flow controller one's flex pipe")
                }
                CheckBox {
                    id: exhuastSystem_ventLocation_flowTwoPipes
                    checked: false
                    text: qsTr("Flow controller two's flex pipe")
                }
            }
        }
        Row{
            spacing: 20
            height: 50;
            width: parent.width
            Behavior on opacity {
                NumberAnimation {
                    duration: 500
                    easing.type: Easing.InOutQuad
                }
            }
            CheckBox {
                id:exhuastSystem_confirm
                checked: false
                text: qsTr("Comfirm rig setup")
            }
            Button
            {
                text: qsTr("Begin Vent")
                enabled: (exhuastSystem_confirm.checked) ? true : false;
                visible: (MachineStatusManager.ventMachine["status"] === 0) ? 1 : 0;
                onClicked:
                {
                    TestingManager.requestVent(exhuastSystem_ventLocation_output.checked,
                                                  exhuastSystem_ventLocation_vacuumOutput.checked,
                                                  exhuastSystem_ventLocation_flow.checked,
                                                  exhuastSystem_ventLocation_nitrogenPipes.checked,
                                                  exhuastSystem_ventLocation_multiPipes.checked,
                                                  exhuastSystem_ventLocation_flowOnePipes.checked,
                                                  exhuastSystem_ventLocation_flowTwoPipes.checked);
                }
            }
            Button
            {
                text: qsTr("Stop")
                visible: (MachineStatusManager.ventMachine["status"] === 1) ? 1 : 0;
                onClicked:
                {
                    TestingManager.requestVentStop();
                }
            }
            Button
            {
                text: qsTr("Stopping")
                enabled: false
                visible: (MachineStatusManager.ventMachine["status"] === 2) ? 1 : 0;
            }
        }
    }
}
