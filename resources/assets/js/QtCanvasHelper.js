
/**
 * Sets a font scale of the monitors pixel density ratio
 */
var fontScale = 1;
function setFontScale(scale)
{
    fontScale = scale;
}


/**
 * Determins a font size depening on the monitor font density
 */
function calcFontSize(ptFont)
{
    // Take a referance of
    var ref = 4.29;

    // Place holder for our new font size
    var scaledPtFont = ptFont;

    // Take the different of our font scale
    if(fontScale >= 4.29)
    { // Larger monitor resolution
        var scaleHigh = fontScale - ref;
        scaledPtFont = ptFont + (( ptFont * scaleHigh )*0.42);
    }
    else if(fontScale >= 3)
    { // medium monitor resolution
        var scaleMedium = ref - fontScale;
        scaledPtFont = ptFont - (( ptFont * scaleMedium )*0.42);
    }
    else if(fontScale >= 2)
    { // Smaller monitor resolution
        var scaleLow = ref - fontScale;
        scaledPtFont = (( ptFont * scaleLow ) - ptFont) * 1.8;
    }

    return Math.abs(scaledPtFont);
}

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
    pipe(ctx, 100+leftPadding+widthPadding, 350, 250+leftPadding+widthPadding, 350);
    pipe(ctx, 200+leftPadding+widthPadding, 350, 400+leftPadding+widthPadding, 350);


    // First coloumn of vertial pipes
    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 50, 150+leftPadding+widthPadding / 3.75, 150);
    pipe(ctx, 250+leftPadding+widthPadding/1.3, 50, 250+leftPadding+widthPadding/1.3, 150);

    // Seconds coloumn of vertical pipes
    pipe(ctx, 250+leftPadding+widthPadding / 1.3, 150, 250+leftPadding+widthPadding / 1.3, 250);
    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 150, 150+leftPadding+widthPadding / 3.75, 350);

    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 400, 150+leftPadding+widthPadding / 3.75, 450);

    // Third colomn of vertical pipes
    pipe(ctx, 150+leftPadding+widthPadding / 3.75, 350, 150+leftPadding+widthPadding / 3.75, 400);
    pipe(ctx, 295+leftPadding+widthPadding/1.3, 350, 295+leftPadding+widthPadding/1.3, 400);
    pipe(ctx, 392.5+leftPadding+widthPadding, 350, 392.5+leftPadding+widthPadding, 400);

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

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);
    var fontSize2 = calcFontSize(12);

    //Valve number
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = 'white';
    ctx.fillText(number, x, y-2);

    //open close
    ctx.font = 'bold '+fontSize2+'pt Calibri';
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

    // Fix colour
    //ctx.fillStyle = '#d4d5d8';
    //ctx.strokeStyle = '#c8cace';


    var colorBackground = "#"+getColorForPercentage(pressure/100);
    var colorStoke = colorLuminance("#"+colorBackground, -0.005);
    var colorText = getColorByBgColor(colorBackground);

    ctx.fillStyle = colorBackground;
    ctx.strokeStyle = colorText;

    ctx.rect(x, y, width, height);

    ctx.fill();

    ctx.lineWidth = 4;
    ctx.stroke();

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);

    // Pressure
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    //ctx.fillStyle = colorLuminance(colorText, -0.2);
    ctx.fillStyle = colorText;
    ctx.fillText("Pressure Sensor", x+57, y+25);

    // value
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = colorText;
    ctx.fillText(parseFloat(pressure).toFixed(4)+" Bar", x+55, y+45);


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

    // Fix colour
    ctx.fillStyle = '#d4d5d8';
    ctx.strokeStyle = '#c8cace';

    ctx.rect(x, y, width, height);

    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);

    // Output
    ctx.font = 'bold '+fontSize1+'pt Calibri';
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

    // Fix colour
    ctx.fillStyle = '#d4d5d8';
    ctx.strokeStyle = '#c8cace';

    ctx.rect(x, y, width, height);

    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);

    // Output
    ctx.font = 'bold '+fontSize1+'pt Calibri';
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

    // Fix colour
    ctx.fillStyle = '#d4d5d8';
    ctx.strokeStyle = '#c8cace';

    ctx.rect(x, y, width, height);

    ctx.fillStyle = '#d4d5d8';
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = '#c8cace';
    ctx.stroke();

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);

    // Output
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Vacuum Out", x+45, y+20);


    ctx.closePath();
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

    // Scale the font sizes
    var fontSize1 = calcFontSize(19);

    // High
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("High", rectX+40, rectY+40);

    // Pressure
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Pressure", rectX+40, rectY+60);

    // Pressure
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Input", rectX+40, rectY+80);

    ctx.closePath();
}



/**
 * Draw high pressure box
 */
function nitrogenPressureInput(ctx, windowWidth)
{
    var widthScaling = (windowWidth-514);

    var rectX = 121.5+widthScaling/3.75;
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

    // Scale the font sizes
    var fontSize1 = calcFontSize(19);

    // High
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("High", rectX+40, rectY+40);

    // Pressure
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Pressure", rectX+40, rectY+60);

    // Pressure
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Nitrogen", rectX+40, rectY+80);

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
        backgroundVac = '#FF9800';
        strokeVac = '#FB8C00';
    }
    else if(backingPump && turbo)
    {
        backgroundVac = '#EF5350';
        strokeVac = '#E53935';
    }

    ctx.fillStyle = backgroundVac;
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = strokeVac;
    ctx.stroke();

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);
    var fontSize2 = calcFontSize(12);

    // Output
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Vacuum Station", x+70, y+20);

    // Turbo
    ctx.font = 'bold '+fontSize2+'pt Calibri';
    ctx.fillText("Turbo: "+(turbo ? 'enabled' : 'disabled'), x+70, y+45);

    // Vacuum pressure
    ctx.fillText("Vacuum: "+vacuumPressure.toExponential(3)+" mBar", x+70, y+65);

    // Gas mode
    var mode;
    if(gasMode === 0) { mode = '>= 39'; }else if(gasMode === 1) { mode = '< 39'; }else if(gasMode === 2) { mode = 'helium'; }
    ctx.fillText("Gas Mode: "+mode, x+70, y+85);

    ctx.closePath();

}


