#include <GL/glut.h>
#include <math.h>
#include "global.h"
#include "game.h"
#include "player.h"
#include "obstacle.h"
#include "background.h"

#include <stdio.h> // debug

// グローバル変数 ////////////////////////////////////////////////////////////////

int window_width;
int window_height;
int viewport_start_x;
int viewport_start_y;
int viewport_width;
int viewport_height;
int frame_count;
unsigned int refresh_msec;

STATE state;

// game ////////////////////////////////////////////////////////////////////////

static int GAME_INIT_FLAG = UNFINISHED; // game state
static int DEVELOPPE_MODE = OFF;        // game state

static double camera_x;             // devlopper camera
static double camera_y;             // devlopper camera
static double camera_z;             // devlopper camera
static double camera_px;            // devlopper camera
static double camera_py;            // devlopper camera
static double camera_pz;            // devlopper camera

static int game_over_flag;          // game system

///////////////////////////////////////////////////////////////////////////////

// 各フレームごとにゲームの状態を更新する関数
static void game_refresh(void){
    refreshResidents();
    double z = getPlayerPosition();
    if(judgeCollision(z))game_over_flag = ON;
}

// 画面に文字列を表示する関数
static void printString(char* str, int x0, int y0){
    glDisable(GL_LIGHTING);
    // 平行投影にする
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, viewport_width, viewport_height, 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 画面上にテキスト描画
    glRasterPos2f(x0, y0);
    int length = 0; while(str[length]!='\0')length++;
    int i;
    for(i=0;i<length;i++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ゲーム開始時に実行する
static void game_init(void){
    glEnable(GL_LIGHTING);
    playerInit();
    initObstacles();
    backgroundInit();

    camera_x = 0.0;
    camera_y = 0.0;
    camera_z = 20.0;
    camera_px = 0.0;
    camera_py = 0.0;
    camera_pz = -1.0;

    game_over_flag = OFF;

}

// ゲームを終わる時に実行する
static void game_exit(void){
    printf("now game exit.\n");
}


// debug ///////////////////////////////////////////////////////////////////////

// 座標軸を描画する関数
static void coordinate_axis(double length){
    glDisable(GL_LIGHTING);
    glPushMatrix();

    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3d( length,    0.0,    0.0);
    glVertex3d(-length,    0.0,    0.0);
    glEnd();

    glColor3d(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3d(    0.0, length,    0.0);
    glVertex3d(    0.0,-length,    0.0);
    glEnd();

    glColor3d(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3d(    0.0,    0.0, length);
    glVertex3d(    0.0,    0.0,-length);
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// 座標軸方向を表示する
static void direction(double position,double scale){
    glDisable(GL_LIGHTING);
    glPushMatrix();

    glPushMatrix();
    glColor3d(1.0, 0.0, 0.0);
    glTranslatef(position,0.0,0.0);
    glRotated(90,0.0,1.0,0.0);
    glutSolidCone(0.1*scale,0.5*scale,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.0, 1.0, 0.0);
    glTranslatef(0.0,position,0.0);
    glRotated(-90,1.0,0.0,0.0);
    glutSolidCone(0.1*scale,0.5*scale,10,10);
    glPopMatrix();

    glPushMatrix();
    glColor3d(0.0, 0.0, 1.0);
    glTranslatef(0.0,0.0,position);
    glutSolidCone(0.1*scale,0.5*scale,10,10);
    glPopMatrix();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// XZ平面上に目盛りを追加する関数
static void zx_lines(int max, double interval){
    glDisable(GL_LIGHTING);
    glPushMatrix();

    int i;
    double length = max * interval;

    glColor3d(0.2,0.2,0.2);
    glBegin(GL_LINES);
    for(i=1;i<20;i++){
        glVertex3d(i*interval,0,length);
        glVertex3d(i*interval,0,-length);

        glVertex3d(-i*interval,0,length);
        glVertex3d(-i*interval,0,-length);

        glVertex3d(length,0,i*interval);
        glVertex3d(-length,0,i*interval);

        glVertex3d(length,0,-i*interval);
        glVertex3d(-length,0,-i*interval);
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

// XY平面上に目盛りを追加する関数
static void xy_lines(int max, double interval){
    glDisable(GL_LIGHTING);
    glPushMatrix();

    int i;
    double length = max * interval;

    glColor3d(0.2,0.2,0.2);
    glBegin(GL_LINES);
    for(i=1;i<20;i++){
        glVertex3d(i*interval,length,0);
        glVertex3d(i*interval,-length,0);

        glVertex3d(-i*interval,length,0);
        glVertex3d(-i*interval,-length,0);

        glVertex3d(length,i*interval,0);
        glVertex3d(-length,i*interval,0);

        glVertex3d(length,-i*interval,0);
        glVertex3d(-length,-i*interval,0);
    }
    glEnd();

    glPopMatrix();
    glEnable(GL_LIGHTING);
}

////////////////////////////////////////////////////////////////////////////////

// プレイヤーのカメラ変換行列をかける関数()
static void setPlayerCam(void){
    glMatrixMode(GL_MODELVIEW);     // モデルビュー行列モードにする
    glLoadIdentity();               // 単位行列をロード
    gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// 開発者用のカメラ変換行列をかける関数
static void setDevCam(void){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(camera_x, camera_y, camera_z,
                camera_x+camera_px, camera_y+camera_py, camera_z+camera_pz, 0.0, 1.0, 0.0);
}

// 画面サイズに合わせてカメラのアスペクト比を決める
static void adjust_aspect(int width, int height){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double)width / (double)height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// ワールドの全てを描画する関数
static void renderAll(int width,int height){

    adjust_aspect(width,height);
    setPlayerCam();                 // カメラ変換行列をかける
    render_player();
    renderObstacles();
    backgroundRender();
}

// ゲームオーバー画面の描画
static void over_disp(void){
    printString("game over",30,30);
}

// ゲーム状態で呼び出される描画関数
void game_disp(void){

    if(GAME_INIT_FLAG == UNFINISHED){
        game_init();
        GAME_INIT_FLAG = FINISHED;
    }

    game_refresh();                 // ゲームの内部状態を更新

    if(DEVELOPPE_MODE == ON){       // 開発者モードなら
        glDisable(GL_LIGHTING);
        glColor3d(0.9,0.3,0.3);
        printString("press x key to exit developper mode",30,30);
        glEnable(GL_LIGHTING);

        // 左半分に描画するよう設定
        glViewport(viewport_start_x, viewport_start_y+viewport_height/2,
                    viewport_width/2, viewport_height/2);
        renderAll(viewport_width/2,viewport_height/2);

        // 右上に軸付きのプレイ画面を表示
        glViewport(viewport_start_x, viewport_start_y,
                    viewport_width/2, viewport_height/2);
        adjust_aspect(viewport_width/2, viewport_height/2);
        setPlayerCam();
        coordinate_axis(20.0);  // 座標軸を表示
        xy_lines(20,0.5);
        renderAll(viewport_width/2,viewport_height/2);

        // 右下に描画するように設定
        glViewport(viewport_start_x+viewport_width/2, viewport_start_y,
                    viewport_width/2, viewport_height);
        adjust_aspect(viewport_width/2, viewport_height);
        setDevCam();
        coordinate_axis(20.0);  // 座標軸を表示
        zx_lines(20,0.5);          // XZ平面の目盛りを表示
        direction(4.0,1.0);     // 座標軸の方向を表示
        renderAll(viewport_width/2,viewport_height);

    }else if(game_over_flag == OFF){                          // 開発者モードでないなら
        glDisable(GL_LIGHTING);
        glColor3d(0.9,0.3,0.3);
        printString("press x key to developper mode",30,30);
        printString("press UP key to start and jump",30,60);
        glEnable(GL_LIGHTING);

        glViewport(viewport_start_x, viewport_start_y,
                    viewport_width, viewport_height);
        renderAll(viewport_width,viewport_height);    // プレイヤー向けの描画関数を呼び出す
    }else{
        glViewport(viewport_start_x, viewport_start_y,
                    viewport_width, viewport_height);
        over_disp();
    }

    glutSwapBuffers();              // 描画を更新
}

// キーボードの入力で呼び出される関数
void game_keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'm':
            game_exit();
            if(DEVELOPPE_MODE == ON){
                printf("developper mode off.\n");
                DEVELOPPE_MODE = OFF;
            }
            GAME_INIT_FLAG = UNFINISHED;
            state = MENU;
            break;
        case 'w':
            break;
        case 'a':
            break;
        case 's':
            break;
        case 'd':
            break;
        case 'o':
            if(DEVELOPPE_MODE == ON) game_over_flag = ON;
            break;
        case 'x':
            if(DEVELOPPE_MODE == ON){
                printf("developper mode off.\n");
                DEVELOPPE_MODE = OFF;
            }else{
                printf("developper mode on.\n");
                DEVELOPPE_MODE = ON;
            }
    }
}

// 特殊なキーの入力で呼び出される関数
void game_special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            jump();
            break;
        case GLUT_KEY_LEFT:
            game_exit();
            if(DEVELOPPE_MODE == ON){
                printf("developper mode off.\n");
                DEVELOPPE_MODE = OFF;
            }
            GAME_INIT_FLAG = UNFINISHED;
            state = MENU;
            break;
    }
}

// マウスの入力で呼び出される関数
void game_mouse(int button, int mouse_state, int x, int y){

}
