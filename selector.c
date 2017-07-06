
#include <GL/glut.h>
#include "global.h"
#include "selector.h"

#include <stdio.h> // debug

static int SELECTOR_INIT_FLAG = UNFINISHED;

// ゲーム開始時に実行する
static void selector_init(void){
    printf("now selector initialize.\n");
}

// ゲームを終わる時に実行する
static void selector_exit(void){
    printf("now selector exit.\n");
}

// スコア・ランキング画面の描画関数
void selector_disp(void){
    if(SELECTOR_INIT_FLAG == UNFINISHED){
        selector_init();
        SELECTOR_INIT_FLAG = FINISHED;
    }

    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glClearColor(0.8,0.3,0.3,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutSwapBuffers();
}

// スコア・ランキングでのキーボード入力
void selector_keyboard(unsigned char key, int x, int y){

}

// スコア・ランキングでの特殊キー入力
void selector_special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_LEFT:
            selector_exit();
            SELECTOR_INIT_FLAG = UNFINISHED;
            state = MENU;
            break;
        case GLUT_KEY_RIGHT:
            selector_exit();
            SELECTOR_INIT_FLAG = UNFINISHED;
            state = GAME;
            break;
    }
}

// スコア・ランキングでのマウス入力
void selector_mouse(int button, int mouse_state, int x, int y){

}

// スコア・ランキングでのマウスドラッグ入力
void selector_motion(int x, int y){

}

// スコア・ランキングでのマウスの動き入力
void selector_passiveMotion(int x, int y){

}
