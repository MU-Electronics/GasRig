import QtQuick 2.5
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQml.Models 2.2

import Fluid.Layouts 1.0 as FluidLayouts
import Fluid.Controls 1.0 as FluidControls
import Fluid.Material 1.0 as FluidMaterial
import Fluid.Core 1.0 as FluidCore

import "../../parts"

Item {
    id: valveTab

    width: parent.width - 10



    Flickable {

        width: parent.width
        height: window.height - 180
        contentWidth: parent.width
        contentHeight: purgeSystemFunction.height + valveStateControl.height + outputHighPressure.height + exhuastSystem.height + setHighPressure.height + 50;




        /**
          * Set a high pressure in the system
          */
        FluidControls.Card
        {
            id: setHighPressure

            width: parent.width - 10

            height: setHighPressureContainer.height + 30

            padding: 5


            Behavior on height {
                PropertyAnimation {
                    duration: 500
                }
            }

            Column
            {
                width: parent.width

                spacing: 20

                id: setHighPressureContainer

                move: Transition {
                    NumberAnimation { properties: "x,y"; duration: 500 }
                }


                AlertBox
                {
                    width: setHighPressure.width - 10
                    type: "Danger"
                    textContent: qsTr("This function will set a high pressure within the system. Ensure the rig is correctly setup before running.")
                }

                Row
                {
                    anchors.left: parent.left
                    spacing: 10
                    width: parent.width
                    height: 50
                    Text {
                        text: qsTr("Pressure (in bar): ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    TextField
                    {
                        id: setHighPressure_pressure
                        validator: IntValidator { bottom:0; top: 90 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - 170
                        placeholderText: "10"
                    }
                }
                Row{
                    spacing: 20
                    opacity: (!setHighPressure_pressure.text) ? 0 : 1;
                    height: (!setHighPressure_pressure.text) ? 0 : 50;
                    width: parent.width
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutQuad
                        }
                    }
                    CheckBox {
                        id:setHighPressure_confirm
                        checked: false
                        text: qsTr("Comfirm rig setup")
                    }
                    Button
                    {
                        text: qsTr("Set high pressure")
                        enabled: (setHighPressure_confirm.checked) ? true : false;
                        onClicked:
                        {
                            TestingManager.requestHighPressure(setHighPressure_pressure.text);
                        }
                    }
                }
            }
        }




        /**
          * Exhuast sysem
          */
        FluidControls.Card
        {
            id: exhuastSystem

            width: parent.width - 10

            height: exhuastSystemContainer.height + 30

            padding: 5

            anchors.top: setHighPressure.bottom
            anchors.topMargin: 15

            Column
            {
                width: parent.width
                spacing: 20
                id: exhuastSystemContainer
                AlertBox
                {
                    width: exhuastSystem.width - 10
                    type: "Warning"
                    textContent: qsTr("This function will exhuast high pressures safely to the exhuast port. Ensure the exhuast port is conneted to the correct and safe location.")
                }
                Row{
                    RadioButton {
                        checked: false
                        id: exhuastSystem_valveSlow
                        text: qsTr("Slow filter")
                    }
                    RadioButton {
                        checked: false
                        id: exhuastSystem_valveFast
                        text: qsTr("Fast filter")
                    }
                    RadioButton {
                        checked: true
                        id: exhuastSystem_valveCombo
                        text: qsTr("Combination of fast + slow filter")
                    }
                }
                Row
                {
                    anchors.left: parent.left
                    spacing: 10
                    width: parent.width
                    height: 50
                    Text {
                        text: qsTr("Frequency: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    TextField
                    {
                        id: exhuastSystem_frequency
                        validator: IntValidator { bottom:-1; top: 5 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - 170
                        placeholderText: "2"
                    }
                }
                Row{
                    spacing: 20
                    opacity: (!exhuastSystem_frequency.text) ? 0 : 1;
                    height: (!exhuastSystem_frequency.text) ? 0 : 50;
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
                        text: qsTr("Begin Exhuast")
                        enabled: (exhuastSystem_confirm.checked) ? true : false;
                        onClicked:
                        {
                            var filterType;
                            if(exhuastSystem_valveSlow.checked)
                                filterType = 1;
                            if(exhuastSystem_valveFast.checked)
                                filterType = 2;
                            if(exhuastSystem_valveCombo.checked)
                                filterType = 3;

                            TestingManager.requestExhuast(filterType, exhuastSystem_frequency.text);
                        }
                    }
                }
            }
        }




        /**
          * Output high pressure safely
          */
        FluidControls.Card
        {
            id: outputHighPressure

            width: parent.width - 10

            height: outputHighPressureContainer.height + 30

            padding: 5

            anchors.top: exhuastSystem.bottom
            anchors.topMargin: 15

            Column
            {
                width: parent.width
                spacing: 20
                id: outputHighPressureContainer
                AlertBox
                {
                    width: outputHighPressure.width - 10
                    type: "Warning"
                    textContent: qsTr("This function will pulse the output valve to output port pressures safely. Ensure the output port is conneted to the correct and safe location.")
                }

                Row
                {
                    anchors.left: parent.left
                    spacing: 10
                    width: parent.width
                    height: 50
                    Text {
                        text: qsTr("Frequency: ")
                        color: "#777777"
                        visible: parent.opacity
                        font.pixelSize: 16
                        verticalAlignment : Text.AlignVCenter
                        height: parent.height
                    }
                    TextField
                    {
                        id: outputHighPressure_frequency
                        validator: IntValidator { bottom:-1; top: 5 }
                        inputMethodHints: Qt.ImhDigitsOnly
                        height: parent.height
                        width: parent.width - 170
                        placeholderText: "2"
                    }
                }
                Row{
                    spacing: 20
                    opacity: (!outputHighPressure_frequency.text) ? 0 : 1;
                    height: (!outputHighPressure_frequency.text) ? 0 : 50;
                    width: parent.width
                    Behavior on opacity {
                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.InOutQuad
                        }
                    }
                    CheckBox {
                        id:outputHighPressure_confirm
                        checked: false
                        text: qsTr("Comfirm rig setup")
                    }
                    Button
                    {
                        text: qsTr("Begin Outputting Pressure")
                        enabled: (outputHighPressure_confirm.checked) ? true : false;
                        onClicked:
                        {
                            TestingManager.requestOutputPressure(outputHighPressure_frequency.text);
                        }
                    }
                }
            }
        }






        /**
          * Safe vacuum station functions
          */
        FluidControls.Card
        {
            id: purgeSystemFunction

            width: parent.width - 10

            height: purgingColumConatiner.height + 30

            padding: 5

            anchors.top: outputHighPressure.bottom
            anchors.topMargin: 15

            Column
            {
                width: parent.width
                spacing: 20
                id: purgingColumConatiner
                AlertBox
                {
                    width: purgeSystemFunction.width - 10
                    type: "Notice"
                    textContent: qsTr("Purging the system can take many forms and parameters. Ensure you setup the rig correctly for the selected options.")
                }
                Column
                {
                    ButtonGroup
                    {
                        id: purgingMethodGroup
                    }

                    spacing: 10

                    width: purgeSystemFunction.width - 10


                    move: Transition {
                        NumberAnimation { properties: "x,y"; duration: 400 }
                    }


                    RadioButton {
                        checked: false
                        id: purgeMethodOne
                        text: qsTr("1: Pressurise with nitrogren, then exhuast, then vac down and repeat")
                        ButtonGroup.group: purgingMethodGroup
                        onClicked: {
                            purgeMethod_confirm.checked = false
                        }
                    }
                    Column
                    {
                        spacing: 10
                        opacity: (!purgeMethodOne.checked) ? 0 : 1;
                        width: parent.width
                        Behavior on opacity {
                            NumberAnimation {
                                duration: 1000
                                easing.type: Easing.InOutQuad
                            }
                        }
                        CheckBox {
                            id:purgeMethodOne_outputValve
                            height: (!purgeMethodOne.checked) ? 0 : 30;
                            checked: false
                            text: qsTr("Should the output valve be open during the purge?")
                            anchors.leftMargin: 100
                            anchors.left: parent.left
                        }

                        Row
                        {
                            anchors.leftMargin: 100
                            anchors.left: parent.left
                            height: (!purgeMethodOne.checked) ? 0 : 50;
                            spacing: 10
                            width: parent.width - 110
                            Text {
                                text: qsTr("Number of cycles to run before finishing: ")
                                color: "#777777"
                                visible: parent.opacity
                                font.pixelSize: 16
                                verticalAlignment : Text.AlignVCenter
                                height: parent.height
                            }
                            TextField
                            {
                                id: purgeMethodOne_numberCycles
                                validator: IntValidator { bottom:0; top: 2000 }
                                inputMethodHints: Qt.ImhDigitsOnly
                                height: parent.height
                                width: parent.width - 315
                            }
                        }

                        Row
                        {
                            anchors.leftMargin: 100
                            anchors.left: parent.left
                            height: (!purgeMethodOne.checked) ? 0 : 50;
                            spacing: 10
                            width: parent.width - 110
                            Text {
                                text: qsTr("Maximum nitrogen pressure in bar: ")
                                color: "#777777"
                                visible: parent.opacity
                                font.pixelSize: 16
                                verticalAlignment : Text.AlignVCenter
                                height: parent.height
                            }
                            TextField
                            {
                                id:purgeMethodOne_nitrogenPressure
                                validator: IntValidator { bottom:0; top: 90}
                                inputMethodHints: Qt.ImhDigitsOnly
                                height: parent.height
                                width: parent.width - 270
                            }
                        }
                    }



                    RadioButton {
                        id: purgeMethodTwo
                        checked: false
                        text: qsTr("2: Continuous nitrogen flow")
                        ButtonGroup.group:purgingMethodGroup
                        onClicked: {
                            purgeMethod_confirm.checked = false
                        }
                    }
                    Column
                    {
                        spacing: 10
                        opacity: (!purgeMethodTwo.checked) ? 0 : 1;
                        width: parent.width
                        Behavior on opacity {
                            NumberAnimation {
                                duration: 1000
                                easing.type: Easing.InOutQuad
                            }
                        }
                        AlertBox
                        {
                            id: purgeMethodTwo_warningOne
                            width: purgeSystemFunction.width - 110
                            height: (!purgeMethodTwo.checked) ? 0 : 30;
                            type: "Danger"
                            textContent: qsTr("Output valve and exhuast valve will be open!")
                            anchors.leftMargin: 100
                            anchors.left: parent.left
                        }
                        AlertBox
                        {
                            id: purgeMethodTwo_warningTwo
                            width: purgeSystemFunction.width - 110
                            height: (!purgeMethodTwo.checked) ? 0 : 30;
                            type: "Warning"
                            textContent: qsTr("Remember to set the pressure on the pressure regulator")
                            anchors.leftMargin: 100
                            anchors.left: parent.left
                        }

                        Row
                        {
                            anchors.leftMargin: 100
                            anchors.left: parent.left
                            height: (!purgeMethodTwo.checked) ? 0 : 50;
                            spacing: 10
                            width: parent.width - 110
                            Text {
                                text: qsTr("How long to purge for in minutes: ")
                                color: "#777777"
                                visible: parent.opacity
                                font.pixelSize: 16
                                verticalAlignment : Text.AlignVCenter
                                height: parent.height
                            }
                            TextField
                            {
                                id:purgeMethodTwo_time
                                validator: IntValidator { bottom:0; top: 200}
                                inputMethodHints: Qt.ImhDigitsOnly
                                height: parent.height
                                width: parent.width - 270
                            }
                        }
                        Row
                        {
                            anchors.leftMargin: 100
                            anchors.left: parent.left
                            height: (!purgeMethodTwo.checked) ? 0 : 50;
                            spacing: 10
                            width: parent.width - 110
                            Text {
                                text: qsTr("Maximum nitrogen pressure in bar: ")
                                color: "#777777"
                                visible: parent.opacity
                                font.pixelSize: 16
                                verticalAlignment : Text.AlignVCenter
                                height: parent.height
                            }
                            TextField
                            {
                                id:purgeMethodTwo_nitrogenPressure
                                validator: IntValidator { bottom:0; top: 90}
                                inputMethodHints: Qt.ImhDigitsOnly
                                height: parent.height
                                width: parent.width - 270
                            }
                        }
                    }
                }
                Column
                {
                    Row
                    {
                        id: purgeSubmittion
                        property int methodSelected: {
                            if(purgeMethodTwo.checked && purgeMethodTwo_time.text && purgeMethodTwo_nitrogenPressure.text)
                            {
                                return 2;
                            }
                            if(purgeMethodOne.checked && purgeMethodOne_numberCycles.text && purgeMethodOne_nitrogenPressure.text)
                            {
                                return 1;
                            }

                            return 0;
                        }

                        height: ( methodSelected > 0 ) ? 50 : 0;
                        opacity: ( methodSelected > 0 ) ? 1 : 0;

                        Behavior on opacity {
                            NumberAnimation {
                                duration: 1000
                                easing.type: Easing.InOutQuad
                            }
                        }

                        CheckBox {
                            id: purgeMethod_confirm
                            checked: false
                            text: qsTr("Confirm rig setup")
                        }
                        Button
                        {
                            text: qsTr("Start Purging System")
                            enabled: (purgeMethod_confirm.checked) ? true : false;
                            onClicked:
                            {
                                if(purgeSubmittion.methodSelected == 1)
                                {
                                    TestingManager.requestPurgeSystemMethodOne(purgeMethodOne_outputValve.text, purgeMethodOne_numberCycles.text, purgeMethodOne_nitrogenPressure.text);
                                }
                                else if (purgeSubmittion.methodSelected == 2)
                                {
                                    TestingManager.requestPurgeSystemMethodTwo(purgeMethodTwo_time.text, purgeMethodTwo_nitrogenPressure.text);
                                }
                            }
                        }
                    }
                }
            }
        }




        /**
          * Set the valve status
          */
        FluidControls.Card
        {
            id: valveStateControl

            anchors.top: purgeSystemFunction.bottom
            anchors.topMargin: 15

            width: parent.width-10

            height: 230

            padding: 5


            Column
            {
                spacing: 0
                width: parent.width
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
    }
}
