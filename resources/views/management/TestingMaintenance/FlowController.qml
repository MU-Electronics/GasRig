import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item
{

    id: flowControllerTab

    width: parent.width - 10

    // Hold the select value for override status
    property int controller1Override: SystemStatusManager.flowControllerState["controller_1_override"]
    property int controller2Override: SystemStatusManager.flowControllerState["controller_2_override"]

    // Hold the selected source value
    property int controller1Source: SystemStatusManager.flowControllerState["controller_1_source"]
    property int controller2Source: SystemStatusManager.flowControllerState["controller_1_source"]

    Flickable {
        width: parent.width
        height: window.height - 180
        contentWidth: parent.width
        contentHeight: flowController.height + flowRate.height + softStart.height + flowControllerSource.height + safeFlowControllerFunctions.height + 50


        /**
         * Turn pump and turbo on and off
         */
        FluidControls.Card
        {
            id: safeFlowControllerFunctions



            width: parent.width-10

            height: flowControllerReadings.height + 15

            padding: 5

            Column
            {
                id: flowControllerReadings
                width: parent.width
                AlertBox
                {
                    id: alertBoxSafeFunction
                    width: parent.width
                    type: "Notice"
                    textContent: qsTr("This container contains safe functions for the flow controller.")
                }

                Row {
                    spacing: 6

                    Button
                    {
                        id: getFlowControllerOne
                        text: "Get Flow Rate Controller 1"
                        opacity: (GlobalManager.rigSettings["flow_controller_1"]) ? 1 : 0
                        width: (GlobalManager.rigSettings["flow_controller_1"]) ? 250 : 0
                        onClicked:
                        {
                            TestingManager.requestActualFlowControllerFlowRate("FlowControllerOne");
                        }
                    }

                    Button
                    {
                        id: getTemperatureControllerOne
                        text: "Get Temperature Controller 1"
                        opacity: (GlobalManager.rigSettings["flow_controller_1"]) ? 1 : 0
                        width: (GlobalManager.rigSettings["flow_controller_1"]) ? 250 : 0
                        onClicked:
                        {
                            TestingManager.requestFlowControllerTemperature("FlowControllerOne");
                        }
                    }

                }
                Row{
                    spacing: 6

                    Button
                    {
                        id: getFlowControllerTwo
                        text: "Get Flow Rate Controller 2"
                        opacity: (GlobalManager.rigSettings["flow_controller_2"]) ? 1 : 0
                        width: (GlobalManager.rigSettings["flow_controller_2"]) ? 250 : 0
                        onClicked:
                        {
                            TestingManager.requestActualFlowControllerFlowRate("FlowControllerTwo");
                        }
                    }

                    Button
                    {
                        id: getTemperatureControllerTwo
                        text: "Get Temperature Controller 2"
                        opacity: (GlobalManager.rigSettings["flow_controller_2"]) ? 1 : 0
                        width: (GlobalManager.rigSettings["flow_controller_2"]) ? 250 : 0
                        onClicked:
                        {
                            TestingManager.requestFlowControllerTemperature("FlowControllerTwo");
                        }
                    }
                }
            }

        }


        /**
          * Set the flow rate
          */
        FluidControls.Card
        {
            id: flowController

            anchors.top: safeFlowControllerFunctions.bottom
            anchors.topMargin: 25

            width: parent.width-10
            height: rowValveOverride.height

            padding: 5

            Column
            {
                spacing: 15
                width: parent.width
                id: rowValveOverride

                AlertBox
                {
                    height: 30
                    width: parent.width
                    type: "Warning"
                    textContent: qsTr("Applies a valve override and take precedence over all other settings.")
                }
                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_1"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_1"]) ? 50 : 0
                    Text {
                        text: qsTr("Controller 1: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    RadioButton {

                        checked: (SystemStatusManager.flowControllerState["controller_1_override"] === 0) ? true : false;
                        text: qsTr("Off")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestFlowControllerValveOverride("FlowControllerOne", 0);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_1_override"] === 1) ? true : false;
                        text: qsTr("Open")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestFlowControllerValveOverride("FlowControllerOne", 1);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_1_override"] === 2) ? true : false;
                        text: qsTr("Closed")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestFlowControllerValveOverride("FlowControllerOne", 2);
                        }
                    }
                }
                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_2"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_2"]) ? 50 : 0
                    Text {
                        text: qsTr("Controller 2: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_override"] === 0) ? true : false;
                        text: qsTr("Off")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestFlowControllerValveOverride("FlowControllerTwo", 0);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_override"] === 1) ? true : false;
                        text: qsTr("Open")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestFlowControllerValveOverride("FlowControllerTwo", 1);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_override"] === 2) ? true : false;
                        text: qsTr("Closed")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestFlowControllerValveOverride("FlowControllerTwo", 2);
                        }
                    }
                }
            }

        }





        /**
         * Set the flow rate
         */
        FluidControls.Card
        {
            id: flowRate

            anchors.top: flowController.bottom
            anchors.topMargin: 15

            width: parent.width-10
            height: setFlowRate.height + 20

            padding: 5

            Column
            {
                width: parent.width
                spacing: 15
                id: setFlowRate
                AlertBox
                {
                    height: 30
                    width: parent.width
                    type: "Notice"
                    textContent: qsTr("Have you configured the valves into the correct state?")
                }
                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_1"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_1"]) ? 50 : 0
                    TextField
                    {
                        id: flowRateController1
                        placeholderText: qsTr("In millilitre per minute")
                        validator: DoubleValidator { bottom:0; top: 2000; decimals: 4; notation: DoubleValidator.StandardNotation}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                        text: SystemStatusManager.flowControllerState["controller_1_set_flowrate"]
                    }

                    Button
                    {
                        text: qsTr("Set Flow Rate For Controller 1")
                        onClicked:
                        {
                            // Set the flow rate
                            TestingManager.requestFlowControllerFlowRate("FlowControllerOne", flowRateController1.text);
                        }
                    }
                }
                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_2"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_2"]) ? 50 : 0
                    TextField
                    {
                        id: flowRateController2
                        placeholderText: qsTr("In millilitre per minute")
                        validator: DoubleValidator { bottom:0; top: 2000; decimals: 4; notation: DoubleValidator.StandardNotation}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                        text: SystemStatusManager.flowControllerState["controller_2_set_flowrate"]
                    }

                    Button
                    {
                        text: qsTr("Set Flow Rate For Controller 2")
                        onClicked:
                        {
                            // Set the flow rate
                            TestingManager.requestFlowControllerFlowRate("FlowControllerTwo", flowRateController2.text);
                        }
                    }
                }
            }
        }





        /**
         * Set the soft start value
         */
        FluidControls.Card
        {
            id: softStart

            anchors.top: flowRate.bottom
            anchors.topMargin: 15

            width: parent.width-10
            height: setSoftStart.height + 20

            padding: 5

            Column
            {
                width: parent.width
                spacing: 25
                id: setSoftStart
                AlertBox
                {
                    height: 30
                    width: parent.width
                    type: "Notice"
                    textContent: qsTr("This will be saved and used for all experiments unless specificed otherwise. Remember to enable soft start below as well as settting the time constant.")
                }
                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_1"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_1"]) ? 50 : 0
                    TextField
                    {
                        id: softStartTimeController1
                        placeholderText: qsTr("In seconds")
                        validator: IntValidator { bottom:0; top: 60}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                        text: SystemStatusManager.flowControllerState["controller_1_softstart_time"]
                    }

                    Button
                    {
                        text: qsTr("Set Soft Start Controller 1")
                        onClicked:
                        {
                            // Set the soft start time
                            TestingManager.requestFlowControllerSoftStartTime("FlowControllerOne", softStartTimeController1.text);
                        }
                    }
                }

                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_2"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_2"]) ? 50 : 0
                    TextField
                    {
                        id: softStartTimeController2
                        placeholderText: qsTr("In seconds")
                        validator: IntValidator { bottom:0; top: 60}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                        text: SystemStatusManager.flowControllerState["controller_2_softstart_time"]
                    }

                    Button
                    {
                        text: qsTr("Set Soft Start Controller 2")
                        onClicked:
                        {
                            // Set the soft start time
                            TestingManager.requestFlowControllerSoftStartTime("FlowControllerTwo", softStartTimeController2.text);
                        }
                    }
                }

                Row
                {
                    spacing: 20;
                    width: parent.width

                    Button
                    {
                        opacity: (GlobalManager.rigSettings["flow_controller_1"]) ? 1 : 0
                        height: (GlobalManager.rigSettings["flow_controller_1"]) ? 50 : 0
                        text: qsTr("Soft Start Controller 1")
                        Material.background: if(SystemStatusManager.flowControllerState["controller_1_softstart"] === 4){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                        Material.foreground: if(SystemStatusManager.flowControllerState["controller_1_softstart"] === 4){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                        onClicked:
                        {
                            // Toggle the state
                            var toggle;
                            if(SystemStatusManager.flowControllerState["controller_1_softstart"] === 4)
                            {
                                toggle = 0;
                            }
                            else
                            {
                                toggle = 4;
                            }

                            // Set vac pump
                            TestingManager.requestFlowControllerSoftStart("FlowControllerOne", toggle);
                        }
                    }

                    Button
                    {
                        opacity: (GlobalManager.rigSettings["flow_controller_2"]) ? 1 : 0
                        height: (GlobalManager.rigSettings["flow_controller_2"]) ? 50 : 0
                        text: qsTr("Soft Start Controller 2")
                        Material.background: if(SystemStatusManager.flowControllerState["controller_2_softstart"] === 4){ Material.color(Material.Green, Material.Shade500) }else{ Material.color(Material.Grey, Material.Shade300) }
                        Material.foreground: if(SystemStatusManager.flowControllerState["controller_2_softstart"] === 4){ Material.color(Material.Grey, Material.Shade100) }else{ Material.color(Material.Grey, Material.Shade800) }
                        onClicked:
                        {
                            // Toggle the state
                            var toggle;
                            if(SystemStatusManager.flowControllerState["controller_2_softstart"] === 4)
                            {
                                toggle = 0;
                            }
                            else
                            {
                                toggle = 4;
                            }

                            // Set vac pump
                            TestingManager.requestFlowControllerSoftStart("FlowControllerTwo", toggle);
                        }
                    }
                }
            }
        }



        /**
          * Set the control source
          */
        FluidControls.Card
        {
            id: flowControllerSource

            anchors.topMargin: 15
            anchors.top: softStart.bottom

            width: parent.width-10
            height: setSource.height

            padding: 5

            Column
            {
                id: setSource
                spacing: 15
                width: parent.width
                AlertBox
                {
                    height: 30
                    width: parent.width
                    type: "Danger"
                    textContent: qsTr("If setting the below to analogue the program wont be able to control the flow controller; but you can always change it back to digital.")
                }
                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_1"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_1"]) ? 50 : 0
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_1_source"] === 0) ? true : false;
                        text: qsTr("Analogue")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestSetFlowControllerSourceControl("FlowControllerOne", 0);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_1_source"] === 3) ? true : false;
                        text: qsTr("Digital")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestSetFlowControllerSourceControl("FlowControllerOne", 3);
                        }
                    }
                }
                Row
                {
                    spacing: 20;
                    width: parent.width
                    opacity: (GlobalManager.rigSettings["flow_controller_2"]) ? 1 : 0
                    height: (GlobalManager.rigSettings["flow_controller_2"]) ? 50 : 0
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_source"] === 0) ? true : false;
                        text: qsTr("Analogue")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestSetFlowControllerSourceControl("FlowControllerTwo", 0);
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_source"] === 3) ? true : false;
                        text: qsTr("Digital")
                        onClicked: {
                            // set the over ride status
                            TestingManager.requestSetFlowControllerSourceControl("FlowControllerTwo", 3);
                        }
                    }

                }
            }

        }
    }

}
