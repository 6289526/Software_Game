let pDeviceOrientation;
var socket = null;


// タッチした情報を格納するクラス
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
    stroke(abs(this.id * 73) % 360, 70, 10);
    strokeWeight(this.count / 2);
    ellipse(this.touch.x, this.touch.y, this.count * this.count / 20, this.count * this.count / 20);
  }
}

let touchObjectList = [];

var scdSound;
var eynSound;
function preload() {
  soundFormats('mp3', 'ogg');
  scdSound = loadSound('assets/sccesscond.mp3')
  eynSound = loadSound('assets/entername');
}



var context = new AudioContext();
var buttons = [];
function setup() {

  window.addEventListener("touchstart", function (event) { event.preventDefault(); }, { passive: false });
  window.addEventListener("touchmove", function (event) { event.preventDefault(); }, { passive: false });

  createCanvas(windowWidth, windowHeight);

  background(0);
  colorMode(HSB, 360, 100, 100, 100);

  touchObjectList = new Array();

  createSock();

  context.resume().then(() => {
    console.log('Playback resumed successfully');
  });
  // 文字の設定
  fill(250);
  textFont('Paytone One');
  textStyle(BOLD);

  createButton();

}


var clickFlag = 0;
var load = 0;
var deviceMode = "launch"
var index = 0;

function draw() {

  switch (deviceMode) {
    case "launch":
      background(0, 2);
      textSize(45);
      textAlign(CENTER);
      text('Click here.', width / 2, height / 2);

      if (mouseIsPressed && load == 0) {
        socket.send("J");

        load = 1;
      }

      if (load >= 1 && load <= 30) {
        scdSound.play();
        load++;
      }
      else if (load > 30) {
        deviceMode = "enterName";
        // $('#nameBox').fadeIn();
        // $('#defaultCanvas0').fadeOut();
      }

      break;
    case "enterName":
      console.log(deviceMode);
      blendMode(BLEND);
      background(0, 2);
      // スプライトを表示する
      drawSprites();
      blendMode(ADD);
      stroke(255);


      var Forward = "f";
      var Right = "f";
      var Left = "f";
      var Jump = "f";
      var Put = "f";
      var Up = "f";
      var Down = "f";

      if (touches.length != 0) {

        // 初出現のIDを探す
        touches.forEach(element => {
          // console.log(element);
          let isExist = false;
          touchObjectList.forEach(object => {
            if (element.id === object.id) {
              isExist = true;
            }
          });
          if (isExist === false) {
            // 要素の追加
            touchObjectList.push(new touchObject(element));
          }
        });
      }


      touchObjectList.forEach(element => {
        if (element.update() === false) {
          touchObjectList.pop(element);
        }
      });

      // if (mouseIsPressed) {
      // すべてのタッチした場所で判定
      var touchCount = 0;
      touchObjectList.forEach(element => {
        /* キーマップ
        U U L R
        D D D E
        */
        if (element.touch.y <= windowHeight / 2) {
          if (element.touch.x >= windowWidth / 2) {
            Up = "t";
            if(index>8){
              index -= 9;
            }
          }
          if (element.touch.x < windowWidth / 2) {
            Down = "t";
            if(index < 18){
              index += 9;
            }
          }
        }
        else {
          if (element.touch.x >= windowWidth / 2) {
            if (element.touch.y < windowHeight * 3 / 4) {
              Left = "t";
              if(index != 0 && index != 9 && index != 18){
                index--;
              }
            }
            else {
              Right = "t";
              if(index != 8 && index != 17 && index != 26){
                index++;
              }
            }
          }
          else {
            if (element.touch.y < windowHeight * 3 / 4) {
              Forward = "t";
            }
            else {
              Jump = "t";
            }

          }
        }
        touchCount++;
      })
      if (touchObjectList.length != 0) {
        socket.send(Forward + " " + Right + " " + Left + " " + Jump + " " + Put + " " + "f" + "" + Up + "" + Down);
        clickFlag = 1;
        if (index == 27) {
          deviceMode = "gameControl";
        }
      }
      else {
        if (clickFlag = 1) {
          clickFlag = 0;
          socket.send("f f f f f f f f");
        }
      }
      break;
    case "gameControl":
      console.log(deviceMode);
      blendMode(BLEND);
      background(0, 2);
      // スプライトを表示する
      drawSprites();
      blendMode(ADD);
      stroke(255);


      var Forward = "f";
      var Right = "f";
      var Left = "f";
      var Jump = "f";
      var Put = "f";

      if (touches.length != 0) {

        // 初出現のIDを探す
        touches.forEach(element => {
          // console.log(element);
          let isExist = false;
          touchObjectList.forEach(object => {
            if (element.id === object.id) {
              isExist = true;
            }
          });
          if (isExist === false) {
            // 要素の追加
            touchObjectList.push(new touchObject(element));
          }
        });
      }


      touchObjectList.forEach(element => {
        if (element.update() === false) {
          touchObjectList.pop(element);
        }
      });

      // if (mouseIsPressed) {
      // すべてのタッチした場所で判定
      var touchCount = 0;
      touchObjectList.forEach(element => {
        /* キーマップ
        F F L R
        F F p J
        */
        if (element.touch.y <= windowHeight / 2) {
          if (element.touch.x >= windowWidth / 2) {
            Forward = "t";
          }
          if (element.touch.x < windowWidth / 2) {
            Forward = "t";
          }
        }
        else {
          if (element.touch.x >= windowWidth / 2) {
            if (element.touch.y < windowHeight * 3 / 4) {
              Left = "t";
            }
            else {
              Right = "t";
            }
          }
          else {
            if (element.touch.y < windowHeight * 3 / 4) {
              Put = "t";
            }
            else {
              Jump = "t";
            }

          }
        }
        touchCount++;
      })
      if (touchObjectList.length != 0) {
        socket.send(Forward + " " + Right + " " + Left + " " + Jump + " " + Put + " " + "f f f");
        clickFlag = 1;
      }
      else {
        if (clickFlag = 1) {
          clickFlag = 0;
          socket.send("f f f f f f f f");
        }
      }
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
  console.log(button.position.x, button.position.y, button.id);

  // if (button.id == 0) {
  //   Forward = "t";
  // }
  // else if (button.id == 1) {
  //   Left = "t";
  //   Right = "f";
  // }
  // else if (button.id == 2) {
  //   Right = "t";
  //   Left = "f";
  // }
  // else if (button.id == 3) {
  //   Put = "t";
  // }
  // else if (button.id == 4) {
  //   Jump = "t";
  // }

}
