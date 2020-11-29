// ページの読み込みを待つ
window.addEventListener('load', init);
// サイズを指定
let width = 650;
let height = 200;
// 1辺あたりに配置するオブジェクトの個数
let CELL_NUM_X = 20;
let CELL_NUM_Y = 20;
let renderer;
let scene;
let camera;
let stats;
let controls;

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
    camera.position.set(0, 0, 500);
    // カメラコントローラーを作成
    controls = new THREE.OrbitControls(camera);

    // 滑らかにカメラコントローラーを制御する
    controls.enableDamping = true;
    controls.dampingFactor = 0.2;
    // フレームレートの数値を画面に表示
    stats = new Stats();
    stats.domElement.style.position = 'absolute';
    stats.domElement.style.top = '10px';
    document.body.appendChild(stats.domElement);
    setMap();
    tick();
}

function setMap() {
    // シーンを作成こうするとsceneを更新できる
    scene = new THREE.Scene();
    // 一応更新
    CELL_NUM_X = ArrayData[0][0];
    CELL_NUM_Y = ArrayData[0][1];
    // 空のジオメトリを作成
    const geometry = new THREE.Geometry();
    // console.log("setMap");
    // Box
    // for (let i = 0; i < CELL_NUM; i++) {
    for (let j = 1; j < CELL_NUM_X+1; j++) {
        for (let k = 0; k < CELL_NUM_Y; k++) {
            // 立方体個別の要素を作成
            const sampleGeometry = new THREE.BoxGeometry(10, 10 * (ArrayData[j][k] + 1), 10);
            // console.log(ArrayData);
            // 座標調整の行列を作成
            const matrix = new THREE.Matrix4();
            matrix.makeTranslation(
                10 * (k),
                5 * (ArrayData[j][k]),
                10 * (j-1)
            );

            // ジオメトリをマージ（結合）
            geometry.merge(sampleGeometry, matrix);
        }
    }
    // }
    // マテリアルを作成
    const material = new THREE.MeshNormalMaterial();
    // メッシュを作成
    const mesh = new THREE.Mesh(geometry, material);
    scene.add(mesh);

}

function tick() {
    // カメラコントローラーを更新
    controls.update();
    // レンダリング
    renderer.render(scene, camera);
    
    // // レンダリング情報を画面に表示
    // document.getElementById('info').innerHTML = JSON.stringify(
    //     renderer.info.render,
    //     '',
    //     '    '
    // );

    // フレームレートを表示
    stats.update();

    requestAnimationFrame(tick);
}

