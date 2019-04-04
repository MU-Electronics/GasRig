import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import "../../../assets/js/QtCanvasHelper.js" as CanvasHelper


Item
{
    id: systemDrawing

    width: parent.width
    height: parent.height


    /**
     * Draw the system pipes
     */
    SystemDiagramPipes{

    }

    /**
     * Draw the output port
     */
    SystemDiagramBoxedText{
        x: 2
        y: 35
        width: 65
        height: 30
        message: "Output"
        texttop: 5
        textleft:5
    }

    /**
     * Draw the exhuast port
     */
    SystemDiagramBoxedText{
        x: (systemDrawing.width - 514) + 360
        y: 35
        width: 70
        height: 30
        message: "Exhuast"
        texttop: 5
        textleft:5
    }


    /**
     * Draw the vacuum port
     */
    SystemDiagramBoxedText{
        x: (systemDrawing.width - 514) + 360
        y: 235
        width: 128
        height: 30
        message: "Vacuum Output"
        texttop: 5
        textleft:5
        opacity: GlobalManager.rigSettings["valve_6"]
    }

    /**
     * Draw the high pressure input
     */
    SystemDiagramBoxedText{
        x: 5
        y: 390
        width: 90
        height: 130
        message: "High\nPressure\nInput"
        texttop: 35
        textleft:10
        radius: 45;
    }


    /**
     * Draw the high pressure nitrogen input
     */
    SystemDiagramBoxedText{
        x: (systemDrawing.width - 514 + 435) / 3.75;
        y: 440
        width: 90
        height: 130
        message: "High\nPressure\nNitrogen"
        texttop: 35
        textleft:10
        radius: 45;
    }


    /**
     * Draw the pressure value on the diagram
     */
    SystemDiagramPressureSensor{
        x: 0
        y: 125
    }

    /**
     * Draw text for vacuum station
     */
    SystemDiagramVacStation{
        x: systemDrawing.width - 514 + 360
        y: 97
    }


    /**
     * Draw flow controllers
     */
    SystemDiagramFlowController{
        controller: 1
        x: ((systemDrawing.width - 514) + 273) / 1.3
        y: 395
        opacity: GlobalManager.rigSettings["flow_controller_1"]
    }

    SystemDiagramFlowController{
        controller: 2
        x: ((systemDrawing.width - 514) + 360)
        y: 395
        opacity: GlobalManager.rigSettings["flow_controller_2"]
    }


    /**
     * Draw all the valves
     */
    SystemDiagramValve{
        number: 1
        x: 85
        y: 25
    }
    SystemDiagramValve{
        number: 2
        x: (systemDrawing.width - 514 + 12 + 360) / 2
        y: 25
    }
    SystemDiagramValve{
        number: 3
        x: ((systemDrawing.width - 514 + 12) + 275)
        y: 25
    }
    SystemDiagramValve{
        number: 4
        x: ((systemDrawing.width - 514 + 12) + 360) / 2
        y: 125
        opacity: GlobalManager.rigSettings["valve_4"]
    }
    SystemDiagramValve{
        number: 5
        x: ((systemDrawing.width - 514 + 12) + 275)
        y: 125
    }
    SystemDiagramValve{
        number: 6
        x: ((systemDrawing.width - 514 + 12) + 300) / 1.3
        y: 225
        opacity: GlobalManager.rigSettings["valve_6"]
    }
    SystemDiagramValve{
        number: 7
        x: 85
        y: 325
    }
    SystemDiagramValve{
        number: 8
        x: ((systemDrawing.width - 514 + 12) + 360) / 2
        y: 325
        opacity: GlobalManager.rigSettings["valve_8"]
    }
    SystemDiagramValve{
        number: 9
        x: ((systemDrawing.width - 514 + 12) + 490) / 3.7
        y: 375
    }
}
