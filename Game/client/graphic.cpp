#include "graphic.h"
#include "graphic_objmesh.hpp"
#include "graphic_2dui.hpp"
#include <math.h>

/*private*/
FloatPosition lookatPlace = {0.0f, 100.0f, 0.0f};
FloatPosition lookatCenter = {5.0f, 0.0f, 5.0f};
FloatPosition lookatUp = {0.0f, 1.0f, 0.0f};
const double PI = 3.141592;

OBJMESH characterMesh;
SDL_Color playerColors[MAX_NUMCLIENTS];

GLuint BlockTexture[BLOCK_TYPE_NUM];
#ifdef DEBUG
char TextureFileName[BLOCK_TYPE_NUM][128] ={
    "../data/ui/all_bace2.png",
    "../data/FieldBlock.jpg",
    "../data/nbrock1.jpg"
};
char Meshfile[256] ={"../data/SegModel.obj"};
#else
char TextureFileName[BLOCK_TYPE_NUM][128] ={
    "../../data/ui/all_bace2.png",
    "../../data/FieldBlock.jpg",
    "../../data/nbrock1.jpg"
    

};
char Meshfile[256] ={"../../data/SegModel.obj"};
#endif
//SDL2関連

SDL_Window *window = NULL;
SDL_GLContext context = NULL;
SDL_Renderer *renderer = NULL;

//内部関数
bool InitOpenGL();
void SetBlockTexture();
void SetPlayerColors();
void View2D();
void View3D();
void Disp3D();
void DrawMap(); //マップ描画
void DrawCharacter();//キャラクター描画
void DrowMapBlock(FloatCube cube, BlockType block);
void RotateCube(FloatCube cube, double dir, SDL_Color *color);

Gui2D gui2d;

/*public*/

