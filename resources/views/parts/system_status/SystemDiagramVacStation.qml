import QtQuick 2.0

Rectangle
{
    function whichColorBackground(backingPump, turbo)
    {
        if(backingPump && !turbo)
        {
            return '#FF9800';
        }
        else if(backingPump && turbo)
        {
            return '#EF5350';
        }
        else
        {
            return '#d4d5d8';
        }
    }

    function whichColorBorder(backingPump, turbo)
    {
        if(backingPump && !turbo)
        {
            return '#FB8C00';
        }
        else if(backingPump && turbo)
        {
            return '#E53935';
        }
        else
        {
            return '#c8cace';
        }
    }

    color: whichColorBackground(SystemStatusManager.vacuumState["backing_pump"], SystemStatusManager.vacuumState["turbo_pump"])
    border.color: whichColorBorder(SystemStatusManager.vacuumState["backing_pump"], SystemStatusManager.vacuumState["turbo_pump"])
    border.width: 5
    width: 145
    height: 105

    Text{
        text: "Vac Station"
        color: "#5e5f63"
        font.bold: true
        font.pixelSize: 15
        horizontalAlignment: Text.AlignHCenter
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 35
    }

    Text{
        text:   "Turbo: " + SystemStatusManager.vacuumState["turbo_pump_verbal"] + "\n" +
                "Gas Mode: " + SystemStatusManager.vacuumState["gas_type_mode_verbal"] + "\n" +
                "Vacuum: " + SystemStatusManager.vacuumState["vacuum_round_3"];
        horizontalAlignment: Text.AlignHCenter
        color: "#5e5f63"
        anchors.top: parent.top
        anchors.topMargin: 25
        anchors.left: parent.left
        anchors.leftMargin: 7

    }
}
