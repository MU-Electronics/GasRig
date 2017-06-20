

/**
 * Draws all the pipes for the system
 */
function drawPipes(ctx, windowWidth)
{
    // Padding
    var widthPadding = (windowWidth-514);
    var leftPadding = 12;

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
    pipe(ctx, 31+leftPadding, 350, 100+leftPadding+widthPadding, 350);
    pipe(ctx, 100+leftPadding+widthPadding, 350, 200+leftPadding+widthPadding, 350);
    pipe(ctx, 200+leftPadding+widthPadding, 350, 350+leftPadding+widthPadding, 350);


    // First coloumn of vertial pipes
    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 50, 150+leftPadding+widthPadding / 3.75, 150);
    pipe(ctx, 250+leftPadding+widthPadding/1.3, 50, 250+leftPadding+widthPadding/1.3, 150);

    // Seconds coloumn of vertical pipes
    pipe(ctx, 250+leftPadding+widthPadding / 1.3, 150, 250+leftPadding+widthPadding / 1.3, 250);
    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 150, 150+leftPadding+widthPadding / 3.75, 350);

    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 400, 150+leftPadding+widthPadding / 3.75, 450);

    // Third colomn of vertical pipes
    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 350, 150+leftPadding+widthPadding / 3.75, 400);
    pipe(ctx, 275+leftPadding+widthPadding/1.3, 350, 275+leftPadding+widthPadding/1.3, 400);
    pipe(ctx, 342.5+leftPadding+widthPadding, 350, 342.5+leftPadding+widthPadding, 400);

    pipe(ctx, 47.5+leftPadding / 4, 350, 47.5+leftPadding / 4, 400);
}



/**
 * Draws all the valves for the system
 */
function drawValves(ctx, windowWidth, valve1, valve2, valve3, valve4, valve5, valve6, valve7, valve8, valve9)
{
    // Padding
    var widthScaling = (windowWidth-514);
    var leftPadding = 12;

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
function pressureSensor(ctx, windowWidth, pressure)
{
    var x = 20;
    var y = 120;
    var width = 115;
    var height = 60;
    var widthScaling = (windowWidth-514);

    ctx.beginPath();

    ctx.rect(x, y, width, height);

    ctx.fillStyle = '#d4d5d8';
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Pressure
    ctx.font = 'bold 15pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Pressure Sensor", x+55, y+25);

    // value
    ctx.font = 'bold 15pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText(parseFloat(pressure).toFixed(4), x+55, y+45);


    ctx.closePath();
}


/**
 * Draws the exhaust port
 */
function exhaustPort(ctx, windowWidth)
{
    var widthScaling = (windowWidth-514);

    var x = 363 + widthScaling;
    var y = 35;
    var width = 60;
    var height = 30;

    ctx.beginPath();

    ctx.rect(x, y, width, height);

    ctx.fillStyle = '#d4d5d8';
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Output
    ctx.font = 'bold 15pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Exhuast", x+29, y+20);


    ctx.closePath();
}


/**
 * Draws the output port
 */
function outputPort(ctx, windowWidth)
{
    var widthScaling = (windowWidth-514);

    var x = 2;
    var y = 35;
    var width = 60;
    var height = 30;

    ctx.beginPath();

    ctx.rect(x, y, width, height);

    ctx.fillStyle = '#d4d5d8';
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Output
    ctx.font = 'bold 15pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Output", x+28.5, y+20);


    ctx.closePath();
}


/**
 * Draws the exhaust port
 */
function vacuumOutPort(ctx, windowWidth)
{
    var widthScaling = (windowWidth-514);

    var x = 363 + widthScaling;
    var y = 235;
    var width = 90;
    var height = 30;

    ctx.beginPath();

    ctx.rect(x, y, width, height);

    ctx.fillStyle = '#d4d5d8';
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Output
    ctx.font = 'bold 15pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Vacuum Out", x+45, y+20);


    ctx.closePath();
}


/**
 * Draws the flow controller
 */
function flowController(ctx, windowWidth, setFlowRate, flowRate, valveOverride, softStart, softStartTime)
{

    var widthScaling = (windowWidth-514);

}


/**
 * Draw high pressure box
 */
function highPressureInput(ctx, windowWidth)
{
    var widthScaling = (windowWidth-514);

    var rectX = 10;
    var rectY = 395;
    var rectWidth = 80;
    var rectHeight = 120;
    var cornerRadius = 80;

    ctx.beginPath();

    ctx.lineJoin = "round";
    ctx.lineWidth = cornerRadius;

    // Fix colour
    ctx.fillStyle = '#d4d5d8';
    ctx.strokeStyle = '#c8cace';

    ctx.strokeRect(rectX+(cornerRadius/2), rectY+(cornerRadius/2), rectWidth-cornerRadius, rectHeight-cornerRadius);
    ctx.fillRect(rectX+(cornerRadius/2), rectY+(cornerRadius/2), rectWidth-cornerRadius, rectHeight-cornerRadius);

    // High
    ctx.font = 'bold 20pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("High", rectX+38, rectY+40);

    // Pressure
    ctx.font = 'bold 20pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Pressure", rectX+38, rectY+60);

    // Pressure
    ctx.font = 'bold 20pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Input", rectX+38, rectY+80);

    ctx.closePath();
}



/**
 * Draw high pressure box
 */
function nitrogenPressureInput(ctx, windowWidth)
{
    var widthScaling = (windowWidth-514);

    var rectX = 123+widthScaling/3.75;
    var rectY = 440;
    var rectWidth = 80;
    var rectHeight = 120;
    var cornerRadius = 80;

    ctx.beginPath();

    ctx.lineJoin = "round";
    ctx.lineWidth = cornerRadius;

    // Fix colour
    ctx.fillStyle = '#d4d5d8';
    ctx.strokeStyle = '#c8cace';

    ctx.strokeRect(rectX+(cornerRadius/2), rectY+(cornerRadius/2), rectWidth-cornerRadius, rectHeight-cornerRadius);
    ctx.fillRect(rectX+(cornerRadius/2), rectY+(cornerRadius/2), rectWidth-cornerRadius, rectHeight-cornerRadius);

    // High
    ctx.font = 'bold 20pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("High", rectX+38, rectY+40);

    // Pressure
    ctx.font = 'bold 20pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Pressure", rectX+38, rectY+60);

    // Pressure
    ctx.font = 'bold 20pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Nitrogen", rectX+38, rectY+80);

    ctx.closePath();
}



/**
 * Draws the vac station
 */
function vaccumStation(ctx, windowWidth, backingPump, turbo, vacuumPressure, gasMode, backingPumpMode)
{
    var widthScaling = (windowWidth-514);

    var x = 363 + widthScaling;
    var y = 100;
    var width = 140;
    var height = 100;

    ctx.beginPath();

    ctx.rect(x, y, width, height);

    // Calculate the background colour
    var backgroundVac = '#d4d5d8';
    var strokeVac = '#c8cace';
    if(backingPump && !turbo)
    {
        backgroundVac = '#e5960d';
        strokeVac = '#ce860a';
    }
    else if(backingPump && turbo)
    {
        backgroundVac = '#dd1a1a';
        strokeVac = '#ce0909';
    }

    ctx.fillStyle = backgroundVac;
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = strokeVac;
    ctx.stroke();

    // Output
    ctx.font = 'bold 15pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Vacuum Station", x+70, y+20);


    ctx.closePath();

}




