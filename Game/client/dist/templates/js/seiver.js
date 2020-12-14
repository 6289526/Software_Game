let pDeviceOrientation;

function setup(){
    createCanvas(windowWidth, windowHeight);

    background(0);
    colorMode(HSB, 360, 100, 100, 100);

    pDeviceOrientation = deviceOrientation;
}

function draw(){

    // if(pDeviceOrientation !== undefined && pDeviceOrientation !== deviceOrientation){

    //     noCanvas();
    //     createCanvas(window.innerWidth, windowHeight);
    // }
    // pDeviceOrientation = deviceOrientation;

    blendMode(BLEND);
    background(0, 5);
    blendMode(ADD);

    translate(windowWidth / 2, windowHeight / 2);

    stroke(rotationZ, 85, 10);
    strokeWeight(3);

    let startPos = new p5.Vector(0, 0);
    let endPos = new p5.Vector(rotationY / 90 * width, -rotationX / 90 * height);
    for(let i = 0; i < 25; i++){
        let startRandVec = p5.Vector.random2D().mult(random(0, 10));
        let endRandVec = p5.Vector.random2D().mult(random(0, 10));
        line(startPos.x + startRandVec.x, startPos.y + startRandVec.y, endPos.x + endRandVec.x, endPos.y + endRandVec.y);
        
    }
}
