

/**
 * Draws all the pipes for the system
 */
function drawPipes(ctx, windowWidth)
{
    // Padding
    var widthPadding = (windowWidth-514);
    var leftPadding = 47;

    // Top row of horizonal pipes
    pipe(ctx, 50+leftPadding, 50, 100+leftPadding, 50);
    pipe(ctx, 100+leftPadding, 50, 200+leftPadding+widthPadding / 2 , 50);
    pipe(ctx, 200+leftPadding+widthPadding / 2, 50, 300+leftPadding+widthPadding, 50);
    pipe(ctx, 300+leftPadding+widthPadding, 50, 350+leftPadding+widthPadding, 50);

    // 2nd row of horizonal pipes
    pipe(ctx, 100+leftPadding, 150, 200+leftPadding+widthPadding, 150);
    pipe(ctx, 200+leftPadding+widthPadding, 150, 300+leftPadding+widthPadding, 150);
    pipe(ctx, 300+leftPadding+widthPadding, 150, 350+leftPadding+widthPadding, 150);

    // 3nd row of horizonal pipes
    pipe(ctx, 250+leftPadding+widthPadding / 1.3, 250, 350+leftPadding+widthPadding, 250);

    // 4nd row of horizonal pipes
    pipe(ctx, 50+leftPadding, 350, 100+leftPadding+widthPadding, 350);
    pipe(ctx, 100+leftPadding+widthPadding, 350, 200+leftPadding+widthPadding, 350);
    pipe(ctx, 200+leftPadding+widthPadding, 350, 350+leftPadding+widthPadding, 350);


    // First coloumn of vertial pipes
    pipe(ctx, 150+leftPadding+widthPadding / 4, 50, 150+leftPadding+widthPadding / 4, 150);
    pipe(ctx, 250+leftPadding+widthPadding/1.3, 50, 250+leftPadding+widthPadding/1.3, 150);

    // Seconds coloumn of vertical pipes
    pipe(ctx, 250+leftPadding+widthPadding / 1.3, 150, 250+leftPadding+widthPadding / 1.3, 250);
    pipe(ctx, 150+leftPadding+widthPadding / 4, 150, 150+leftPadding+widthPadding / 4, 350);

    // Third colomn of vertical pipes
    pipe(ctx, 150+leftPadding+widthPadding / 4, 350, 150+leftPadding+widthPadding / 4, 400);
    pipe(ctx, 275+leftPadding+widthPadding / 4, 350, 275+leftPadding+widthPadding / 4, 400);
    pipe(ctx, 342.5+leftPadding+widthPadding / 4, 350, 342.5+leftPadding+widthPadding / 4, 400);

}



/**
 * Draws all the valves for the system
 */
function drawValves(ctx, windowWidth, valve1, valve2, valve3, valve4, valve5, valve6, valve7, valve8, valve9)
{
    // Padding
    var widthScaling = (windowWidth-514);
    var leftPadding = 47;

    // First row of valves
    valve(ctx, 100+leftPadding, 50, valve1, "V1");
    valve(ctx, 200+leftPadding+widthScaling / 2, 50, valve2, "V2");
    valve(ctx, 300+leftPadding+widthScaling, 50, valve3, "V3");

    // 2nd row of valves
    valve(ctx, 200+leftPadding+widthScaling / 2, 150, valve4, "V4");
    valve(ctx, 300+leftPadding+widthScaling, 150, valve5, "V5");

    // 3rd row of valves
    valve(ctx, 250+leftPadding+widthScaling/1.3, 250, valve6, "V6");

    // 4th row of valves
    valve(ctx, 100+leftPadding, 350, valve7, "V7");
    valve(ctx, 200+leftPadding+widthScaling / 2, 350, valve8, "V8");

    // 5th row of valves
    valve(ctx, 150+leftPadding+widthScaling/3.7, 400, valve9, "V9");
}


/**
 * Draws a valve symbol in the correct postion and state
 */
function valve(ctx, x, y, state, number)
{
    var radius = 20;

    ctx.beginPath();

    // Cirlce
    ctx.arc(x, y, radius, 0, 2 * Math.PI, false);
    ctx.fillStyle = (state ? '#F44336' : '#4CAF50');
    ctx.fill();
    ctx.lineWidth = 5;
    ctx.strokeStyle = (state ? '#D32F2F' : '#388E3C');
    ctx.stroke();

    //Valve number
    ctx.font = 'bold 14pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = 'white';
    ctx.fillText(number, x, y-2);

    //open close
    ctx.font = 'bold 12pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = 'white';
    ctx.fillText((state ? 'open' : 'closed'), x, y+8);

    ctx.closePath();
}


/**
 * Draws a pipe from X,Y to X,Y
 */
function pipe(ctx, x, y, tox, toy)
{
    var radius = 20;

    ctx.beginPath();

    ctx.moveTo(x, y);
    ctx.lineTo(tox, toy);
    ctx.lineWidth = 15;

    ctx.strokeStyle = '#6a6b6d';
    ctx.stroke();

    ctx.closePath();

}


/**
 * Draws the pressure sensor
 */
function pressureSensor(ctx, pressure)
{
    var x = 50;
    var y = 145.5;
    var width = 100;
    var height = 90;

    ctx.beginPath();

    ctx.rect(x, y, width, height);

    ctx.fillStyle = '#d4d5d8';
    ctx.fill();

    ctx.lineWidth = 7;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Pressure
    ctx.font = 'bold 18pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Pressure", x+45, y+25);

    // Sensor
    ctx.font = 'bold 18pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Sensor", x+45, y+45);

    // value
    ctx.font = 'bold 18pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText(parseFloat(pressure).toFixed(4), x+45, y+75);


    ctx.closePath();
}



/**
 * Draws the vac station
 */
function vaccumStation(ctx, x, y, state, turbo, vacuumPressure, gasMode)
{


}


/**
 * Draws the exhaust port
 */
function exhaustPort(ctx, x, y)
{


}


/**
 * Draws the output port
 */
function outputPort(ctx, x, y)
{


}


/**
 * Draws the flow controller
 */
function flowController(ctx, x, y, setFlowRate, flowRate, valveOverride, softStart, softStartTime)
{


}



