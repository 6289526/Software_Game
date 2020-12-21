let pDeviceOrientation;
// var socket = null;
var accel_buttonS;
var accel_buttonL;
var jumpbutton_bace;
var jumpbutton_bace_Image;
var jumpbutton_circle;
var putbutton_bace;
var putbutton_bace_Image;
var putbutton_circle;
var button_circle_Image;
var direction_buttonL;
var direction_circleL_Image;
var direction_buttonL;
var direction_circleS_Image;
var angle = 0;

// タッチした情報を格納するクラス
let circleL = [];
let circleS = [];
class touchObject {
  constructor(_touch) {
    this.touch = _touch;
    this.id = _touch.id;
    this.count = 0;


  }

  update() {
    this.count++;

    // 削除判定
    // 同じIDのtouchが存在しなければ削除
    let isExist = false;

    touches.forEach(element => {
      if (element.id === this.id) {
        this.touch = element;
        isExist = true;
      }
    });

    this.draw();


    return isExist;
  }

  draw() {
    noFill();

    // // 回す
    // for (var i = 0; i < circleS.length; i++) {
    //   circleS[i].position.x = this.touch.x;
    //   circleS[i].position.y = this.touch.y;
    //   circleS[i].life = 10;
    //   circleS[i]._rotation = angle;
    // }
    // for (var i = 0; i < circleL.length; i++) {
    //   circleL[i].position.x = this.touch.x;
    //   circleL[i].position.y = this.touch.y;
    //   circleL[i].life = 10;
    //   circleL[i]._rotation = -angle;
    // }
    // // direction_circleS.scale = 0.1 + 0.025*sin(angle/180*PI);

    // angle += 2;
    // if (angle === 360) {
    //   angle = 0;
    // }

  }
}

let touchObjectList = [];


var scdSound;
var eynSound;
function preload() {
  soundFormats('mp3', 'ogg');
  scdSound = loadSound('assets/sccesscond.mp3');
  eynSound = loadSound('assets/entername');
  // 方向
  direction_circleL_Image = loadImage('assets/direction_circleL.png');
  direction_circleS_Image = loadImage('assets/direction_circleS.png');
  jumpbutton_bace_Image = loadImage('assets/jumpbutton_bace.png');
  putbutton_bace_Image = loadImage('assets/putbutton_bace.png');
  button_circle_Image = loadImage('assets/putbutton_circle.png');
}



var context;
var buttons = [];
var currentWidth;
var currentHeight;
function setup() {

  window.addEventListener("touchstart", function (event) { event.preventDefault(); }, { passive: false });
  window.addEventListener("touchmove", function (event) { event.preventDefault(); }, { passive: false });

  if(windowWidth <= windowHeight){
  currentWidth = windowWidth;
  currentHeight = windowHeight;
  }
  else{
    currentWidth = windowHeight;
    currentHeight = windowWidth;
  }

  createCanvas(currentWidth, currentHeight);

  background(0);
  // colorMode(HSB, 360, 100, 100, 100);

  touchObjectList = new Array();

  // 文字の設定
  fill(250);
  textFont('Paytone One');
  textStyle(BOLD);


  accel_buttonL = createSprite(currentWidth * 20 / 40, currentHeight * 10 / 40);
  accel_buttonL.addAnimation('bace', direction_circleL_Image);
  accel_buttonL.scale = 0.2;
  accel_buttonS = createSprite(currentWidth * 20 / 40, currentHeight * 10 / 40);
  accel_buttonS.addAnimation('bace', direction_circleS_Image);
  accel_buttonS.scale = 0.2;
  accel_buttonS._rotation = 90;

  direction_buttonL = createSprite(currentWidth * 10 / 40, currentHeight * 30 / 40);
  direction_buttonL.addAnimation('bace', direction_circleL_Image);
  direction_buttonL.scale = 0.15;
  direction_buttonS = createSprite(currentWidth * 10 / 40, currentHeight * 30 / 40);
  direction_buttonS.addAnimation('bace', direction_circleS_Image);
  direction_buttonS.scale = 0.15;

  jumpbutton_bace = createSprite(currentWidth * 30 / 40, currentHeight * 35 / 40);
  jumpbutton_bace.addAnimation('bace', jumpbutton_bace_Image);
  jumpbutton_bace.scale = 0.1;
  jumpbutton_bace._rotation = 90;
  jumpbutton_circle = createSprite(currentWidth * 30 / 40, currentHeight * 35 / 40);
  jumpbutton_circle.addAnimation('bace', button_circle_Image);
  jumpbutton_circle.scale = 0.1;
  jumpbutton_circle._rotation = 90;

  putbutton_bace = createSprite(currentWidth * 30 / 40, currentHeight * 25 / 40);
  putbutton_bace.addAnimation('bace', putbutton_bace_Image);
  putbutton_bace.scale = 0.1;
  putbutton_bace._rotation = -90;
  putbutton_circle = createSprite(currentWidth * 30 / 40, currentHeight * 25 / 40);
  putbutton_circle.addAnimation('bace', button_circle_Image);
  putbutton_circle.scale = 0.1;
  putbutton_circle._rotation = 90;
}


