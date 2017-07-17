
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "global.h"
#include "ranking.h"
#include "raw_data.h"

#include <stdio.h> // debug

static int RANKING_INIT_FLAG = UNFINISHED;

typedef struct{
    GLfloat  ambient[4];
    GLfloat  diffuse[4];
    GLfloat  specular[4];
    GLfloat  shininess;
}Material;

typedef struct{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat position[4];
    GLfloat direction[3];
}Light;


// 物質定義
Material ruby    = {    {0.1745,   0.01175,  0.01175,   1.0},
                        {0.61424,  0.04136,  0.04136,   1.0},
                        {0.727811, 0.626959, 0.626959,  1.0},
                        76.8                                    };

Material saphire  = {   {0.01175,   0.01175, 0.1745,     1.0},
                        {  0.04136,  0.04136,0.61424,   1.0},
                        { 0.626959, 0.626959,0.727811,  1.0},
                        76.8                                    };

Material emerald  = {   {0.31175,    0.9745,   0.31175,  1.0},
                        {  0.24136,  0.81424, 0.24136,  1.0},
                        { 0.626959,0.727811, 0.626959,  1.0},
                        76.8                                    };

// 背景
#define POSMAX 36
#define SHIFT  0.4
#define SPEED  0.007
double pos[POSMAX] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

// コンテンツの中身
#define SCORELEN 5
char str[SCORELEN][20] = {"1, A.B : 20","1, A.B : 20","1, A.B : 20","1, A.B : 20","1, A.B : 20"};

// ゲーム開始時に実行する
static void ranking_init(void){
    printf("now ranking initialize.\n");

    int i;

    {
        Light light0    = { {0.15, 0.15, 0.15, 1.0},    // ambient
                            {1.0, 1.0, 1.0, 1.0},       // diffuse
                            {1.0, 1.0, 1.0, 1.0},       // specular
                            {0.0, 20.0, 20.0,            // porition
                            1.0},                       // type = spot
                            {0.0, -1.0, -1.0}        };  // direction

        glLightfv(GL_LIGHT0, GL_AMBIENT, light0.ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0.diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0.specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light0.position);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0.direction);
    }

    // fileの読み込み


    // posの初期化
    for(i=0;i<POSMAX;i++){
        pos[i] = i*SHIFT;
    }

}

// ゲームを終わる時に実行する
static void ranking_exit(void){
    printf("now ranking exit.\n");
    {
        Light light0    = { {0.0, 0.0, 0.0, 1.0},    // ambient
                            {1.0, 1.0, 1.0, 1.0},       // diffuse
                            {1.0, 1.0, 1.0, 1.0},       // specular
                            {0.0, 0.0, 1.0,            // porition
                            0.0},                       // type = spot
                            {0.0, 0.0, -1.0}        };  // direction

        glLightfv(GL_LIGHT0, GL_AMBIENT, light0.ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light0.diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light0.specular);

        glLightfv(GL_LIGHT0, GL_POSITION, light0.position);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0.direction);
    }

    {
        Material first  =   {   {0.2,    0.2,   0.2,  1.0},  // ambient
                                {  0.8,  0.8, 0.8,  1.0},   // diffuse
                                { 0.0,0.0, 0.0,  1.0},   // specular
                                0.0                                    };  // shininess
        glMaterialfv(GL_FRONT, GL_AMBIENT, first.ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, first.diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, first.specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, &first.shininess);
    }

    //コンテンツのフリー

}

