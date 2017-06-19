import QtQuick 2.0
import QtQuick.Controls 2.1
import QtQuick.Window 2.2
import "../../assets/js/QtCanvasHelper.js" as CanvasHelper


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
    }

    Canvas {
        id: systemDrawing


        width: parent.width
        height: parent.height


        antialiasing: true
        transformOrigin: Item.TopLeft

        function paint_canvas(width, height){
            var ctx = systemDrawing.getContext('2d');

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
