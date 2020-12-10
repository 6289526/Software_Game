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

function setup() {
    window.addEventListener("touchstart", function (event) { event.preventDefault(); }, { passive: false });
    window.addEventListener("touchmove", function (event) { event.preventDefault(); }, { passive: false });
    createCanvas(windowWidth, windowHeight);

    background(0);
    colorMode(HSB, 360, 100, 100, 100);

    touchObjectList = new Array();

    pDeviceOrientation = deviceOrientation;

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
}
var clickFlag = 0;
function draw() {
    // if (pDeviceOrientation !== undefined && pDeviceOrientation !== deviceOrientation) {
    //   // 向きが変わったとき
    //   noCanvas();
    //   createCanvas(window.innerWidth, windowHeight);
    // }
    // pDeviceOrientation = deviceOrientation;

    blendMode(BLEND);
    background(0, 2);
    blendMode(ADD);
    stroke(255);
    if (touches.length != 0) {

        // 初出現のIDを探す
        touches.forEach(element => {
            console.log(element);
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

    if (mouseIsPressed) {
        if (mouseY <= windowHeight / 2) {
            if (mouseX >= windowWidth / 2) {
                socket.send("F");
            }
            if (mouseX < windowWidth / 2) {
                socket.send("P");
            }
        }
        else {
            if (mouseX >= windowWidth / 2) {
                if (mouseY < windowHeight * 3 / 4) {
                    socket.send("L");
                }
                else {
                    socket.send("R");
                }
            }
            else {

                socket.send("U");

            }
        }
        clickFlag = 1;
    }
    else {
        if (clickFlag) {
            clickFlag = 0;
            socket.send("N");
        }
    }

}
