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
        SystemDiagramPressureSensor{
            x: 0
            y: 120
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
        }

        SystemDiagramFlowController{
            controller: 2
            x: ((systemDrawing.width - 514) + 360)
            y: 395
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
            x: ((systemDrawing.width - 514 + 12) + 490) / 3.7
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