void InitGraphic(){
    IMG_Init(IMG_INIT_JPG);
    window = SDL_CreateWindow("OpenGL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Wd_Width, Wd_Height, SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    InitOpenGL();
    SetBlockTexture();
    characterMesh.LoadFile(Meshfile);
    SetPlayerColors();

}

void Init2dGraphic(){
    TTF_Init();
    gui2d.Set();
}

//画面描画
void Disp(){
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glFlush();
    View3D();
    Disp3D();
    glFlush();
    View2D();
    gui2d.Draw();
    glFlush();
    SDL_GL_SwapWindow(window);
}

SDL_Renderer* GetWindowRenderer(){
    return renderer;
}

SDL_Color* GetPlayerColors(){
    return playerColors;
}

void TerminateGraphic(){
    for(int i = 0; i < BLOCK_TYPE_NUM; i++){
        glDeleteTextures(1, &(BlockTexture[i]));
    }
    characterMesh.Release();
    gui2d.Destroy();
    SDL_DestroyRenderer(renderer);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    
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

void SetBlockTexture(){

    for(int i = 0; i < BLOCK_TYPE_NUM; i++){
        SDL_Surface *img = IMG_Load(TextureFileName[i]);
        if(!img){
            //エラー処理
            fprintf(stderr,"image file \"%s\" cannot loaded",TextureFileName[i]);
        }else{
            glGenTextures(1,&(BlockTexture[i]));
            glBindTexture(GL_TEXTURE_2D,BlockTexture[i]);
            int Mode = GL_RGB;
            if(img->format->BytesPerPixel == 4){
                Mode = GL_RGBA;
            }
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

            glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, Mode, GL_UNSIGNED_BYTE, img->pixels);
            SDL_FreeSurface(img);

        }
    }
}

SDL_Color HSVtoRGB(int h){
    SDL_Color res = {255, 255, 255, 255};
    float s = 90, l = 55;
    float max, min;
    max = 2.55 * (l + (100 - l) * s/100.0);
    min = 2.55 * (l - (100 - l) * s/100.0);

    if(h < 60){
        res.r = max;
        res.g = (h * (max - min) / 60.0) + min;
        res.b = min;
    }else if(h < 120){
        res.r = ((120 - h) * (max - min) / 60.0) + min;
        res.g = max;
        res.b = min;
    }else if(h < 180){
        res.r = min;
        res.g = max;
        res.b = ((h - 120) * (max - min) / 60.0) + min;
    }else if(h < 240){
        res.r = min;
        res.g = ((240 - h) * (max - min) / 60.0) + min;
        res.b = max;
    }else if(h < 300){
        res.r = ((h - 240) * (max - min) / 60.0) + min;
        res.g = min;
        res.b = max;
    }else{
        res.r = max;
        res.g = min;
        res.b = ((360 - h) * (max - min) / 60.0) + min;
    }
    return res;
}

void SetPlayerColors(){
    for(int i = 0; i < MAX_NUMCLIENTS; i++){
        int H = i * 360 / (MAX_NUMCLIENTS + 1);
        playerColors[i] = HSVtoRGB(H);
        //fprintf(stderr,"%d, %d, %d, %d\n",playerColors[i].r, playerColors[i].g, playerColors[i].b, playerColors[i].a);
    }
}

//2D描画用設定
void View2D() {
    glDisable(GL_DEPTH_TEST); //Depthバッファ無効
    glDisable( GL_LIGHTING ); //光源処理無効
    glMatrixMode(GL_PROJECTION);// 射影変換行列設定
    glPopMatrix();// 射影変換行列を復元
    glPushMatrix();// 現在の射影変換行列を保存
    glOrtho(0, Wd_Width, Wd_Height, 0, -1, 1);// 正射影変換設定
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
    gluPerspective(60.0, (double)Wd_Width / (double)Wd_Height, 20.0, 2000.0);
    glMatrixMode(GL_MODELVIEW);// モデルビュー変換行列設定
    glPopMatrix();// モデルビュー行列を復元
    glPushMatrix();// 現在のモデルビュー行列を保存
    glLoadIdentity();// 単位行列を設定
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glOrtho(0.0, (double)Wd_Width, (double)Wd_Height, 0.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);

}

//3D描画
void Disp3D(){
    DrawCharacter();
    DrawMap();
}

//マップ描画
void DrawMap(){
    const int (*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = GetSystem().GetClientMap().GetTerrainData();
    FloatCube mcube;
    //仮宣言
    int map_w = GetSystem().GetClientMap().GetMapW(), map_d = GetSystem().GetClientMap().GetMapD(), map_h = GetSystem().GetClientMap().GetMapH();

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
        glVertex3f(map_w * BLOCK_MAGNIFICATION, 0.0, 0.0);
        glVertex3f(map_w * BLOCK_MAGNIFICATION, 0.0, map_d * BLOCK_MAGNIFICATION);
        glVertex3f(0.0, 0.0, map_d * BLOCK_MAGNIFICATION);
    glEnd();

    SDL_Color mapColor = {255,255,0,255};

    //3次元配列データ使用時
    for(int width = 0; width < map_w; width++){
        for(int depth = 0; depth < map_d; depth++){
            for(int hight = 0; hight < map_h; hight++){
                mcube = {
                    width * BLOCK_MAGNIFICATION, hight * BLOCK_MAGNIFICATION, depth * BLOCK_MAGNIFICATION,
                    BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION
                };
                if(terrainData[width][hight][depth] > 0) DrowMapBlock(mcube,(BlockType)(terrainData[width][hight][depth]));
            }
        }
    }

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,BlockTexture[0]);
    glBegin(GL_QUADS);
        glNormal3f(0.0,0.0,-1.0);
        glTexCoord2f(0.0, 0.0);glVertex3f(map_w * BLOCK_MAGNIFICATION, map_h * BLOCK_MAGNIFICATION, map_d * BLOCK_MAGNIFICATION);
        glTexCoord2f(1.0, 0.0);glVertex3f(0, map_h * BLOCK_MAGNIFICATION, map_d * BLOCK_MAGNIFICATION);
        glTexCoord2f(1.0, 1.0);glVertex3f(0, 0, map_d * BLOCK_MAGNIFICATION);
        glTexCoord2f(0.0, 1.0);glVertex3f(map_w * BLOCK_MAGNIFICATION, 0, map_d * BLOCK_MAGNIFICATION);
    glEnd();
    glDisable(GL_TEXTURE_2D);

}

//キャラクター描画
void DrawCharacter(){
    const PlayerData* playerData = GetSystem().GetPlayerData();
    //仮宣言
    int myid = GetSystem().GetMyID();
    float ver = MAXIMUM_ANGLE_DOWN + (0 - MAXIMUM_ANGLE_DOWN) * ((playerData[myid].direction.vertical - MAXIMUM_ANGLE_DOWN) / (MAXIMUM_ANGLE_UP - MAXIMUM_ANGLE_DOWN));

    //視点変更
    lookatPlace.x = playerData[myid].pos.x + playerData[myid].pos.w / 2 + 60 * -sin(playerData[myid].direction.horizontal);
    lookatPlace.y = playerData[myid].pos.y + playerData[myid].pos.h / 2 + 60 * -sin(ver);
    lookatPlace.z = playerData[myid].pos.z + playerData[myid].pos.d / 2 + 60 * -cos(playerData[myid].direction.horizontal);

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

    //設置場所の表示
    PlaceData placeData = GetSystem().GetPlaceData();
    placeData.pos.x /= BLOCK_MAGNIFICATION;
    placeData.pos.y /= BLOCK_MAGNIFICATION;
    placeData.pos.z /= BLOCK_MAGNIFICATION;

    FloatCube mcube = {
        placeData.pos.x * BLOCK_MAGNIFICATION, placeData.pos.y * BLOCK_MAGNIFICATION, placeData.pos.z * BLOCK_MAGNIFICATION,
        BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION, BLOCK_MAGNIFICATION
    };
    SDL_Color color = {0, 0, 0, 255};
    RotateCube(mcube, 0.0, &color);

    for(int i = 0; i < GetSystem().Num_Clients; i++){
        //fprintf(stderr,"%d, %d, %d, %d\n",playerColors[i].r, playerColors[i].g, playerColors[i].b, playerColors[i].a);
        FloatCube ccube = playerData[i].pos;
        GLfloat diffuse[] = {playerColors[i].r / 255.0f, playerColors[i].g / 255.0f, playerColors[i].b / 255.0f, playerColors[i].a / 255.0f};
        GLfloat ambient  [] = { 0.1f, 0.1f, 0.1f, 1.0f};
        GLfloat specular [] = { 1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat shininess[] = { 0.0f};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        characterMesh.Draw(&(playerData[i].pos),playerData[i].direction.horizontal);
        //RotateCube(ccube,playerData[i].direction.horizontal, &(playerColors[i]));
    }
}


void DrowMapBlock(FloatCube cube, BlockType block){

    glEnable(GL_TEXTURE_2D);
    if(block == GoalBlock) glBindTexture(GL_TEXTURE_2D,BlockTexture[0]);
    else if(block < BLOCK_TYPE_NUM) glBindTexture(GL_TEXTURE_2D,BlockTexture[block]);

    // マテリアルを設定する
    GLfloat diffuse[] = {1,1,1,1};
    GLfloat ambient  [] = { 0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat specular [] = { 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[] = { 0.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glPolygonMode(GL_FRONT,GL_FILL);

    FloatPosition base[4] = {
        {cube.x,cube.y,cube.z},
        {cube.x + cube.w,cube.y,cube.z},
        {cube.x + cube.w,cube.y,cube.z+cube.d},
        {cube.x,cube.y,cube.z+cube.d}
    };
    FloatPosition top[4];

    for(int i = 0; i < 4; i++){
        top[i] = base[i];
        top[i].y += cube.h;
    }

    // 立方体を描画する
    glBegin(GL_QUADS);
        //上
        glNormal3f(0.0,1.0,0.0);
        glTexCoord2f(1.0, 0.5);glVertex3f(top[0].x,top[0].y,top[0].z);
        glTexCoord2f(0.0, 0.5);glVertex3f(top[1].x,top[1].y,top[1].z);
        glTexCoord2f(0.0, 0.0);glVertex3f(top[2].x,top[2].y,top[2].z);
        glTexCoord2f(1.0, 0.0);glVertex3f(top[3].x,top[3].y,top[3].z);
        //下
        glNormal3f(0.0,-1.0,0.0);
        glTexCoord2f(1.0, 0.0);glVertex3f(base[0].x, base[0].y, base[0].z);
        glTexCoord2f(0.0, 0.0);glVertex3f(base[1].x, base[1].y, base[1].z);
        glTexCoord2f(0.0, 0.5);glVertex3f(base[2].x, base[2].y, base[2].z);
        glTexCoord2f(1.0, 0.5);glVertex3f(base[3].x, base[3].y, base[3].z);
        //手前
        glNormal3f(0.0,0.0,-1.0);
        glTexCoord2f(1.0, 1.0);glVertex3f(base[0].x, base[0].y, base[0].z);
        glTexCoord2f(1.0, 0.5);glVertex3f(top[0].x,top[0].y,top[0].z);
        glTexCoord2f(0.0, 0.5);glVertex3f(top[1].x,top[1].y,top[1].z);
        glTexCoord2f(0.0, 1.0);glVertex3f(base[1].x, base[1].y, base[1].z);
        //左
        glNormal3f(-1.0,0.0,0.0);
        glTexCoord2f(1.0, 1.0);glVertex3f(base[1].x, base[1].y, base[1].z);
        glTexCoord2f(1.0, 0.5);glVertex3f(top[1].x,top[1].y,top[1].z);
        glTexCoord2f(0.0, 0.5);glVertex3f(top[2].x,top[2].y,top[2].z);
        glTexCoord2f(0.0, 1.0);glVertex3f(base[2].x, base[2].y, base[2].z);
        //右
        glNormal3f(1.0,0.0,0.0);
        glTexCoord2f(0.0, 1.0);glVertex3f(base[0].x, base[0].y, base[0].z);
        glTexCoord2f(0.0, 0.5);glVertex3f(top[0].x,top[0].y,top[0].z);
        glTexCoord2f(1.0, 0.5);glVertex3f(top[3].x,top[3].y,top[3].z);
        glTexCoord2f(1.0, 1.0);glVertex3f(base[3].x, base[3].y, base[3].z);
        //奥
        glNormal3f(0.0,0.0,1.0);
        glTexCoord2f(1.0, 1.0);glVertex3f(base[2].x, base[2].y, base[2].z);
        glTexCoord2f(1.0, 0.5);glVertex3f(top[2].x,top[2].y,top[2].z);
        glTexCoord2f(0.0, 0.5);glVertex3f(top[3].x,top[3].y,top[3].z);
        glTexCoord2f(0.0, 1.0);glVertex3f(base[3].x, base[3].y, base[3].z);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void RotateCube(FloatCube cube, double dir, SDL_Color *color){

    // マテリアルを設定する
    GLfloat diffuse[] = {1,1,1,1};
    if(color != NULL){
        diffuse[0] = color->r / 255.0f;
        diffuse[1] = color->g / 255.0f;
        diffuse[2] = color->b / 255.0f;
        diffuse[3] = color->a / 255.0f;
    }

    GLfloat ambient  [] = { 0.1f, 0.1f, 0.1f, 1.0f};
    GLfloat specular [] = { 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat shininess[] = { 0.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

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
    
    glLineWidth(3.0);

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