/**
 * Draws the flow controller
 */
function flowController(ctx, windowWidth, id, setFlowRate, flowRate, valveOverride, softStart, softStartTime)
{
    var widthScaling = (windowWidth-514);

    // Set id 1 if not 2
    var x = 210 + widthScaling / 1.3;
    var y = 395;
    if(id === 2)
    {
        x = 360 + widthScaling;
        y = 395;
    }


    var width = 140;
    var height = 170;

    ctx.beginPath();

    ctx.rect(x, y, width, height);

    var background = '#d4d5d8';
    var stroke = '#c8cace';
    if(valveOverride === 1)
    {
        background = '#EF5350';
        stroke = '#E53935';
    }
    else if(setFlowRate > 0)
    {
        background = '#FF9800';
        stroke = '#FB8C00';
    }

    ctx.fillStyle = background;
    ctx.fill();

    ctx.lineWidth = 4;
    ctx.strokeStyle = stroke;
    ctx.stroke();

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);
    var fontSize2 = calcFontSize(12);

    // Flow Controller ID
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Flow Controller "+id, x+70, y+20);

    // Flow
    ctx.font = 'bold '+fontSize2+'pt Calibri';
    ctx.fillText("Flow: " + flowRate.toExponential(3) + "ml/min", x+70, y+45);

    // Set flow
    ctx.fillText("Set: " + setFlowRate.toExponential(3) + "ml/min", x+70, y+65);

    // Soft start state
    ctx.fillText("Soft Start: " + (softStart === 4 ? 'enabled' : 'disabled'), x+70, y+95);

    // Soft start time
    ctx.fillText("Soft Start time: " + softStartTime + "s", x+70, y+115);

    // Valve override
    var overrideVerbal;
    if(valveOverride === 0){ overrideVerbal = "off"; } else if(valveOverride === 1){ overrideVerbal = "open"; } else if(valveOverride === 2){ overrideVerbal = "closed"; }
    ctx.fillText("Override: " + overrideVerbal, x+70, y+145);

    ctx.closePath();
}



/**
 * Draw the filters
 */
function filters(ctx, windowWidth)
{
    var widthScaling = (windowWidth-514);

    // Scale the font sizes
    var fontSize1 = calcFontSize(14);
    var fontSize2 = calcFontSize(9);

    // Set style
    ctx.font = 'bold '+fontSize1+'pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = '#ffffff';

    // Draw filter
    ctx.fillText("F", 240+widthScaling/1.8, 55);

    // Draw filter
    ctx.fillText("F", 240+widthScaling/1.8, 155);

    // Draw filter
    ctx.fillText("F", 144, 355);

    // Draw filter
    ctx.fillText("F", 164+widthScaling/3.8, 373);

    // Description
    ctx.font = 'bold '+fontSize2+'pt Calibri';
    ctx.fillText("(fast)", 255+widthScaling/1.8, 55);
    ctx.fillText("(slow)", 255+widthScaling/1.8, 155);

}

















/**
 * Convets number to hex
 */
function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
}

/**
 * Convets percentage to colour
 */
function getColorForPercentage(pct)
{
    var percentColors = [
        { pct: 0.0, color: { r: 0x00, g: 0xff, b: 0 } },
        { pct: 0.5, color: { r: 0xff, g: 0xff, b: 0 } },
        { pct: 1.0, color: { r: 0xff, g: 0x00, b: 0 } } ];

    for (var i = 1; i < percentColors.length - 1; i++) {
        if (pct < percentColors[i].pct) {
            break;
        }
    }
    var lower = percentColors[i - 1];
    var upper = percentColors[i];
    var range = upper.pct - lower.pct;
    var rangePct = (pct - lower.pct) / range;
    var pctLower = 1 - rangePct;
    var pctUpper = rangePct;
    var color = {
        r: Math.floor(lower.color.r * pctLower + upper.color.r * pctUpper),
        g: Math.floor(lower.color.g * pctLower + upper.color.g * pctUpper),
        b: Math.floor(lower.color.b * pctLower + upper.color.b * pctUpper)
    };
    //return 'rgb(' + [color.r, color.g, color.b].join(',') + ')';
    return componentToHex(color.r)+componentToHex(color.g)+componentToHex(color.b);
}


/**
 * Lightens or darkens a colour
 */
function colorLuminance(color, percent) {

    var num = parseInt(color,16),
        amt = Math.round(2.55 * percent),
        R = (num >> 16) + amt,
        B = (num >> 8 & 0x00FF) + amt,
        G = (num & 0x0000FF) + amt;

    return "#"+(0x1000000 + (R<255?R<1?0:R:255)*0x10000 + (B<255?B<1?0:B:255)*0x100 + (G<255?G<1?0:G:255)).toString(16).slice(1);

}

/**
 * Get correct text color for background
 */
function getColorByBgColor(bgColor) {
    if (!bgColor) { return ''; }
    return (parseInt(bgColor.replace('#', ''), 16) > 0xffffff / 2) ? '#000' : '#fff';
}
