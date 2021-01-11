// ページの読み込みを待つ
// window.addEventListener('load', init);
// サイズを指定
let width = 1600;
let height = 900;
// 正方形ブロックの辺の長さ
const SQUARE_LENGTH = 30;
// 1辺あたりに配置するオブジェクトの個数
let CELL_NUM_X = 20;
let CELL_NUM_Y = 20;
const MOVE_VELOCITY = 10;
let renderer;
let scene;
let camera;
let stats;
let controls;

function setup() {
    init();
}

function init() {
    // レンダラーを作成
    renderer = new THREE.WebGLRenderer({
        canvas: document.querySelector('#myCanvas')
    });
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(width, height);



    // カメラを作成
    camera = new THREE.PerspectiveCamera(45, width / height);
    // カメラの初期座標を設定
    camera.position.set(0, 50, -100);
    camera.lookAt(0, 0, 50);
    // フレームレートの数値を画面に表示
    stats = new Stats();
    stats.domElement.style.position = 'absolute';
    stats.domElement.style.top = '10px';
    document.body.appendChild(stats.domElement);


    window.addEventListener("keydown", handleKeydown);


    window.addEventListener("keyup", handleKeyup);

    readFile(currentPath);
}


let rot = 0;
let lot = 0;
let par = 0;
let xtu = 0;
let ytu = 0;

function handleKeyup(event) {
    console.log("キーボードが離された");
    rot = 0;
    lot = 0;
    par = 0;
    xtu = 0;
    ytu = 0;

}
function handleKeydown(event) {
    // キーコード(どのキーが押されたか)を取得
    var keyCode = event.keyCode;
    // 条件文で制御する
    if (keyCode == 68) {
        // 右
        // model.position.x += 5;
        rot = -MOVE_VELOCITY;
    }
    if (keyCode == 65) {
        // 左
        // model.position.x -= 5;
        rot = +MOVE_VELOCITY;
    }

    if (keyCode == 87) {
        // 上
        // model.position.y += 5;
        lot = MOVE_VELOCITY * 3;
    }
    if (keyCode == 83) {
        // 下
        // model.position.y -= 5;
        lot = -MOVE_VELOCITY;
    }
    if (keyCode == 188) {
        // model.position.z += 5;
        par = MOVE_VELOCITY;
    }
    if (keyCode == 190) {
        // model.position.z -= 5;
        par = -MOVE_VELOCITY;
    }
    if (keyCode == 39) {
        ytu = -0.01;
    }

    if (keyCode == 37) {
        ytu = 0.01;
    }
    if (keyCode == 38) {
        xtu = 0.01;
    }

    if (keyCode == 40) {
        xtu = -0.01;
    }
    if (loadModel) {
        model.position = collision_CB(model, rot, lot, par);
        camera.position.x = model.position.x;
        camera.position.y = model.position.y + 50;
        camera.position.z = model.position.z - 100;
    };
    model.rotation.y += ytu * 3.6;
}


