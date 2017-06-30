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

// ゲーム開始時に実行する
static void game_init(void){
    printf("now game initialize.\n");
}

// ゲームを終わる時に実行する
static void game_exit(void){
    printf("now game exit.\n");
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

    glDisable(GL_LIGHTING);
    coordinate_axis(20.0);  // 座標軸を表示
    lines(20,0.5);          // XZ平面の目盛りを表示
    direction(4.0,1.0);     // 座標軸の方向を表示
    glEnable(GL_LIGHTING);

    double rad = frame_count / 10.0;

    glPushMatrix();
    glTranslatef(3.0*cos(rad), 0.0, 3.0*sin(rad));
    glRotated(0.0, 1.0, 0.0, 0.0);
    glRotated(rad, 0.0, 1.0, 0.0);
    glRotated(0.0, 0.0, 0.0, 1.0);
    glutSolidCube(2);
    glPopMatrix();

}

static void developper_disp(void){
    glViewport(viewport_start_x, viewport_start_y,
                viewport_width/2, viewport_height);
    player_disp(viewport_width/2,viewport_height);

    glViewport(viewport_start_x+viewport_width/2, viewport_start_y,
                viewport_width/2, viewport_height);
    player_disp(viewport_width/2,viewport_height);

    glViewport(viewport_start_x, viewport_start_y,
                viewport_width, viewport_height);
}

void game_disp(void){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(DEVELOPPE_MODE == ON){
        developper_disp();
    }else{
        player_disp(viewport_width,viewport_height);
    }
    
    glutSwapBuffers();
}

void game_keyboard(unsigned char key, int x, int y){
    if(key == 'e') state = MENU;
    switch(key){
        case 'e':
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

void game_mouse(int button, int mouse_state, int x, int y){

}