var clickFlag = 1;
var load = 0;
var deviceMode = "launch"
var index = 0;
var accel_buttonL_angle = 0;
var put_button_angle = 0;
var jump_button_angle = 0;
var direction_buttonL_angle = 0;
function draw() {

  switch (deviceMode) {
    case "launch":
      background(0, 2);
      textSize(45);
      textAlign(CENTER);
      text('Click here.', width / 2, height / 2);

      if (mouseIsPressed && load == 0) {
        context = new AudioContext();
        context.resume().then(() => {
          console.log('Playback resumed successfully');
        });
        socket.send("J");

        load = 1;
      }

      if (load >= 1 && load <= 20) {
        scdSound.play();
        load++;
      }
      else if (load > 20) {
        deviceMode = "gamePlay";
      }

      break;
    case "gamePlay":
      background(0);
      // スプライトを表示する
      drawSprites();
      
      var L1 = "f";
      var R1 = "f";
      var R2 = "f";
      var R3 = "f";
      var R4 = "f";
      var R5 = "f";
      var R6 = "f";

      // すべてのタッチした場所で判定
      if (touches.length != 0) {
        let tid = 0;
        touches.forEach(element => {
          if (tid <= 5) {
            if (tid === 5) {
              tid = 0;
            }

            /* キーマップ
            L1 L1 R1    R2
            L1 L1     R3
            L1 L1 R4    R5
            L1 L1     R6
            */
            if (element.y <= windowHeight / 2) {
              // if (element.x >= windowWidth / 2) {
              L1 = "t";
              accel_buttonL._rotation = accel_buttonL_angle;
              accel_buttonL_angle += 5;
              if (accel_buttonL_angle > 360) {
                accel_buttonL_angle = 0;
                // }
              }
            }
            else {

              if (element.x >= windowWidth / 2) {
                if (element.y < windowHeight * 3 / 4) {
                  R1 = "t";
                  putbutton_circle._rotation = put_button_angle;
                  put_button_angle += 5;
                }
                else {
                  R2 = "t";
                  jumpbutton_circle._rotation = jump_button_angle;
                  jump_button_angle += 5;
                }
              }
              else {
                let direction_angleS = atan2((element.y - direction_buttonL.position.y), (element.x - direction_buttonL.position.x));
                direction_buttonS._rotation = direction_angleS * 180 / PI + 90;
                // console.log(direction_angleS * 180 / PI);
                direction_buttonL._rotation = direction_buttonL_angle;
                direction_buttonL_angle += 5;
                // text(direction_buttonS._rotation, width / 2, height / 2);
                if ((direction_buttonS._rotation < 0 && direction_buttonS._rotation > -22.5) || (direction_buttonS._rotation >= 0 && direction_buttonS._rotation < 22.5)) {
                  R4 = "t";
                }
                else if ((direction_buttonS._rotation < 67.5 && direction_buttonS._rotation >= 22.5)) {
                  R3 = "t";
                  R4 = "t";
                }
                else if((direction_buttonS._rotation < 112.5 && direction_buttonS._rotation >= 67.5)){
                  R3 = "t";
                }
                else if((direction_buttonS._rotation < 157.5 && direction_buttonS._rotation >= 112.5)){
                  R3 = "t";
                  R5 = "t";
                }
                else if ((direction_buttonS._rotation < 202.5 && direction_buttonS._rotation >= 157.5)) {
                  R5 = "t";
                }
                else if((direction_buttonS._rotation < 247.5 && direction_buttonS._rotation >= 202.5)){
                  R5 = "t";
                  R6 = "t";
                }
                else if((direction_buttonS._rotation <= 270 && direction_buttonS._rotation >= 247.5 || (direction_buttonS._rotation < -67.5 && direction_buttonS._rotation >= -90))){
                  R6 = "t";
                }
                else if((direction_buttonS._rotation < -22.5 && direction_buttonS._rotation >= -67.5)){
                  R4 = "t";
                  R6 = "t";
                }
                
              }
            }
            
            tid++;
          }
        });
      }

      if (touches.length != 0) {
        socket.send(L1 + " " + R1 + " " + R2 + " " + R3 + " " + R4 + " " + R5 + " " + R6 + " " + "f");
        clickFlag = 1;
        // if (index == 26) {
        //   deviceMode = "gameControl";
        // }
        
      }
      else {
        if (clickFlag === 1) {
          clickFlag = 0;
          socket.send("f f f f f f f f");
        }
      }
      // console.log(clickFlag);


      break;
  }
}

function createSock() {
  // webソケット
  // こいつが通信の要
  if (!window["WebSocket"]) {
    alert("Error: Your browser does not support web sockets.")
  } else {
    socket = new WebSocket("ws://localhost:8080/room");
    socket.onclose = function () {
      alert("Connection has been closed.");
    }
    socket.onmessage = function (e) {
      // messages.append($("<li>").text(e.data));
    }
  }
}

function createButton() {
  var button;
  button = createSprite(width * 3 / 4, height / 4, width / 2, height / 2);//前
  button.onMousePressed = clicked;
  button.id = 0;
  buttons[0] = button;
  button = createSprite(width * 3 / 4, height * 5 / 8, width / 2, height / 4);//左
  button.onMousePressed = clicked;
  button.id = 1;
  buttons[1] = button;
  button = createSprite(width * 3 / 4, height * 7 / 8, width / 2, height / 4);//右
  button.onMousePressed = clicked;
  button.id = 2;
  buttons[2] = button;
  button = createSprite(width / 4, height * 5 / 8, width / 2, height / 4);//置く
  button.onMousePressed = clicked;
  button.id = 3;
  buttons[3] = button;
  button = createSprite(width / 4, height * 7 / 8, width / 2, height / 4);//ジャンプ
  button.onMousePressed = clicked;
  button.id = 4;
  buttons[4] = button;
  button = createSprite(width / 4, height / 4, width / 2, height / 2);//下
  button.onMousePressed = clicked;
  button.id = 5;
  buttons[5] = button;

  console.log(buttons);
}


function clicked(button) {
  // console.log(button.position.x, button.position.y, button.id);

}
