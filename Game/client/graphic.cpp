#include "graphic.h"
#include <math.h>
// #include "graphic_test.hpp"//テスト用

/*private*/
FloatPosition lookatPlace = {0.0f, 100.0f, 0.0f};
FloatPosition lookatCenter = {5.0f, 0.0f, 5.0f};
FloatPosition lookatUp = {0.0f, 1.0f, 0.0f};
const double PI = 3.141592;

//SDL2関連

SDL_Window *window = NULL;
SDL_GLContext context = NULL;

//内部関数
bool InitOpenGL();
void View2D();
void View3D();
void Disp2D();
void Disp3D();
void DrawMap(); //マップ描画
void DrawObject(); //設置オブジェクト描画
void DrawCharacter();//キャラクター描画
void Cube(FloatCube cube, SDL_Color color);
void RotateCube(FloatCube cube, double dir, SDL_Color color);


/*public*/

/////////////////////////////////////////////////////////////
void setlookat(int x, int y, int z, double dir){
    
    if(dir >= 2* PI)dir -= 2*PI;

    lookatPlace.x += x;
    lookatPlace.y += y;
    lookatPlace.z += z;
    lookatCenter.x = lookatPlace.x + 30*sin(dir);
    lookatCenter.z = lookatPlace.z + 30*cos(dir);
    lookatCenter.y = lookatPlace.y - 10;
    printf("(%f, %f, %f, %f)\n", lookatPlace.x, lookatPlace.y, lookatPlace.z, dir);
}
/////////////////////////////////////////////////////////////
void InitGraphic(){
    window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Wd_Width, Wd_Height, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	
    InitOpenGL();
}

//画面描画
void Disp(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glFlush();
    View3D();
    Disp3D();
    glFlush();   
    View2D();
    Disp2D();
    glFlush();
    SDL_GL_SwapWindow(window);
}

/*private*/

//OpenGL初期化
bool InitOpenGL() {
	
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glViewport(0, 0, Wd_Width, Wd_Height);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// 照明を設定する
	glEnable(GL_LIGHT0);
	static GLfloat position[] = {100.0f, 100.0f, 100.0f, 1.0f};
	static GLfloat ambient [] = { 1.0f, 1.0f, 1.0f, 1.0f};
	static GLfloat diffuse [] = { 1.0f, 1.0f, 1.0f, 1.0f};
	static GLfloat specular[] = { 0.0f, 0.0f, 0.0f, 0.0f};


	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	return true;
}

//2D描画用設定
void View2D() {
    glDisable(GL_DEPTH_TEST); //Depthバッファ無効
    glDisable( GL_LIGHTING ); //光源処理無効
    glMatrixMode(GL_PROJECTION);// 射影変換行列設定
    glPopMatrix();// 射影変換行列を復元
    glPushMatrix();// 現在の射影変換行列を保存
    glOrtho(0, 640, 420, 0, -1, 1);// 正射影変換設定
    glMatrixMode(GL_MODELVIEW);// モデルビュー変換行列設定
    glPopMatrix();// モデルビュー行列を復元
    glPushMatrix();// 現在のモデルビュー行列を保存
    glLoadIdentity();// 単位行列を設定
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

//3D描画用設定
void View3D() {
    //２D設定解除
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);// 射影変換行列設定
    glPopMatrix();// 射影変換行列を復元
    glPushMatrix();// 現在の射影変換行列を保存
    glLoadIdentity();// 単位行列を設定
    gluPerspective(60.0, Wd_Width / Wd_Height, 20.0, 500.0);
    glMatrixMode(GL_MODELVIEW);// モデルビュー変換行列設定
    glPopMatrix();// モデルビュー行列を復元
    glPushMatrix();// 現在のモデルビュー行列を保存
    glLoadIdentity();// 単位行列を設定

    glOrtho(0.0, 640, 420, 0.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);

}

//3D描画
void Disp3D(){
    DrawCharacter();
    DrawMap();
    DrawObject();
}

//2D描画
void Disp2D(){

}

