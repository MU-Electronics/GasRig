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
            //systemDrawing.paint_canvas(parent.width, parent.height, true, false, false, false);
        }
        onEmit_pressureSensorChanged: {
            //systemDrawing.paint_canvas(parent.width, parent.height, false, false, false, true);
        }
        onEmit_vacuumStateChanged: {
            //systemDrawing.paint_canvas(parent.width, parent.height, false, true, false, false);
        }
        onEmit_flowControllerStateChanged: {
            //systemDrawing.paint_canvas(parent.width, parent.height, false, false, true, false);
        }
    }

    Canvas
    {
        id: systemDrawing

        width: parent.width
        height: parent.height

        antialiasing: true
        transformOrigin: Item.TopLeft
        renderStrategy: Canvas.Threaded
        //renderTarget: Canvas.FramebufferObject


        /**
         * Draw the pressure value on the diagram
         * NOTE: Far more efficent than redrawing using Canvas!!!
         */
        Text{
            text:SystemStatusManager.pressureSensor["pressure"]+ " Bar"
            horizontalAlignment: Text.AlignHCenter
            color: "#5e5f63"
            //x: systemDrawing.width - 514 + 200
            x: 40
            y: 155
        }

        /**
         * Draw text for vacuum station
         * NOTE: Far more efficent than redrawing using Canvas!!!
         */
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

            color: whichColorBackground(SystemStatusManager.vacuumState["backing_pump"], SystemStatusManager.pressureSensor["turbo_pump"])
            border.color: whichColorBorder(SystemStatusManager.vacuumState["backing_pump"], SystemStatusManager.pressureSensor["turbo_pump"])
            border.width: 5
            width: 145
            height: 105
            x: systemDrawing.width - 514 + 360
            y: 97

            Text{
                text:   "Turbo: " + SystemStatusManager.pressureSensor["turbo_pump"] + "\n" +
                        "Gas Mode: " + SystemStatusManager.vacuumState["gas_type_mode_verbal"] + "\n" +
                        "Vacuum: " + SystemStatusManager.vacuumState["vacuum_round_3"];

                horizontalAlignment: Text.AlignHCenter
                color: "#5e5f63"

            }
        }

        /**
         * Fraw values for slow controller
         * NOTE: Far more efficent than redrawing using Canvas!!!
         */
        Text{
            text:   "Flow: " + SystemStatusManager.pressureSensor["controller_1_flow"] + "\n" +
                    "Set: " + SystemStatusManager.vacuumState["controller_1_set_flowrate"] + "\n" +
                    "Soft Start: " + SystemStatusManager.vacuumState["controller_1_softstart"] + "\n" +
                    "Soft Start time: " + SystemStatusManager.vacuumState["controller_1_softstart_time"] + "\n" +
                    "Override: " + SystemStatusManager.vacuumState["controller_1_override"];

            horizontalAlignment: Text.AlignHCenter
            color: "#5e5f63"
            x: ((systemDrawing.width - 514) + 270) / 1.3
            y: 430
        }

        Text{
            text:   "Flow: " + SystemStatusManager.pressureSensor["controller_2_flow"] + "\n" +
                    "Set: " + SystemStatusManager.vacuumState["controller_2_set_flowrate"] + "\n" +
                    "Soft Start: " + SystemStatusManager.vacuumState["controller_2_softstart"] + "\n" +
                    "Soft Start time: " + SystemStatusManager.vacuumState["controller_2_softstart_time"] + "\n" +
                    "Override: " + SystemStatusManager.vacuumState["controller_2_override"];

            horizontalAlignment: Text.AlignHCenter
            color: "#5e5f63"
            x: ((systemDrawing.width - 514) + 360)
            y: 430
        }

        /**
         * Draw valve one
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
        }
        SystemDiagramValve{
            number: 9
            x: ((systemDrawing.width - 514 + 12) + 485) / 3.7
            y: 375
        }




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

            // Draw pressure sensor
            CanvasHelper.pressureSensor(ctx, systemDrawing.width);

            // Draw vac pump
            CanvasHelper.vaccumStation(ctx, systemDrawing.width, SystemStatusManager.vacuumState["backing_pump"],
                                                                 SystemStatusManager.vacuumState["turbo_pump"],
                                                                 SystemStatusManager.vacuumState["vacuum"],
                                                                 SystemStatusManager.vacuumState["gas_type_mode"] ,
                                                                 SystemStatusManager.vacuumState["backing_pump_mode"]);

            // Draw flow controller one and two
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

            // Draw valves
            CanvasHelper.drawValves(ctx, systemDrawing.width);
        }

        /**
         * Draw system rig
         */
        function paint_canvas(width, height)
        {
            //Get the context
            var ctx = systemDrawing.getContext('2d');

            // Should we draw the none dynamic parts
            if(!drawInit)
            {
               systemDrawing.paint_init(ctx)
               drawInit = true;
            }

            // Repaint canvas
            systemDrawing.requestPaint();
        }

        onWidthChanged:
        {
            // Redraw the none dynamic parts
            drawInit = false;

            // Dont draw on bootup
            if(bootup == true)
                systemDrawing.paint_canvas(parent.width, parent.height);

            // After inital bootup
            bootup = true;
        }

       onPaint:
       {
           // Draw the canvas
            systemDrawing.paint_canvas(parent.width, parent.height);
       }
    }
}
