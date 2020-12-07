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
      scdSound.play();
      // eynSound.play();
    }
    var clickFlag = 0;
    function draw() {

      blendMode(BLEND);
      background(0, 2);
      blendMode(ADD);
      stroke(255);
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

      var Forward = "f";
      var Right = "f";
      var Left = "f";
      var Jump = "f";
      var Put = "f";
      // if (mouseIsPressed) {
      // すべてのタッチした場所で判定
      var touchCount = 0;
      touchObjectList.forEach(element => {
        /* キーマップ
        F F L R
        P P J J
        */
        if (element.touch.y <= windowHeight / 2) {
          if (element.touch.x >= windowWidth / 2) {
            Forward = "t";
          }
          if (element.touch.x < windowWidth / 2) {
            Put = "t";
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

            Jump = "t";

          }
        }
        touchCount++;
      })
      if (touchCount != 0) {
        // if (touchCount == 1 && mouseX != touchObjectList[0].touch.x) {
        //   Forward = "f";
        //   Right = "f";
        //   Left = "f";
        //   Jump = "f";
        //   Put = "f";
        //   if (mouseY <= windowHeight / 2) {
        //     if (mouseX >= windowWidth / 2) {
        //       Forward = "t";
        //     }
        //     if (mouseX < windowWidth / 2) {
        //       Put = "t";
        //     }
        //   }
        //   else {
        //     if (mouseX >= windowWidth / 2) {
        //       if (mouseY < windowHeight * 3 / 4) {
        //         Left = "t";
        //       }
        //       else {
        //         Right = "t";
        //       }
        //     }
        //     else {

        //       Jump = "t";

        //     }
        //     socket.send(Forward + " " + Right + " " + Left + " " + Jump + " " + Put + " " + "f");
        //   }
        // }
        // else {
          socket.send(Forward + " " + Right + " " + Left + " " + Jump + " " + Put + " " + "f");
        // }
        clickFlag = 1;
      }
      else {
        if (clickFlag = 1) {
          clickFlag = 0;
          socket.send("f f f f f f");
        }
      }
      // clickFlag = 1;
      // }
      // if (mouseIsPressed) {
      //   socket.send(Forward + " " + Right + " " + Left + " " + Jump + " " + Put + " " + "f");
      //   clickFlag = 0;
      // }
      // else {
      //   // 何も押さなくても7回は送信する
      //   if (clickFlag < 7) {
      //     clickFlag++;
      //     socket.send(Forward + " " + Right + " " + Left + " " + Jump + " " + Left + " " + "f");
      //   }
      //   else if (clickFlag == 7){
      //     clickFlag++;
      //     socket.send("f f f f f f f");
      //   }
      // }

    }