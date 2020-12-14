// 変数
// ベース
var all_bace;
var all_bace_Image;
// 方向
var direction;
var direction_bace;
var direction_bace_Image;
var direction_circleL;
var direction_circleL_Image;
var direction_circleS;
var direction_circleS_Image;
// マップ
var map;
var map_Image;
// タイマー
var timer_bace;
var timer_bace_Image;
var timer_circle;
var timer_circle_Image;
// 名前
var name_bace;
var name_bace_Image;

function preload() {
    // ベース
    all_bace_Image = loadImage('all_bace.png');
    // 方向
    direction_bace_Image = loadImage('direction_bace.png');
    direction_circleL_Image = loadImage('direction_circleL.png');
    direction_circleS_Image = loadImage('direction_circleS.png');
    // 時間
    timer_bace_Image = loadImage('timer_bace.png');
    timer_circle_Image = loadImage('timer_circle.png');
    // マップ
    map_Image = loadImage('map_bace.png');
    // 名前
    name_bace_Image = loadImage('name_bace.png');
    
}

const width = 1600;
const height = 900;
function setup() {
    // キャンバスを作る
    createCanvas(width, height);

    all_bace = createSprite(width/2, height/2);
    all_bace.addImage('bace', all_bace_Image);
    all_bace.scale = 0.5;

    direction_bace = createSprite(width*4/40, height*5/30);
    direction_bace.addAnimation('bace', direction_bace_Image);
    direction_bace.scale = 0.15;
    direction_circleL = createSprite(width*4/40, height*5/30);
    direction_circleL.addAnimation('bace', direction_circleL_Image);
    direction_circleL.scale = 0.15;
    direction_circleS = createSprite(width*4/40, height*5/30);
    direction_circleS.addAnimation('bace', direction_circleS_Image);
    direction_circleS.scale = 0.15;

    timer_bace = createSprite(width*36/40, height*5/30);
    timer_bace.addImage('bace', timer_bace_Image);
    timer_bace.scale = 0.15;
    timer_circle = createSprite(width*36/40, height*5/30);
    timer_circle.addImage('bace',timer_circle_Image);
    timer_circle.scale = 0.15;

    map = createSprite(width*4/40, height*21/30);
    map.addImage('bace',map_Image);
    map.scale = 0.15;

    name_bace = createSprite(width*35/40, height*24/30);
    name_bace.addImage('bace', name_bace_Image);
    name_bace.scale = 0.15;
    
}


var angle = 0;
function draw() {
    // 背景色でぬりつぶす
    background(0, 0, 0, 65);

    // 全てのスプライトを描く
    drawSprites();
    // 回す
    direction_circleS._rotation = angle;
    // direction_circleS.scale = 0.1 + 0.025*sin(angle/180*PI);
    direction_circleL._rotation = -angle;
    // direction_circleL.scale = 0.1 + 0.025*sin(angle/180*PI);
    timer_circle._rotation = angle;
    angle += 2;
    if(angle === 360){
        angle = 0;
    }
}

