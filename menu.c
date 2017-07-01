#include <stdlib.h> // 必ずglut.hより先にインクルードする(exit()の多重定義で怒られるので)
#include <GL/glut.h>
#include "global.h"
#include "menu.h"

#include <stdio.h> //debug


// グローバル変数 ////////////////////////////////////////////////////////////////
int window_width;
int window_height;
int viewport_start_x;
int viewport_start_y;
int viewport_width;
int viewport_height;
int frame_count;

STATE state;

////////////////////////////////////////////////////////////////////////////////

static int MENU_INIT_FLAG = UNFINISHED;

typedef enum{
    selectGAME,
    selectSCORE,
    selectQUIT
}CURSOR;

static CURSOR cursor = selectGAME;

// ゲーム開始時に実行する
static void menu_init(void){
    printf("now menu initialize.\n");
}

// ゲームを終わる時に実行する
static void menu_exit(void){
    printf("now menu exit.\n");
}

// 画面に文字を描画する関数
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

    glEnable(GL_LIGHTING);
}

// メニュー画面での描画関数
void menu_disp(void){

    if(MENU_INIT_FLAG == UNFINISHED){
        menu_init();
        MENU_INIT_FLAG = FINISHED;
    }

    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double)viewport_width / (double)viewport_height, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3d(1.0,1.0,1.0);
    printString("menu",620,300);
    if(cursor == selectGAME)    glColor3d(0.9,0.9,0.4);
    else                        glColor3d(0.4,0.4,0.4);
    printString("game start : press g key",630,320);
    if(cursor == selectSCORE)   glColor3d(0.9,0.9,0.4);
    else                        glColor3d(0.4,0.4,0.4);
    printString("score ranking : press s key",630,340);
    if(cursor == selectQUIT)    glColor3d(0.9,0.9,0.4);
    else                        glColor3d(0.4,0.4,0.4);
    printString("quit : press q key",630,360);

    glutSwapBuffers();
}

// メニュー画面でのキーボード受け取り
void menu_keyboard(unsigned char key, int x, int y){

    printf("%d\n",key);

    switch(key){
        case 'g':
            menu_exit();
            MENU_INIT_FLAG = UNFINISHED;
            state = GAME;
            break;
        case 's':
            menu_exit();
            MENU_INIT_FLAG = UNFINISHED;
            state = SCORE;
            break;
        default :
            break;
    }
}

// メニュー画面での特殊キー受け取り
void menu_special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            if(cursor == selectGAME) cursor = selectQUIT;
            else cursor--;
            break;
        case GLUT_KEY_DOWN:
            if(cursor == selectQUIT) cursor = selectGAME;
            else cursor++;
            break;
        case GLUT_KEY_RIGHT:
            switch(cursor){
                case selectGAME:
                    menu_exit();
                    MENU_INIT_FLAG = UNFINISHED;
                    state = GAME;
                    break;
                case selectSCORE:
                    menu_exit();
                    MENU_INIT_FLAG = UNFINISHED;
                    state = SCORE;
                    break;
                case selectQUIT:
                    exit(0);
                    break;
            }
            break;
    }
}

// メニュー画面でのマウス受け取り
void menu_mouse(int button, int mouse_state, int x, int y){

}
