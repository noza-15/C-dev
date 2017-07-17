
#include <stdlib.h>
#include <GL/glut.h>
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

// コンテンツの中身

// ゲーム開始時に実行する
static void ranking_init(void){
    printf("now ranking initialize.\n");

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

    // background
    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,10.0,0.0,10.0*viewport_height/viewport_width,0.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);

    // ここに背景

    // ページタイトル用変換行列設定
    // ページタイトルバック

    // page title
    glColor3d(1.0,1.0,1.0);
    drawRanking(0.0,10.0*viewport_height/viewport_width-0.5,0.0,0,0.0);


    // いらない
    glMaterialfv(GL_FRONT, GL_AMBIENT, ruby.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ruby.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, &ruby.shininess);
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0,0.0,1.0);
    glVertex3d(1.0,1.0,0.0);
    glVertex3d(2.0,1.0,0.0);
    glVertex3d(1.0,2.0,0.0);
    glEnd();

    // ページコンテンツ用変換行列設定
    // ページコンテンツバック

    // ページコンテンツ

    // メニューに戻る
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
