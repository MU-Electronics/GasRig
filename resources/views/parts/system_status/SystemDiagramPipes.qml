import QtQuick 2.0

Item
{
    width: parent.width
    /**
     * 1st horizontal
     */
    SystemDiagramPipe
    {
        width: (parent.width - 514) + 350
        x: 35
        y: 43
    }

    /**
     * 2nd horizontal
     */
    SystemDiagramPipe
    {
        width: ((parent.width - 514) / 3.7) + 130
        x: 35
        y: 143
    }
    SystemDiagramPipe
    {
        width: (parent.width - 514) + 350
        x: 35
        y: 143
        opacity: GlobalManager.rigSettings["valve_4"]
    }
    SystemDiagramPipe
    {
        width: ((parent.width - 514) / 3.7) + 130
        x: ((parent.width - 514 + 12) + 320) / 1.3
        y: 143
    }


    /**
     * 2nd horizontal
     */
    SystemDiagramPipe
    {
        width: ((parent.width - 514) / 3.7) + 150
        x: ((parent.width - 514 + 12) + 320) / 1.3
        y: 243
        opacity: GlobalManager.rigSettings["valve_6"]
    }

    /**
     * 3rd horizontal
     */
    SystemDiagramPipe
    {
        width: ((parent.width - 514) / 3.7) + 120
        x: 43
        y: 344
    }
    // Flow controller 1
    SystemDiagramPipe
    {
        width: ((parent.width - 514) / 2) + 135
        x: ((parent.width - 514 + 12) + 555) / 3.7
        y: 344
        opacity: GlobalManager.rigSettings["flow_controller_1"]
    }
    // Flow controller 2
    SystemDiagramPipe
    {
        width: ((parent.width - 514) / 1.37) + 282
        x: ((parent.width - 514 + 12) + 555) / 3.7
        y: 344
        opacity: GlobalManager.rigSettings["flow_controller_2"]
    }







    /**
     * 1nd vertical
     */
    SystemDiagramPipe
    {
        height: 100
        x: 43
        y: 344
    }

    /**
     * 2nd vertical
     */
    SystemDiagramPipe
    {
        height: 400
        x: ((parent.width - 514 + 12) + 555) / 3.7
        y: 43
    }


    /**
     * 3rd vertical
     */
    SystemDiagramPipe
    {
        height: (GlobalManager.rigSettings["valve_6"]) ? 200 : 100
        x: ((parent.width - 514 + 12) + 320) / 1.3
        y: 43
    }

    /**
     * 4th vertical
     */
    SystemDiagramPipe
    {
        height: 100
        x: ((parent.width - 514) + 355) / 1.3
        y: 350
        opacity: GlobalManager.rigSettings["flow_controller_1"]
    }

    /**
     * 4th vertical
     */
    SystemDiagramPipe
    {
        height: 100
        x: ((parent.width - 514) + 420)
        y: 350
        opacity: GlobalManager.rigSettings["flow_controller_2"]
    }



    /**
     * Top pipe filter
     */
    Text{
        text: "F"
        font.pixelSize: 8
        font.bold: true
        color: "white"
        x: 140
        y: 45
    }
    Text{
        text: "F (slow)"
        font.pixelSize: 8
        font.bold: true
        color: "white"
        x: ((parent.width - 514) / 2) + 240
        y: 45
    }
    Text{
        text: "F (fast)"
        font.pixelSize: 8
        font.bold: true
        color: "white"
        x: ((parent.width - 514) / 2) + 240
        y: 145
        opacity: GlobalManager.rigSettings["valve_4"]
    }
    Text{
        text: "F"
        font.pixelSize: 8
        font.bold: true
        color: "white"
        x: 140
        y: 346
    }
    Text{
        text: "F"
        font.pixelSize: 8
        font.bold: true
        color: "white"
        x: ((systemDrawing.width - 514 ) / 3.7) + 158
        y: 360
    }
    Text{
        text: "F"
        font.pixelSize: 8
        font.bold: true
        color: "white"
        x: ((systemDrawing.width - 514 ) / 2) + 174
        y: 346
        opacity: GlobalManager.rigSettings["valve_8"]
    }
}