function collision_CB(C, goX, goY, goZ) {
    if (C.position.z < 0 || C.position.z > SQUARE_LENGTH * CELL_NUM_X) {
        console.log("マップの外z");
        return C.position;
    }
    if (C.position.y < -10) {
        console.log("マップの外y");
        return C.position;
    }
    if (C.position.x < 0 || C.position.x > SQUARE_LENGTH * CELL_NUM_Y) {
        console.log("マップの外x");
        return C.position;
    }
    // if ((goX > 0 && goX < SQUARE_LENGTH) || (goX < 0 && goX > -SQUARE_LENGTH)) {
    //     console.log("もう無理");
    //     return C.position;
    // }
    // if ((goZ > 0 && goZ < SQUARE_LENGTH) || (goZ < 0 && goZ > -SQUARE_LENGTH)) {
    //     console.log("もう無理");
    //     return C.position;
    // }
    // if ((goY < 0 && goY > -SQUARE_LENGTH)) {
    //     console.log("もう無理");
    //     return C.position;
    // }

    let cellX = floor(C.position.x / SQUARE_LENGTH);
    if (C.position.x - cellX * SQUARE_LENGTH >= 5) {
        cellX += 1;
    }
    let goCellX = floor((C.position.x + goX) / SQUARE_LENGTH);
    if ((C.position.x + goX) - goCellX * SQUARE_LENGTH >= 5) {
        goCellX += 1;
    }
    let cellY = floor(C.position.y / SQUARE_LENGTH);
    if (C.position.y - cellY * SQUARE_LENGTH >= 5) {
        cellY += 1;
    }
    let goCellY = floor((C.position.y + goY) / SQUARE_LENGTH);
    if ((C.position.y + goY) - goCellY * SQUARE_LENGTH >= 5) {
        goCellY += 1;
    }
    let cellZ = floor(C.position.z / SQUARE_LENGTH);
    if (C.position.z - cellZ * SQUARE_LENGTH >= 5) {
        cellZ += 1;
    }
    let goCellZ = floor((C.position.z + goZ) / SQUARE_LENGTH);
    if ((C.position.z + goZ) - goCellZ * SQUARE_LENGTH >= 5) {
        goCellZ += 1;
    }

    // if (goCellX < 0 || goCellX > SQUARE_LENGTH * CELL_NUM_Y) {
    //     return collision_CB(C, goX / 2, goY, goZ);
    // }
    // if (goCellY < 0) {
    //     return collision_CB(C, goX, goY / 2, goZ);
    // }
    // if (goCellZ < 0 || goCellZ > SQUARE_LENGTH * CELL_NUM_X) {
    //     return collision_CB(C, goX, goY, goZ / 2);
    // }
    console.log(cellX, cellY, cellZ);
    console.log(goCellX, goCellY, goCellZ);
    console.log(C.position);
    if (cellZ < goCellZ) {
        for (var i = cellZ; i <= goCellZ; i++) {
            if (cellX < goCellX) {
                for (var j = cellX; j <= goCellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }

                        }
                    }
                }
            }
            else {
                for (var j = goCellX; j <= cellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }

                        }
                    }
                }
            }
        }
    }
    else {
        for (var i = goCellZ; i <= cellZ; i++) {
            if (cellX < goCellX) {
                for (var j = cellX; j <= goCellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }

                        }
                    }
                }
            }
            else {
                for (var j = goCellX; j <= cellX; j++) {
                    if (cellY < goCellY) {
                        for (var k = cellY; k <= goCellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }

                        }
                    }
                    else {
                        for (var k = goCellY; k <= cellY; k++) {
                            if (mapData[i][j][k] === 1) {
                                console.log("collision", i, j, k);
                                return collision_CB(C, 0, 0, 0);
                            }
                        }
                    }
                }
            }
        }
    }
    C.position.x = goCellX*SQUARE_LENGTH;
    if (C.position.x < 0) {
        C.position.x = 0;
    } else if (C.position.x > SQUARE_LENGTH * (CELL_NUM_Y - 1)) {
        C.position.x = SQUARE_LENGTH * (CELL_NUM_Y - 1);
    }
    C.position.y = goCellY* SQUARE_LENGTH;
    C.position.z = goCellZ* SQUARE_LENGTH;
    if (C.position.z < 0) {
        C.position.z = 0;
    } else if (C.position.z > SQUARE_LENGTH * (CELL_NUM_X - 1)) {
        C.position.z = SQUARE_LENGTH * (CELL_NUM_X - 1)
    }
    return C.position;
}

