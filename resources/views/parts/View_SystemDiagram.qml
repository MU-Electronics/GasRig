import QtQuick 2.0
import QtQuick.Controls 2.1
import "../../assets/js/QtCanvasHelper.js" as CanvasHelper


Item
{

    Canvas {
        id:canvas
        width: 400
        height: 400
        function paint_canvas(){
            var ctx = canvas.getContext('2d');

            CanvasHelper.valve(ctx, 50, 50, SystemStatusManager.valves[1]);

            CanvasHelper.valve(ctx, 200, 50, SystemStatusManager.valves[2]);

            //ctx.moveTo(110,75);
            canvas.requestPaint();
        }

        function clear_canvas(){
            var ctx = canvas.getContext('2d');
            //ctx.reset();
            //ctx.clearRect(0,0,50,50);
            CanvasHelper.valve(ctx, 50, 50, SystemStatusManager.valves[1]);
            canvas.requestPaint();
        }
    }

    Row{
        Button{
            text: 'Clear Canvas'
            onClicked: {
                canvas.clear_canvas();
            }
        }
        Button{
            text: 'Draw Canvas'
            onClicked: {
                canvas.paint_canvas();
            }
        }
    }

}
