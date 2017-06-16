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

    Flickable {
        width: parent.width
        height: window.height - 180
        contentWidth: parent.width
        contentHeight: flowController.height + flowRate.height + softStart.height

        /**
          * Set the flow rate
          */
        FluidControls.Card
        {
            id: flowController

            anchors.topMargin: 15

            width: parent.width-10
            height: 170

            padding: 5

            Column
            {
                spacing: 15
                width: parent.width
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

                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_1_override"] === 0) ? true : false;
                        text: qsTr("Off")
                        onClicked: {
                            controller1Override = 0;
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_1_override"] === 1) ? true : false;
                        text: qsTr("Open")
                        onClicked: {
                            controller1Override = 1;
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_1_override"] === 2) ? true : false;
                        text: qsTr("Closed")
                        onClicked: {
                            controller1Override = 2;
                        }
                    }
                    Button
                    {
                        text: qsTr("Set Controller 1")
                        onClicked:
                        {
                            // set the over ride status
                            TestingManager.requestValveOverride("FlowControllerOne", controller1Override);
                        }
                    }
                }
                Row
                {
                    spacing: 20;
                    width: parent.width

                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_override"] === 0) ? true : false;
                        text: qsTr("Off")
                        onClicked: {
                            controller2Override = 0;
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_override"] === 1) ? true : false;
                        text: qsTr("Open")
                        onClicked: {
                            controller2Override = 1;
                        }
                    }
                    RadioButton {
                        checked: (SystemStatusManager.flowControllerState["controller_2_override"] === 2) ? true : false;
                        text: qsTr("Closed")
                        onClicked: {
                            controller2Override = 2;
                        }
                    }
                    Button
                    {
                        text: qsTr("Set Controller 2")
                        onClicked:
                        {
                            // set the over ride status
                            TestingManager.requestValveOverride("FlowControllerTwo", controller2Override);
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
            height: 175

            padding: 5

            Column
            {
                width: parent.width
                spacing: 15
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

                    TextField
                    {
                        id: flowRateController1
                        placeholderText: qsTr("In millilitre per minute")
                        validator: IntValidator { bottom:0; top: 2000}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                    }

                    Button
                    {
                        text: qsTr("Set Flow Rate For Controller 1")
                        onClicked:
                        {
                            // set the flow rate
                            //TestingManager.requestPressureConfirmation();
                        }
                    }
                }
                Row
                {
                    spacing: 20;
                    width: parent.width

                    TextField
                    {
                        id: flowRateController2
                        placeholderText: qsTr("In millilitre per minute")
                        validator: IntValidator { bottom:0; top: 2000}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                    }

                    Button
                    {
                        text: qsTr("Set Flow Rate For Controller 2")
                        onClicked:
                        {
                            // set the flow rate
                            //TestingManager.requestPressureConfirmation();
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
            height: 260

            padding: 5

            Column
            {
                width: parent.width
                spacing: 25
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

                    TextField
                    {
                        id: softStartTimeController1
                        placeholderText: qsTr("In seconds")
                        validator: IntValidator { bottom:0; top: 60}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                    }

                    Button
                    {
                        text: qsTr("Set Soft Start Controller 1")
                        onClicked:
                        {
                            // set the flow rate
                            //TestingManager.requestPressureConfirmation();
                        }
                    }
                }

                Row
                {
                    spacing: 20;
                    width: parent.width

                    TextField
                    {
                        id: softStartTimeController2
                        placeholderText: qsTr("In seconds")
                        validator: IntValidator { bottom:0; top: 60}
                        inputMethodHints: Qt.ImhDigitsOnly
                        width: parent.width - 285
                    }

                    Button
                    {
                        text: qsTr("Set Soft Start Controller 2")
                        onClicked:
                        {
                            // set the flow rate
                            //TestingManager.requestPressureConfirmation();
                        }
                    }
                }

                Row
                {
                    spacing: 20;
                    width: parent.width

                    Button
                    {
                        text: qsTr("Soft Start Controller 1")
                        onClicked:
                        {
                            // set the flow rate
                            //TestingManager.requestPressureConfirmation();
                        }
                    }

                    Button
                    {
                        text: qsTr("Soft Start Controller 2")
                        onClicked:
                        {
                            // set the flow rate
                            //TestingManager.requestPressureConfirmation();
                        }
                    }
                }
            }
        }
    }

}