// 画面に文字列を表示する関数
static void printString(char *str, int x0, int y0) {
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
    int length = 0;
    while (str[length] != '\0')length++;
    int i;
    for (i = 0; i < length; i++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// スコア・ランキング画面の描画関数
void ranking_disp(void){
    if(RANKING_INIT_FLAG == UNFINISHED){
        ranking_init();
        RANKING_INIT_FLAG = FINISHED;
    }

    float x = 10.0*cos(frame_count*0.03);
    float y = 10.0*sin(frame_count*0.03);

    float lp[4] = {x, y, 20.0, 1.0};  // spot light position
    float ld[3] = {-x, -y, -1.0};       // direction
    glLightfv(GL_LIGHT0, GL_POSITION, lp);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, ld);

    // background
    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,10.0,0.0,10.0*viewport_height/viewport_width,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.7,0.7,0.0);
    int i,j;
    glEnable(GL_LIGHTING);
    for(i=0,j=0;i<POSMAX;i++){
        pos[i] -= SPEED;
        if(pos[i]<-1.0)pos[i]=SHIFT*(POSMAX)-1.0;
        if(j==0){
            glMaterialfv(GL_FRONT, GL_AMBIENT, ruby.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, ruby.specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, &ruby.shininess);
            j=1;
        }else if(j==1){
            glMaterialfv(GL_FRONT, GL_AMBIENT, emerald.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, emerald.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, emerald.specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, &emerald.shininess);
            j=2;
        }else{
            glMaterialfv(GL_FRONT, GL_AMBIENT, saphire.ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, saphire.diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, saphire.specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, &saphire.shininess);
            j=0;
        }
        glBegin(GL_POLYGON);
        glNormal3f(0.0,0.0,1.0);
        glVertex3d(pos[i],      -20.0,  -0.9);
        glVertex3d(pos[i]+0.1,  -20.0,  -0.9);
        glVertex3d(pos[i]+0.1,  20.0,   -0.9);
        glVertex3d(pos[i],      20.0,   -0.9);
        glEnd();
    }

    // ページタイトル
    glViewport(viewport_start_x+viewport_width/2-viewport_height/2,viewport_start_y+viewport_height*3/4,
                viewport_height*2/3,viewport_height/6);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,4.0,0.0,1.0,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);
    glColor3d(0.2,0.2,0.2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ruby.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ruby.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &ruby.shininess);
    glBegin(GL_POLYGON);
    glNormal3f(0.0,0.0,1.0);
    glVertex3d(0.0,0.0,-0.01);
    glVertex3d(4.0,0.0,-0.01);
    glVertex3d(4.0,1.0,-0.01);
    glVertex3d(0.0,1.0,-0.01);
    glEnd();
    glEnable(GL_LIGHTING);
    glColor3d(1.0,1.0,1.0);
    drawRanking(0.0,0.55,0.0,0,0.0);


    // ページコンテンツback
    glViewport(viewport_start_x+viewport_width/2-viewport_height*4/9,viewport_start_y+viewport_height/9,
                viewport_height*8/9,viewport_height*5/9);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,8.0,0.0,5.0,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);
    glColor3d(0.2,0.2,0.2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, saphire.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, saphire.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, saphire.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &saphire.shininess);
    glBegin(GL_POLYGON);
    glNormal3f(0.0,0.0,1.0);
    glVertex3d(0.0,0.0,-0.7);
    glVertex3d(8.0,0.0,-0.7);
    glVertex3d(8.0,5.0,-0.7);
    glVertex3d(0.0,5.0,-0.7);
    glEnd();
    glEnable(GL_LIGHTING);

    // ページコンテンツ
    glColor3d(0.9,0.9,0.2);
    for(i=0;i<SCORELEN;i++){
        printString(str[i],100,100*i+100);
    }

    // メニューに戻る
    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glColor3d(0.9,0.9,0.9);
    printString("< back to menu | ", 40, viewport_height-40);

    glutSwapBuffers();
}

// スコア・ランキングでのキーボード入力
void ranking_keyboard(unsigned char key, int x, int y){

}

// スコア・ランキングでの特殊キー入力
void ranking_special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_LEFT:
            ranking_exit();
            RANKING_INIT_FLAG = UNFINISHED;
            state = MENU;
            break;
    }
}

// スコア・ランキングでのマウス入力
void ranking_mouse(int button, int mouse_state, int x, int y){

}

// スコア・ランキングでのマウスドラッグ入力
void ranking_motion(int x, int y){

}

// スコア・ランキングでのマウスの動き入力
void ranking_passiveMotion(int x, int y){

}
