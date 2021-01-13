class keyInput {
    constructor() {
        this.goX = 0;
        this.goY = 0;
        this.goZ = 0;
        this.rotX = 0;
        this.rotY = 0;
        this.rotZ = 0;
    
    }


    handleKeyup() {
        console.log("キーボードが離された");
        this.goX = 0;
        this.goY = 0;
        this.goZ = 0;
        this.rotX = 0;
        this.rotY = 0;
        this.rotZ = 0;

    }
    handleKeydown(event) {
        // キーコード(どのキーが押されたか)を取得
        var keyCode = event.keyCode;
        // console.log('押された',keyCode);
        // 条件文で制御する
        if (keyCode == 68) {
            // 右
            this.goX -= MOVE_VELOCITY;
        }
        if (keyCode == 65) {
            // 左
            this.goX += MOVE_VELOCITY;
        }

        if (keyCode == 87) {
            // 上

            this.goY += MOVE_VELOCITY * 3;
            console.log(this.goY);
        }
        if (keyCode == 83) {
            // 下

            this.goY -= MOVE_VELOCITY;
        }
        if (keyCode == 188) {
            this.goZ += MOVE_VELOCITY;
        }
        if (keyCode == 190) {

            this.goZ -= MOVE_VELOCITY;
        }
        if (keyCode == 39) {
            this.rotY = -0.01;
        }

        if (keyCode == 37) {
            this.rotY = 0.01;
        }
        if (keyCode == 38) {
            this.rotX = 0.01;
        }

        if (keyCode == 40) {
            this.rotX = -0.01;
        }
        
    }
}
