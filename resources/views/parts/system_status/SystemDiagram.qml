import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import "../../../assets/js/QtCanvasHelper.js" as CanvasHelper


Item
{
    // Hold the select value for backing pump mode
    property bool bootup: false
    property bool drawInit: false

    width: parent.width
    height: parent.height

    // Redraw diagram on valve update
    Connections {
        target: SystemStatusManager
        onEmit_valveStateChanged: {
            systemDrawing.paint_canvas(parent.width, parent.height, true, false, false, false);
        }
        onEmit_pressureSensorChanged: {
            systemDrawing.paint_canvas(parent.width, parent.height, false, false, false, true);
        }
        onEmit_vacuumStateChanged: {
            systemDrawing.paint_canvas(parent.width, parent.height, false, true, false, false);
        }
        onEmit_flowControllerStateChanged: {
            systemDrawing.paint_canvas(parent.width, parent.height, false, false, true, false);
        }
    }

    Canvas {
        id: systemDrawing


        width: parent.width
        height: parent.height


        antialiasing: true
        transformOrigin: Item.TopLeft

        /**
         * Draw parts that never change
         */
        function paint_init(ctx)
        {
            // Set font scale
            CanvasHelper.setFontScale(Screen.pixelDensity.toFixed(2));

            // Draw pipes
            CanvasHelper.drawPipes(ctx, systemDrawing.width);

            // Draw output
            CanvasHelper.outputPort(ctx, systemDrawing.width);

            // Draw exhuast
            CanvasHelper.exhaustPort(ctx, systemDrawing.width);

            // Draw vac out
            CanvasHelper.vacuumOutPort(ctx, systemDrawing.width);

            // Draw high pressure
            CanvasHelper.highPressureInput(ctx, systemDrawing.width);

            // Draw nitrogen pressure
            CanvasHelper.nitrogenPressureInput(ctx, systemDrawing.width);

            // Draw filters
            CanvasHelper.filters(ctx, systemDrawing.width);
        }

        /**
         * Draw system rig
         */
        function paint_canvas(width, height, valves, vacuum, flow, pressure){
            var ctx = systemDrawing.getContext('2d');

            // Should we draw the none dynamic parts
            if(!drawInit)
            {
               systemDrawing.paint_init(ctx)
               drawInit = true;
            }

            // Draw valves
            if(valves)
                CanvasHelper.drawValves(ctx, systemDrawing.width, SystemStatusManager.valveState[1],
                                                                  SystemStatusManager.valveState[2],
                                                                  SystemStatusManager.valveState[3],
                                                                  SystemStatusManager.valveState[4],
                                                                  SystemStatusManager.valveState[5],
                                                                  SystemStatusManager.valveState[6],
                                                                  SystemStatusManager.valveState[7],
                                                                  SystemStatusManager.valveState[8],
                                                                  SystemStatusManager.valveState[9]);

            // Draw pressure sensor
            if(pressure)
                CanvasHelper.pressureSensor(ctx, systemDrawing.width, SystemStatusManager.pressureSensor["pressure"]);



            // Draw vac pump
            if(vacuum)
                CanvasHelper.vaccumStation(ctx, systemDrawing.width, SystemStatusManager.vacuumState["backing_pump"],
                                                                     SystemStatusManager.vacuumState["turbo_pump"],
                                                                     SystemStatusManager.vacuumState["vacuum"],
                                                                     SystemStatusManager.vacuumState["gas_type_mode"] ,
                                                                     SystemStatusManager.vacuumState["backing_pump_mode"]);



            // Draw flow controller one and two
            if(flow)
            {
                CanvasHelper.flowController(ctx, systemDrawing.width, 1, SystemStatusManager.flowControllerState["controller_1_set_flowrate"],
                                                                         SystemStatusManager.flowControllerState["controller_1_flow"],
                                                                         SystemStatusManager.flowControllerState["controller_1_override"],
                                                                         SystemStatusManager.flowControllerState["controller_1_softstart"],
                                                                         SystemStatusManager.flowControllerState["controller_1_softstart_time"]);

                CanvasHelper.flowController(ctx, systemDrawing.width, 2, SystemStatusManager.flowControllerState["controller_2_set_flowrate"],
                                                                         SystemStatusManager.flowControllerState["controller_2_flow"],
                                                                         SystemStatusManager.flowControllerState["controller_2_override"],
                                                                         SystemStatusManager.flowControllerState["controller_2_softstart"],
                                                                         SystemStatusManager.flowControllerState["controller_2_softstart_time"]);
            }


            // Repaint canvas
            systemDrawing.requestPaint();
        }

        function clear_canvas(){
            //var ctx = systemDrawing.getContext('2d');
            //ctx.reset();
            //ctx.clearRect(0,0,50,50);
            //CanvasHelper.valve(ctx, 50, 50, SystemStatusManager.valveState[1]);
            //systemDrawing.requestPaint();
        }

        onWidthChanged: {
            // Redraw the none dynamic parts
            drawInit = false;

            // Dont draw on bootup
            if(bootup == true)
                systemDrawing.paint_canvas(parent.width, parent.height, true, true, true, true);

            // After inital bootup
            bootup = true;
        }

       onPaint: {
            systemDrawing.paint_canvas(parent.width, parent.height, true, true, true, true);
        }
    }
}
