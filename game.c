#include <GL/glut.h>
#include <math.h>
#include "global.h"
#include "game.h"

#include <stdio.h> // debug

int window_width;
int window_height;
int viewport_start_x;
int viewport_start_y;
int viewport_width;
int viewport_height;
int frame_count;

STATE state;

static int GAME_INIT_FLAG = UNFINISHED;
static int DEVELOPPE_MODE = OFF;

static int jump_time = -1;
static int jump_f    = 0;
static double jump_v    = 0;
static double jump_z    = 0;

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

static double play_pos(int flag){
    double g = 17.0;
    double jump = 13.0;

    double z;

    double second = (frame_count-jump_time)/50.0;

    if(flag == 1){
        if(jump_time<0){
            jump_z = 0.0;
            jump_v = jump;
        }else{
            jump_z = jump_z + jump_v*second - g/2.0*second*second;
            jump_v = jump_v - g*second + jump;
        }
        jump_time = frame_count;
    }
    if(jump_time < 0)   z = 0.0;
    else                z = jump_z + jump_v*second - g/2.0*second*second;

    if(z>20.0)z = 20.0;
    if(z< -20.0)z = -20.0;

    return z;
}

// ゲーム開始時に実行する
static void game_init(void){
    printf("now game initialize.\n");
    glEnable(GL_LIGHTING);
    jump_time = -1;
    jump_time = -1;
    jump_f    = 0;
    jump_v    = 0;
    jump_z    = 0;
}

// ゲームを終わる時に実行する
static void game_exit(void){
    printf("now game exit.\n");
    jump_time = -1;
}

// 座標軸を描画する関数
static void coordinate_axis(double length){
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
}

// 座標軸方向を表示する
static void direction(double position,double scale){
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
}

// XZ平面上に目盛りを追加する関数
static void lines(int max, double interval){
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
}

// プレイヤー向け描画関数
static void player_disp(int width,int height){

    if(GAME_INIT_FLAG == UNFINISHED){
        game_init();
        GAME_INIT_FLAG = FINISHED;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double)width / (double)height, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    double rad = frame_count / 10.0;

    double z = play_pos(0);

    glPushMatrix();
    //glTranslatef(3.0*cos(rad), z, 3.0*sin(rad));
    glTranslatef(0.0, z, 0.0);
    glRotated(0.0, 1.0, 0.0, 0.0);
    glRotated(rad, 0.0, 1.0, 0.0);
    glRotated(0.0, 0.0, 0.0, 1.0);
    glutSolidCube(2);
    glPopMatrix();

}

// 開発者モードで呼び出される描画関数
static void developper_disp(void){
    // 左半分に描画するよう設定
    glViewport(viewport_start_x, viewport_start_y,
                viewport_width/2, viewport_height);
    player_disp(viewport_width/2,viewport_height);

    // 右半分に描画するように設定
    glViewport(viewport_start_x+viewport_width/2, viewport_start_y,
                viewport_width/2, viewport_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double)viewport_width/2 / (double)viewport_height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10.0, 10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glDisable(GL_LIGHTING);
    coordinate_axis(20.0);  // 座標軸を表示
    lines(20,0.5);          // XZ平面の目盛りを表示
    direction(4.0,1.0);     // 座標軸の方向を表示
    glEnable(GL_LIGHTING);
    player_disp(viewport_width/2,viewport_height);


    // 描画領域を全体に戻す
    glViewport(viewport_start_x, viewport_start_y,
                viewport_width, viewport_height);
}

// ゲーム状態で呼び出される描画関数
void game_disp(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 描画をクリア

    if(DEVELOPPE_MODE == ON){       // 開発者モードなら
        developper_disp();          // 開発者モードの描画関数を呼び出す
    }else{                          // 開発者モードでないなら
        player_disp(viewport_width,viewport_height);    // プレイヤー向けの描画関数を呼び出す
    }

    glDisable(GL_LIGHTING);
    glColor3d(0.9,0.3,0.3);
    printString("press d key to developper mode",30,30);
    printString("press UP key to start and jump",30,60);
    glEnable(GL_LIGHTING);

    glutSwapBuffers();              // 描画を更新
}

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
        case 'd':
            if(DEVELOPPE_MODE == ON){
                printf("developper mode off.\n");
                DEVELOPPE_MODE = OFF;
            }else{
                printf("developper mode on.\n");
                DEVELOPPE_MODE = ON;
            }
    }
}

void game_special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            play_pos(1);
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


void game_mouse(int button, int mouse_state, int x, int y){

}
