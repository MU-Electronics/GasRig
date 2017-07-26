import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import "../../../assets/js/QtCanvasHelper.js" as CanvasHelper


Item
{
    // Hold the select value for backing pump mode
    property bool bootup: false

    width: parent.width
    height: parent.height

    // Redraw diagram on valve update
    Connections {
        target: SystemStatusManager
        onEmit_valveStateChanged: {
            systemDrawing.paint_canvas();
        }
        onEmit_pressureSensorChanged: {
            systemDrawing.paint_canvas();
        }
        onEmit_vacuumStateChanged: {
            systemDrawing.paint_canvas();
        }
        onEmit_flowControllerStateChanged: {
            systemDrawing.paint_canvas();
        }
    }

    Canvas {
        id: systemDrawing


        width: parent.width
        height: parent.height


        antialiasing: true
        transformOrigin: Item.TopLeft

        function paint_canvas(width, height){
            var ctx = systemDrawing.getContext('2d');

            // Set font scale
            CanvasHelper.setFontScale(Screen.pixelDensity.toFixed(2));

            // Draw pipes
            CanvasHelper.drawPipes(ctx, systemDrawing.width);

            // Draw valves
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
            CanvasHelper.pressureSensor(ctx, systemDrawing.width, SystemStatusManager.pressureSensor["pressure"]);

            // Draw output
            CanvasHelper.outputPort(ctx, systemDrawing.width);

            // Draw exhuast
            CanvasHelper.exhaustPort(ctx, systemDrawing.width);

            // Draw vac out
            CanvasHelper.vacuumOutPort(ctx, systemDrawing.width);

            // Draw vac pump
            CanvasHelper.vaccumStation(ctx, systemDrawing.width, SystemStatusManager.vacuumState["backing_pump"],
                                                                 SystemStatusManager.vacuumState["turbo_pump"],
                                                                 SystemStatusManager.vacuumState["vacuum"],
                                                                 SystemStatusManager.vacuumState["gas_type_mode"] ,
                                                                 SystemStatusManager.vacuumState["backing_pump_mode"]);

            // Draw high pressure
            CanvasHelper.highPressureInput(ctx, systemDrawing.width);

            // Draw nitrogen pressure
            CanvasHelper.nitrogenPressureInput(ctx, systemDrawing.width);

            // Draw flow controller one
            CanvasHelper.flowController(ctx, systemDrawing.width, 1, SystemStatusManager.flowControllerState["controller_1_set_flowrate"],
                                                                     SystemStatusManager.flowControllerState["controller_1_flow"],
                                                                     SystemStatusManager.flowControllerState["controller_1_override"],
                                                                     SystemStatusManager.flowControllerState["controller_1_softstart"],
                                                                     SystemStatusManager.flowControllerState["controller_1_softstart_time"]);

            // Draw flow controller two
            CanvasHelper.flowController(ctx, systemDrawing.width, 2, SystemStatusManager.flowControllerState["controller_2_set_flowrate"],
                                                                     SystemStatusManager.flowControllerState["controller_2_flow"],
                                                                     SystemStatusManager.flowControllerState["controller_2_override"],
                                                                     SystemStatusManager.flowControllerState["controller_2_softstart"],
                                                                     SystemStatusManager.flowControllerState["controller_2_softstart_time"]);

            // Draw filters
            CanvasHelper.filters(ctx, systemDrawing.width);

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
            // Dont draw on bootup
            if(bootup == true)
                systemDrawing.paint_canvas(parent.width, parent.height);

            // After inital bootup
            bootup = true;
        }

       onPaint: {
            systemDrawing.paint_canvas(parent.width, parent.height);
        }
    }
}
