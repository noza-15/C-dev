
#include <GL/glut.h>
#include "global.h"
#include "score.h"

#include <stdio.h> // debug

static int SCORE_INIT_FLAG = UNFINISHED;

// ゲーム開始時に実行する
static void score_init(void){
    printf("now score initialize.\n");
}

// ゲームを終わる時に実行する
static void score_exit(void){
    printf("now score exit.\n");
}

// スコア・ランキング画面の描画関数
void score_disp(void){
    if(SCORE_INIT_FLAG == UNFINISHED){
        score_init();
        SCORE_INIT_FLAG = FINISHED;
    }

    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glClearColor(0.8,0.3,0.3,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutSwapBuffers();
}

// スコア・ランキングでのキーボード入力
void score_keyboard(unsigned char key, int x, int y){

}

// スコア・ランキングでの特殊キー入力
void score_special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_LEFT:
            score_exit();
            SCORE_INIT_FLAG = UNFINISHED;
            state = MENU;
            break;
    }
}

// スコア・ランキングでのマウス入力
void score_mouse(int button, int mouse_state, int x, int y){

}
