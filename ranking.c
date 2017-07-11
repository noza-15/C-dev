
#include <GL/glut.h>
#include "global.h"
#include "ranking.h"

#include <stdio.h> // debug

static int RANKING_INIT_FLAG = UNFINISHED;

// ゲーム開始時に実行する
static void ranking_init(void){
    printf("now ranking initialize.\n");
}

// ゲームを終わる時に実行する
static void ranking_exit(void){
    printf("now ranking exit.\n");
}

// スコア・ランキング画面の描画関数
void ranking_disp(void){
    if(RANKING_INIT_FLAG == UNFINISHED){
        ranking_init();
        RANKING_INIT_FLAG = FINISHED;
    }

    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glClearColor(0.8,0.3,0.3,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
