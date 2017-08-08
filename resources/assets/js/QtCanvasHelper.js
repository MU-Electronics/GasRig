
/**
 * Set the disired fornt
 */
var font = "Arial"; // Calibri

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
    ctx.font = 'bold '+fontSize1+'pt '+font;
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("High", rectX+40, rectY+40);

    // Pressure
    ctx.font = 'bold '+fontSize1+'pt '+font;
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Pressure", rectX+40, rectY+60);

    // Pressure
    ctx.font = 'bold '+fontSize1+'pt '+font;
    ctx.textAlign = 'center';
    ctx.fillStyle = '#5e5f63';
    ctx.fillText("Nitrogen", rectX+40, rectY+80);

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
    ctx.font = 'bold '+fontSize1+'pt '+font;
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
    ctx.font = 'bold '+fontSize2+'pt '+font;
    ctx.fillText("(fast)", 255+widthScaling/1.8, 55);
    ctx.fillText("(slow)", 255+widthScaling/1.8, 155);

}

