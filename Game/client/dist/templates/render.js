let pDeviceOrientation;
    var socket = null;
    var direction_circleL_Image;
    var direction_circleS_Image;
    // var angle = 0;

    // タッチした情報を格納するクラス
    // let circleL = [];
    // let circleS = [];
    // タッチした情報を格納するクラス
    class touchObject {
      constructor(_touch) {
        this.angle = 0;
        this.touch = _touch;
        this.id = _touch.id;
        this.count = 0;
        this.direction_circleL;
        this.direction_circleL = createSprite(this.touch.x, this.touch.y);
        this.direction_circleL.addAnimation('bace', direction_circleL_Image);
        this.direction_circleL.scale = 0.15;
        
        this.direction_circleS;
        this.direction_circleS = createSprite(this.touch.x, this.touch.y);
        this.direction_circleS.addAnimation('bace', direction_circleS_Image);
        this.direction_circleS.scale = 0.15;
        
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
        // drawSprites();
        // 回す
        // for (var i = 0; i < circleS.length; i++) {
          this.direction_circleS.life = 10;
          this.direction_circleS.position.x = this.touch.x;
          this.direction_circleS.position.y = this.touch.y;
          this.direction_circleS._rotation = this.angle;
          this.direction_circleS.scale = 0.15 + 0.05*sin(this.angle*PI/180);
        // }
        // for (var i = 0; i < circleL.length; i++) {
          this.direction_circleL.life = 10;
          this.direction_circleL.position.x = this.touch.x;
          this.direction_circleL.position.y = this.touch.y;
          this.direction_circleL._rotation = -this.angle;
          this.direction_circleL.scale = 0.15 + 0.05*sin(this.angle*PI/180);
        // }
        // direction_circleS.scale = 0.1 + 0.025*sin(angle/180*PI);

        this.angle += 4;
        if (this.angle === 360) {
          this.angle = 0;
        }
      }
    }

    let touchObjectList = [];

    var scdSound;
    var eynSound;
    function preload() {
      soundFormats('mp3', 'ogg');
      scdSound = loadSound('assets/sccesscond.mp3')
      // eynSound = loadSound('assets/entername');
      // 方向
      direction_circleL_Image = loadImage('assets/direction_circleS.png');
      direction_circleS_Image = loadImage('assets/direction_circleL.png');
    }



    var context;
    var buttons = [];
    function setup() {

      window.addEventListener("touchstart", function (event) { event.preventDefault(); }, { passive: false });
      window.addEventListener("touchmove", function (event) { event.preventDefault(); }, { passive: false });

      createCanvas(windowWidth, windowHeight);

      background(0);
      colorMode(HSB, 360, 100, 100, 100);

      touchObjectList = new Array();

      // webソケット
      // こいつが通信の要
      if (!window["WebSocket"]) {
        alert("Error: Your browser does not support web sockets.")
      } else {
        socket = new WebSocket("ws://{{.Host}}/room");
        socket.onclose = function () {
          alert("Connection has been closed.");
        }
        socket.onmessage = function (e) {
          // messages.append($("<li>").text(e.data));
        }
      }
      // 文字の設定
      fill(250);
      textFont('Paytone One');
      textStyle(BOLD);

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


    var clickFlag = 0;
    var load = 0;
    var deviceMode = "launch"
    var index = 0;

    function draw() {

      switch (deviceMode) {
        case "launch":
          background(0);
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
          // textSize(45);
          // textAlign(CENTER);
          // text(index, width / 2, height / 2);

          var L1 = "f";
          var L2 = "f";
          var R1 = "f";
          var R2 = "f";
          var R3 = "f";
          var R4 = "f";


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
                // console.log(element);

              }
            });
          }


          var circle_index = 0;
          touchObjectList.forEach(element => {
            circle_index++;
            if (element.update() === false) {
              element.direction_circleS.remove();
              element.direction_circleL.remove();
              touchObjectList.pop(element);
            }
          });

          // if (mouseIsPressed) {
          // すべてのタッチした場所で判定
          var touchCount = 0;
          touchObjectList.forEach(element => {
            /* キーマップ
            L1 L1 R1 R2
            L2 L2 R3 R4
            */
            if (element.touch.y <= windowHeight / 2) {
              if (element.touch.x >= windowWidth / 2) {
                L1 = "t";

              }
              if (element.touch.x < windowWidth / 2) {
                L2 = "t";
                if (index < 18) {
                  index += 9;
                }
              }
            }
            else {
              if (element.touch.x >= windowWidth / 2) {
                if (element.touch.y < windowHeight * 3 / 4) {
                  R1 = "t";
                  if (index != 0 && index != 9 && index != 18) {
                    index--;
                  }
                }
                else {
                  R2 = "t";
                  if (index != 8 && index != 17 && index != 26) {
                    index++;
                  }
                }
              }
              else {
                if (element.touch.y < windowHeight * 3 / 4) {
                  R3 = "t";
                }
                else {
                  R4 = "t";
                }

              }
            }
            touchCount++;
          })
          if (touchObjectList.length != 0) {
            socket.send(L1 + " " + L2 + " " + R1 + " " + R2 + " " + R3 + " " + R4 + " " + "f");
            clickFlag = 1;
            // if (index == 26) {
            //   deviceMode = "gameControl";
            // }
          }
          else {
            if (clickFlag = 1) {
              clickFlag = 0;
              socket.send("f f f f f f f ");
            }
          }
          break;
        case "gameControl":
          alert(deviceMode);
          console.log(deviceMode);
          // blendMode(BLEND);
          background(0);
          // スプライトを表示する
          drawSprites();
          // blendMode(ADD);
          // stroke(255);


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