//マップ描画
void DrawMap(){
    const int (*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = Map.GetTerrainData();
    FloatCube mcube;
    //仮宣言
    int map_w = Map.GetMapW(), map_d = Map.GetMapD();
    
    //床
    // マテリアルを設定する
    GLfloat ambient  [] = { 0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuse  [] = { 0.0f, 0.5f, 0.0f, 1.0f};
    GLfloat specular [] = { 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[] = { 0.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    //描画
    glBegin(GL_QUADS);
        glNormal3f(0.0,1.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3f(map_w * MAP_MAGNIFICATION, 0.0, 0.0);
        glVertex3f(map_w * MAP_MAGNIFICATION, 0.0, map_d * MAP_MAGNIFICATION);
        glVertex3f(0.0, 0.0, map_d * MAP_MAGNIFICATION);
    glEnd();

    //2次元配列データ使用時
    // for(int i = 0; i < map_w; i++){
    //     for(int j = 0; j < map_h; j++){
    //         if(terrainData[i][j] > 0){
    //             mcube = {
    //                 i * MAGNIFICATION, 0, j * MAGNIFICATION,
    //                 MAGNIFICATION, terrainData[i][j] * MAGNIFICATION, MAGNIFICATION
    //             };
    //             Cube(mcube);
    //         }
    //     }
    // }
    SDL_Color mapColor = {255,255,0,255}; 

    //3次元配列データ使用時
    for(int width = 0; width < map_w; width++){
        for(int depth = 0; depth < map_d; depth++){
            for(int hight = 0; terrainData[width][hight][depth] == 1; hight++){
                mcube = {
                    width * MAP_MAGNIFICATION, hight * MAP_MAGNIFICATION, depth * MAP_MAGNIFICATION,
                    MAP_MAGNIFICATION, MAP_MAGNIFICATION, MAP_MAGNIFICATION
                };
                Cube(mcube,mapColor);
            }
        }
    }

}

//設置オブジェクト描画
void DrawObject(){
    const vector<PlaceData> objectData = Map.GetObjectData();
    SDL_Color objectColor[PLAYER_NUM] = {
        {255,0,0,255},
        {0,255,0,255}
    };

    for(int i = 0; i < (int)objectData.size(); i++){
        FloatCube ocube = {
            objectData[i].pos.x * OBJECT_MAGNIFICATIO, objectData[i].pos.y * OBJECT_MAGNIFICATIO, objectData[i].pos.z * OBJECT_MAGNIFICATIO,
            OBJECT_MAGNIFICATIO, OBJECT_MAGNIFICATIO, OBJECT_MAGNIFICATIO
        };
        Cube(ocube,objectColor[objectData[i].object]);
    }
}


//キャラクター描画
void DrawCharacter(){
    const PlayerData* playerData = GetPlayerData();
    SDL_Color playercolor = {0,0,255,255};
    //仮宣言
    int myid = GetMyID();
    
    //視点変更
    lookatPlace.x = playerData[myid].pos.x + playerData[myid].pos.w / 2 + 60 * -sin(playerData[myid].direction);
    lookatPlace.y = playerData[myid].pos.y + 50;
    lookatPlace.z = playerData[myid].pos.z + playerData[myid].pos.d / 2 + 60 * -cos(playerData[myid].direction);

    lookatCenter.x = playerData[myid].pos.x + playerData[myid].pos.w / 2;
    lookatCenter.y = playerData[myid].pos.y + playerData[myid].pos.h / 2;
    lookatCenter.z = playerData[myid].pos.z + playerData[myid].pos.d / 2;
    // printf("%f\n",dir);
    
    // 視点を設定する
    gluLookAt(
        lookatPlace.x,  lookatPlace.y,  lookatPlace.z,
        lookatCenter.x, lookatCenter.y, lookatCenter.z,
        lookatUp.x,     lookatUp.y,     lookatUp.z
    );


        for(int i = 0; i < PLAYER_NUM; i++){
        FloatCube ccube = playerData[i].pos;
        RotateCube(ccube,playerData[i].direction, playercolor);
    }
}

//FloatCubeの描画
void Cube(FloatCube cube, SDL_Color color){
    //printf("(%f, %f, %f, %f, %f, %f)\n",cube.x, cube.y, cube.z, cube.w, cube.h, cube.d);
            
    // マテリアルを設定する
    GLfloat ambient  [] = { 0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuse  [] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
    GLfloat specular [] = { 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[] = { 0.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    // 立方体を描画する
    glBegin(GL_QUADS);
        //手前
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(cube.x, cube.y, cube.z);
        glVertex3f(cube.x + cube.w, cube.y, cube.z);
        glVertex3f(cube.x + cube.w, cube.y + cube.h, cube.z);
        glVertex3f(cube.x, cube.y + cube.h, cube.z);
        //左
        glNormal3f(-1.0,0.0,0.0);
        glVertex3f(cube.x, cube.y + cube.h, cube.z);
        glVertex3f(cube.x, cube.y + cube.h, cube.z + cube.d);
        glVertex3f(cube.x, cube.y, cube.z + cube.d);
        glVertex3f(cube.x, cube.y, cube.z);
        //下
        glNormal3f(0.0,-1.0,0.0);
        glVertex3f(cube.x, cube.y, cube.z);
        glVertex3f(cube.x + cube.w, cube.y, cube.z);
        glVertex3f(cube.x + cube.w, cube.y, cube.z + cube.d);
        glVertex3f(cube.x, cube.y, cube.z + cube.d);
        //奥
        glNormal3f(0.0,0.0,1.0);
        glVertex3f(cube.x, cube.y, cube.z + cube.d);
        glVertex3f(cube.x + cube.w, cube.y, cube.z + cube.d);
        glVertex3f(cube.x + cube.w, cube.y + cube.h, cube.z + cube.d);
        glVertex3f(cube.x, cube.y + cube.h, cube.z + cube.d);
        //上
        glNormal3f(0.0,1.0,0.0);
        glVertex3f(cube.x + cube.w, cube.y + cube.h, cube.z + cube.d);
        glVertex3f(cube.x, cube.y + cube.h, cube.z + cube.d);
        glVertex3f(cube.x, cube.y + cube.h, cube.z);
        glVertex3f(cube.x + cube.w, cube.y + cube.h, cube.z);
        //右
        glNormal3f(1.0,0.0,0.0);
        glVertex3f(cube.x + cube.w, cube.y + cube.h, cube.z);
        glVertex3f(cube.x + cube.w, cube.y, cube.z);
        glVertex3f(cube.x + cube.w, cube.y, cube.z + cube.d);
        glVertex3f(cube.x + cube.w, cube.y + cube.h, cube.z + cube.d);
    glEnd();
}



void RotateCube(FloatCube cube, double dir, SDL_Color color){
    // マテリアルを設定する
    GLfloat ambient  [] = { 0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat diffuse  [] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f};
    GLfloat specular [] = { 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[] = { 0.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    //座標変換
    FloatPosition defbase[4] = {
        {cube.x,cube.y,cube.z},
        {cube.x + cube.w,cube.y,cube.z},
        {cube.x + cube.w,cube.y,cube.z+cube.d},
        {cube.x,cube.y,cube.z+cube.d}
    };
    FloatPosition base[4];
    FloatPosition top[4];

    FloatPosition center = {cube.x + cube.w/2, cube.y + cube.h/2, cube.z + cube.d/2};

    for(int i = 0; i < 4; i++){
        base[i].x = center.x + sin(dir) * (defbase[i].z - center.z) + cos(dir) * (defbase[i].x - center.x);
        base[i].y = defbase[i].y;
        base[i].z = center.z + cos(dir) *(defbase[i].z - center.z)  - sin(dir) * (defbase[i].x - center.x);
        top[i] = base[i];
        top[i].y += cube.h;
    }

    // 立方体を描画する
    glBegin(GL_QUADS);
        //手前
        glNormal3f(0.0,0.0,-1.0);
        glVertex3f(base[0].x, base[0].y, base[0].z);
        glVertex3f(top[0].x,top[0].y,top[0].z);
        glVertex3f(top[1].x,top[1].y,top[1].z);
        glVertex3f(base[1].x, base[1].y, base[1].z);
        //左
        glNormal3f(-1.0,0.0,0.0);
        glVertex3f(base[1].x, base[1].y, base[1].z);
        glVertex3f(top[1].x,top[1].y,top[1].z);
        glVertex3f(top[2].x,top[2].y,top[2].z);
        glVertex3f(base[2].x, base[2].y, base[2].z);
        //下
        glNormal3f(0.0,-1.0,0.0);
        glVertex3f(base[0].x, base[0].y, base[0].z);
        glVertex3f(base[1].x, base[1].y, base[1].z);
        glVertex3f(base[2].x, base[2].y, base[2].z);
        glVertex3f(base[3].x, base[3].y, base[3].z);
        //奥
        glNormal3f(0.0,0.0,1.0);
        glVertex3f(base[2].x, base[2].y, base[2].z);
        glVertex3f(top[2].x,top[2].y,top[2].z);
        glVertex3f(top[3].x,top[3].y,top[3].z);
        glVertex3f(base[3].x, base[3].y, base[3].z);
        //上
        glNormal3f(0.0,1.0,0.0);
        glVertex3f(top[0].x,top[0].y,top[0].z);
        glVertex3f(top[1].x,top[1].y,top[1].z);
        glVertex3f(top[2].x,top[2].y,top[2].z);
        glVertex3f(top[3].x,top[3].y,top[3].z);
        //右
        glNormal3f(1.0,0.0,0.0);
        glVertex3f(base[0].x, base[0].y, base[0].z);
        glVertex3f(top[0].x,top[0].y,top[0].z);
        glVertex3f(top[3].x,top[3].y,top[3].z);
        glVertex3f(base[3].x, base[3].y, base[3].z);
    glEnd();
}