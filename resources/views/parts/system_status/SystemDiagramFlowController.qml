import QtQuick 2.0

Rectangle
{
    id: flowController

    property int controller: 1

    function whichColorBackground(valveOverride, setFlowRate)
    {
        if(valveOverride === 1)
        {
            return '#EF5350';
        }
        else if(setFlowRate > 0)
        {
            return '#FF9800';
        }
        else
        {
            return '#d4d5d8';
        }
    }

    function whichColorBorder(valveOverride, setFlowRate)
    {
        if(valveOverride === 1)
        {
            return '#E53935';
        }
        else if(setFlowRate > 0)
        {
            return '#FB8C00';
        }
        else
        {
            return '#c8cace';
        }
    }

    color: whichColorBackground(SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_override"], SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_set_flowrate"])
    border.color: whichColorBorder(SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_override"], SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_set_flowrate"])
    border.width: 5
    width: 140
    height: 170

    Text{
        text: "Flow Controller " + flowController.controller
        color: "#5e5f63"
        font.bold: true
        font.pixelSize: 15
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 7
    }

    Text{
        text:   "Flow: " + SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_flow"] + " ml/min\n" +
                "Set: " + SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_set_flowrate"] + " ml/min\n" +
                "Soft Start: " + SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_softstart_verbal"] + "\n" +
                "Soft Start time: " + SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_softstart_time"] + " s\n" +
                "Override: " + SystemStatusManager.flowControllerState["controller_"+flowController.controller+"_override_verbal"];
        horizontalAlignment: Text.AlignHCenter
        color: "#5e5f63"
        font.pixelSize: 10
        lineHeight: 1.5
        width: 140
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
    }
}
