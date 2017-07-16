
#include <stdlib.h> // 必ずglut.hより先にインクルードする(exit()の多重定義で怒られるので)
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include "global.h"
#include "menu.h"
#include "raw_data.h"

#include <stdio.h> //debug

static int MENU_INIT_FLAG = UNFINISHED;

typedef enum{
    selectGAME,
    selectRANKING,
    selectQUIT
}CURSOR;

#define OBJLEN 10

static double objs[OBJLEN][6] = {
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
    {0.0,0.0,0.0,0.0,0.0,0.0},
};

static CURSOR cursor = selectGAME;

// ゲーム開始時に実行する
static void menu_init(void){
    printf("now menu initialize.\n");
    srand((unsigned)time(NULL));
    int i;
    for(i=0;i<OBJLEN;i++){
        objs[i][0] = (double)rand()/RAND_MAX*40.0-20.0;
        objs[i][1] = (double)rand()/RAND_MAX*40.0-20.0;
        objs[i][2] = (double)rand()/RAND_MAX*-20.0;
        objs[i][3] = (double)rand()/RAND_MAX*180.0;
        objs[i][4] = (double)rand()/RAND_MAX*180.0;
        objs[i][5] = (double)rand()/RAND_MAX*180.0;
        printf("%lf,%lf,%lf\n",objs[i][0],objs[i][1],objs[i][2]);
    }
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

    // background
    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0, (double)viewport_width / (double)viewport_height, 1.0, 30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,10.0,0.0,0.0,0.0,0.0,1.0,0.0);
    int i;
    for(i=0;i<OBJLEN;i++){
        glPushMatrix();
        objs[i][1] -= 0.7;
        objs[i][3] += 3.0;
        objs[i][4] += 3.0;
        objs[i][5] += 3.0;
        if(objs[i][1]<-20.0){
            objs[i][0] = (double)rand()/RAND_MAX*40-20;
            objs[i][1] = 20.0;
            objs[i][2] = (double)rand()/RAND_MAX*-20;
            objs[i][3] = (double)rand()/RAND_MAX*180;
            objs[i][4] = (double)rand()/RAND_MAX*180;
            objs[i][5] = (double)rand()/RAND_MAX*180;
        }
        glTranslatef(objs[i][0],objs[i][1],objs[i][2]);
        glRotated(objs[i][3],1.0,0.0,0.0);
        glRotated(objs[i][4],0.0,1.0,0.0);
        glRotated(objs[i][5],0.0,0.0,1.0);
        glutSolidCube(1);
        glPopMatrix();
    }

    // title
    glViewport(viewport_start_x+viewport_width/2-viewport_height/3,viewport_start_y+viewport_height*2/3,
                viewport_height*2/3,viewport_height/6);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,4.0,0.0,1.0,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);
    glColor3d(0.4,0.1,0.7);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glVertex3d(0.0,0.0,-0.01);
    glVertex3d(4.0,0.0,-0.01);
    glVertex3d(4.0,1.0,-0.01);
    glVertex3d(0.0,1.0,-0.01);
    glEnd();
    glEnable(GL_LIGHTING);
    glColor3d(1.0,1.0,1.0);
    drawTitle(0.0,0.5,0.0,0,0.0);

    // menu
    glViewport(viewport_start_x+viewport_width/2+viewport_height/9,viewport_start_y+viewport_height/9,
                viewport_height*4/9,viewport_height*4/9);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,5.0,0.0,5.0,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);
    glColor3d(0.8,0.3,0.7);
    glDisable(GL_LIGHTING);
    glBegin(GL_POLYGON);
    glVertex3d(0.0,0.0,-0.01);
    glVertex3d(5.0,0.0,-0.01);
    glVertex3d(5.0,5.0,-0.01);
    glVertex3d(0.0,5.0,-0.01);
    glEnd();
    glEnable(GL_LIGHTING);
    if(cursor == selectGAME)    glColor3d(0.9,0.9,0.4);
    else                        glColor3d(0.1,0.1,0.1);
    drawGameStart(0.0,4.0,0.0,0,0.0);
    if(cursor == selectRANKING)   glColor3d(0.9,0.9,0.4);
    else                        glColor3d(0.1,0.1,0.1);
    drawRanking(0.0,3.0,0.0,0,0.0);
    if(cursor == selectQUIT)    glColor3d(0.9,0.9,0.4);
    else                        glColor3d(0.1,0.1,0.1);
    drawExit(0.0,2.0,0.0,0,0.0);

    glutSwapBuffers();
}

// メニュー画面でのキーボード受け取り
void menu_keyboard(unsigned char key, int x, int y){

    printf("%d\n",key);

    switch(key){
        case 'g':
            menu_exit();
            MENU_INIT_FLAG = UNFINISHED;
            state = SELECTOR;
            break;
        case 's':
            menu_exit();
            MENU_INIT_FLAG = UNFINISHED;
            state = RANKING;
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
                    state = SELECTOR;
                    break;
                case selectRANKING:
                    menu_exit();
                    MENU_INIT_FLAG = UNFINISHED;
                    state = RANKING;
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

// メニュー画面でのマウスドラッグの受け取り
void menu_motion(int x, int y){

}

// メニュー画面でのマウスの動きの受け取り
void menu_passiveMotion(int x, int y){

}
