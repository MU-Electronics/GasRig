

function valve(ctx, x, y, state)
{
    var radius = 20;

    ctx.beginPath();

    // Cirlce
    ctx.arc(x, y, radius, 0, 2 * Math.PI, false);
    ctx.fillStyle = (state ? '#4CAF50' : '#F44336');
    ctx.fill();
    ctx.lineWidth = 5;
    ctx.strokeStyle = (state ? '#388E3C' : '#D32F2F');
    ctx.stroke();

    //Valve number
    ctx.font = 'bold 14pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = 'white';
    ctx.fillText('V1', x, y-2);

    //open close
    ctx.font = 'bold 12pt Calibri';
    ctx.textAlign = 'center';
    ctx.fillStyle = 'white';
    ctx.fillText('open', x, y+8);

    ctx.closePath();
}


