// ページの読み込みを待つ
// window.addEventListener('load', setup);
// サイズを指定
let width = 1600;
let height = 900;
// 1辺あたりに配置するオブジェクトの個数
let CELL_NUM_X = 20;
let CELL_NUM_Y = 20;

let renderer;
let scene;
let camera;
let stats;
let controls;
const Input = new keyInput();

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
    camera.position.set(0, 80, -100);
    camera.lookAt(0, 0, 50);
    // フレームレートの数値を画面に表示
    stats = new Stats();
    stats.domElement.style.position = 'absolute';
    stats.domElement.style.top = '10px';
    document.body.appendChild(stats.domElement);

    // シーンを作成こうするとsceneを更新できる
    scene = new THREE.Scene();

    readFile(currentPath);
}

var model;
var loadModel = false;

function tick() {
    if (loadModel === true) {
        
    }
    // レンダリング
    renderer.render(scene, camera);
    // フレームレートを表示
    stats.update();
    requestAnimationFrame(tick);
}


var mapData;
var sphere;
const slope = new Object3D('plane', 'normal', 0x6699FF, 40, 2000, 0, null, 32, 32);
function setMap() {
    scene = new THREE.Scene();
    // 一応更新
    CELL_NUM_X = ArrayData[0][1];
    CELL_NUM_Y = ArrayData[0][0];
    // 空のジオメトリを作成
    const geometry = new THREE.Geometry();
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
                (SQUARE_LENGTH * (ArrayData[j][k] + 1)) / 2,
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

    var ambientLight = new THREE.AmbientLight(0xcccccc, 2);
    scene.add(ambientLight);
    // 形状データを作成
    const particleGeometry = new THREE.Geometry();
    // 配置する範囲
    const SIZE = 3000;
    // 配置する個数
    const LENGTH = 1000;
    for (let i = 0; i < LENGTH; i++) {
        particleGeometry.vertices.push(new THREE.Vector3(
            SIZE * (Math.random() - 0.5),
            SIZE * (Math.random() - 0.5),
            SIZE * (Math.random() - 0.5),
        ));
    }
    // マテリアルを作成
    const particleMaterial = new THREE.PointsMaterial({
        // 一つ一つのサイズ
        size: 5,
        // 色
        color: 0xFFFFFF,
    });

    const particle = new THREE.Points(particleGeometry, particleMaterial);
    scene.add(particle);


    load3DModel();

    slope.mesh.position.x = 0;
    slope.mesh.position.y = 30;
    slope.mesh.position.z = 0;
    slope.mesh.rotation.x = 45/180*PI;
    scene.add(slope.mesh);

    window.addEventListener("keydown", (event) => {
        Input.handleKeydown(event);
        if (loadModel === true) {
            // console.log(Input);
            let temp = model;
            temp = slope.collision(temp, Input);
            model.position = temp.position;
            camera.position.x = model.position.x;
            camera.position.y = model.position.y + 50;
            camera.position.z = model.position.z - 150;
        }
    });
    window.addEventListener("keyup", () => {
        Input.handleKeyup();
        if (loadModel === true) {
            // console.log(Input);
            model.position.x += Input.goX;
            model.position.y += Input.goY;
            model.position.z += Input.goZ;
            camera.position.x = model.position.x;
            camera.position.y = model.position.y + 50;
            camera.position.z = model.position.z - 150;
        }
    });

    tick();
}

function load3DModel() {
    // 3DS形式のモデルデータを読み込む
    const loader = new THREE.OBJLoader();

    // 3dsファイルのパスを指定
    loader.load('../../data/SegModel.obj', (object) => {
        // object.makeTranslation{
        //     100,
        //     100,
        //     100
        // }
        object.position.y = 45;
        model = object;
        // 読み込み後に3D空間に追加
        scene.add(object);

        // scene.add(sphere);
        loadModel = true;
    });
}