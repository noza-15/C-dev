#include <GL/glut.h>
#include "global.h"
#include "menu.h"

#include <stdio.h> //debug

int window_width;
int window_height;
int viewport_start_x;
int viewport_start_y;
int viewport_width;
int viewport_height;
int frame_count;

STATE state;

static int MENU_INIT_FLAG = UNFINISHED;

// ゲーム開始時に実行する
static void menu_init(void){
    printf("now menu initialize.\n");
}

// ゲームを終わる時に実行する
static void menu_exit(void){
    printf("now menu exit.\n");
}

static void DrawString(char* str, int w, int h, int x0, int y0){
    glDisable(GL_LIGHTING);
    // 平行投影にする
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // 画面上にテキスト描画
    glColor3d(1.0,0.0,0.0);
    glRasterPos2f(x0, y0);
    int i = 0;
    while(str[i]!='\0')i++;
    int size = i;
    for(i = 0; i < size; i++){
        char ic = str[i];
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void menu_disp(void){

    if(MENU_INIT_FLAG == UNFINISHED){
        menu_init();
        MENU_INIT_FLAG = FINISHED;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double)viewport_width / (double)viewport_height, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    DrawString("start",1280,720,20,20);
    DrawString("press g key : to start",1280,720,20,50);


    glutSwapBuffers();
}

void menu_keyboard(unsigned char key, int x, int y){

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

void menu_mouse(int button, int mouse_state, int x, int y){

}