var model;
var loadModel = false;
let Xrotation = 0;
let Yrotation = 0;
var Zrotation = 0;
function tick() {
    // if (loadModel === true) {
    //     model.position = collision_CB(model, 0, -MOVE_VELOCITY, 0);
    //     camera.position.x = model.position.x;
    //     camera.position.y = model.position.y + 50;
    //     camera.position.z = model.position.z - 100;


    // }

    Xrotation += 5 * xtu;

    Yrotation -= 5 * ytu;

    Zrotation += 5 * (ytu + xtu);
    // camera.lookAt(new THREE.Vector3(20 * sin(Yrotation) + camera.position.x, 20 * sin(Xrotation) -50 + camera.position.y, camera.position.z+150 - 20 * sin(Zrotation)));
    // レンダリング
    renderer.render(scene, camera);


    // フレームレートを表示
    stats.update();


    requestAnimationFrame(tick);
}


var mapData;
function setMap() {
    // シーンを作成こうするとsceneを更新できる
    scene = new THREE.Scene();
    // 一応更新
    CELL_NUM_X = ArrayData[0][1];
    CELL_NUM_Y = ArrayData[0][0];
    // 空のジオメトリを作成
    const geometry = new THREE.Geometry();
    // console.log("setMap");
    // Box
    // for (let i = 0; i < CELL_NUM; i++) {
    let dataZ = new Array();
    for (let j = 1; j < CELL_NUM_X + 1; j++) {
        let dataX = new Array();
        for (let k = 0; k < CELL_NUM_Y; k++) {
            let dataY = new Array();
            for (let l = 0; l < 1 + ArrayData[j][k]; l++) {
                dataY[l] = 1;
            }
            dataX.push(dataY);
            // 立方体個別の要素を作成
            const sampleGeometry = new THREE.BoxGeometry(SQUARE_LENGTH, SQUARE_LENGTH * (ArrayData[j][k] + 1), SQUARE_LENGTH);
            // console.log(ArrayData);
            // 座標調整の行列を作成
            const matrix = new THREE.Matrix4();
            matrix.makeTranslation(
                SQUARE_LENGTH * (k),
                SQUARE_LENGTH / 2 * (ArrayData[j][k]),
                SQUARE_LENGTH * (j - 1),

            );

            // ジオメトリをマージ（結合）
            geometry.merge(sampleGeometry, matrix);
        }
        dataZ.push(dataX);
    }
    mapData = dataZ;
    console.log(mapData);
    // }
    // マテリアルを作成
    const material = new THREE.MeshNormalMaterial();
    // メッシュを作成
    const mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);

    // 3DS形式のモデルデータを読み込む
    const loader = new THREE.OBJLoader();

    // 3dsファイルのパスを指定
    loader.load('../../data/SegModel.obj', (object) => {
        // object.makeTranslation{
        //     100,
        //     100,
        //     100
        // }
        object.position.y = 60;
        model = object;
        // 読み込み後に3D空間に追加
        scene.add(object);
        const sphereGeometry = new THREE.SphereGeometry(5, 32, 32);

        const sherMaterial = new THREE.MeshBasicMaterial({ color: 0xFF0000 });
        const sphere = new THREE.Mesh(sphereGeometry, sherMaterial);
        sphere.position.x = 0;
        sphere.position.y = 10;
        sphere.position.z = 0
        scene.add(sphere);
        loadModel = true;
    });

    var ambientLight = new THREE.AmbientLight(0xcccccc, 2);
    scene.add(ambientLight);
    // // 形状データを作成
    // const particleGeometry = new THREE.Geometry();
    // // 配置する範囲
    // const SIZE = 3000;
    // // 配置する個数
    // const LENGTH = 1000;
    // for (let i = 0; i < LENGTH; i++) {
    //     particleGeometry.vertices.push(new THREE.Vector3(
    //         SIZE * (Math.random() - 0.5),
    //         SIZE * (Math.random() - 0.5),
    //         SIZE * (Math.random() - 0.5),
    //     ));
    // }
    // // マテリアルを作成
    // const particleMaterial = new THREE.PointsMaterial({
    //     // 一つ一つのサイズ
    //     size: 5,
    //     // 色
    //     color: 0xFFFFFF,
    // });

    // const particle = new THREE.Points(particleGeometry, particleMaterial);
    // scene.add(particle);

    tick();
}
