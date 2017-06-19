import QtQuick 2.0
import QtQuick.Controls 2.1
import "../../assets/js/QtCanvasHelper.js" as CanvasHelper


Item
{
    // Hold the select value for backing pump mode
    property bool bootup: false

    width: parent.width
    height: parent.height

    Canvas {
        id: systemDrawing

        width: parent.width
        height: parent.height /2

        function paint_canvas(){
            var ctx = systemDrawing.getContext('2d');

            // Draw pipes
            CanvasHelper.pipe(ctx, 50, 50, 100, 50);
            CanvasHelper.pipe(ctx, 100, 50, 200, 50);
            CanvasHelper.pipe(ctx, 200, 50, 300, 50);
            CanvasHelper.pipe(ctx, 300, 50, 350, 50);

            // Draw valves
            CanvasHelper.valve(ctx, 100, 50, SystemStatusManager.valveState[1]);
            CanvasHelper.valve(ctx, 200, 50, SystemStatusManager.valveState[2]);
            CanvasHelper.valve(ctx, 300, 50, SystemStatusManager.valveState[3]);

            //ctx.moveTo(110,75);
            systemDrawing.requestPaint();
        }

        function clear_canvas(){
            var ctx = systemDrawing.getContext('2d');
            //ctx.reset();
            //ctx.clearRect(0,0,50,50);
            CanvasHelper.valve(ctx, 50, 50, SystemStatusManager.valveState[1]);
            systemDrawing.requestPaint();
        }

        onWidthChanged: {
            // Dont draw on bootup
            if(bootup == true)
                systemDrawing.paint_canvas();

            // After inital bootup
            bootup = true;
        }

       onPaint: {
            systemDrawing.paint_canvas();
        }
    }
}
