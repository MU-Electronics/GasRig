import QtQuick 2.0
import QtQuick.Controls 2.1
import "../../assets/js/QtCanvasHelper.js" as CanvasHelper


Item
{

    Canvas {
        id:canvas
        width: 400
        height: 400
        onPaint:{
            var ctx = canvas.getContext('2d');

            CanvasHelper.valve(ctx, 50, 50, true);

            CanvasHelper.valve(ctx, 200, 50, false);

            ctx.moveTo(110,75);
        }
    }

    Button{

        text: 'change colour'

        onClicked: {
            console.log("hey hey")
            var ct = canvas.getContext('2d');
            ct.width = ct.width;
            ct.clearRect(0,0,300,300); // X, Y, Width, Height
            ct.clearRect(0, 0, ct.width, ct.height);
            ct.restore();

            CanvasHelper.valve(ct, 50, 50, false);
            CanvasHelper.valve(ct, 100, 50, false);
            canvas.requestPaint();
        }
    }

}
