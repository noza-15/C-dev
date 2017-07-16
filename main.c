
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "global.h"

#include "menu.h"
#include "selector.h"
#include "game.h"
#include "ranking.h"

#define INITIAL_WIDTH 1280
#define INITIAL_HHEIGHT 720
#define GAME_NAME "myapp"

/* グローバル変数の宣言 */
int window_width;
int window_height;
int viewport_start_x;
int viewport_start_y;
int viewport_width;
int viewport_height;
int frame_count;
unsigned int refresh_msec;
STATE state;

// 固定フレームレートで実行する関数
void timer(int value) {
    frame_count++;
    if (frame_count > MAX_FRAME) frame_count = 0;
    glutPostRedisplay();                    // 描画関数の呼び出し
    glutTimerFunc(refresh_msec, timer, 0);    // refresh_msec後にもう一度この関数を呼び出す
}

// 描画関数
void display(void) {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    switch (state) {
        case MENU:
            menu_disp();
            break;
        case SELECTOR:
            selector_disp();
            break;
        case GAME:
            game_disp();
            break;
        case GAME_OVER:
            over_disp();
            break;
        case RANKING:
            ranking_disp();
            break;
    }
}

// ウィンドウサイズが変わった時に呼び出される関数
void reshape(int width, int height) {
    window_width = width;
    window_height = height;
    viewport_width = width;
    viewport_height = height;
    if ((double) width > (double) height * 16.0 / 9.0) {        // 横幅が大きすぎたら
        viewport_start_x = width / 2 - (int) (height * 8.0 / 9.0);
        viewport_start_y = 0;
        viewport_width = (int) (height * 16.0 / 9.0);
        viewport_height = height;
    } else if ((double) height > (double) width * 3.0 / 4.0) {   // 縦幅が大きすぎたら
        viewport_start_x = 0;
        viewport_start_y = height / 2 - (int) (width * 1.5 / 4.0);
        viewport_width = width;
        viewport_height = (int) (width * 3.0 / 4.0);
    }
}

// キーボードの入力があった時に呼び出される関数
void keyboard(unsigned char key, int x, int y) {
    if (key == '\033' || key == 'q') { // とりあえずescを押すと終了
        printf("application end.\n");
        exit(0);
    }
    switch (state) {
        case MENU:
            menu_keyboard(key, x, y);
            break;
        case SELECTOR:
            selector_keyboard(key, x, y);
            break;
        case GAME:
            game_keyboard(key, x, y);
            break;
        case GAME_OVER:
            over_keyboard(key, x, y);
            break;
        case RANKING:
            ranking_keyboard(key, x, y);
            break;
    }
}

// 特殊なキーの入力があった時に呼び出される関数
void special(int key, int x, int y) {
    switch (state) {
        case MENU:
            menu_special(key, x, y);
            break;
        case SELECTOR:
            selector_special(key, x, y);
            break;
        case GAME:
            game_special(key, x, y);
            break;
        case GAME_OVER:
            break;
        case RANKING:
            ranking_special(key, x, y);
            break;
    }
}

// マウスの入力があった時に呼び出される関数
void mouse(int button, int mouse_state, int x, int y) {
    switch (state) {
        case MENU:
            menu_mouse(button, mouse_state, x, y);
            break;
        case SELECTOR:
            selector_mouse(button, mouse_state, x, y);
            break;
        case GAME:
            game_mouse(button, mouse_state, x, y);
            break;
        case GAME_OVER:
            break;
        case RANKING:
            ranking_mouse(button, mouse_state, x, y);
            break;
    }
}

// マウスのドラッグがあった時に呼び出される関数
void motion(int x, int y) {
    switch (state) {
        case MENU:
            menu_motion(x, y);
            break;
        case SELECTOR:
            selector_motion(x, y);
            break;
        case GAME:
            game_motion(x, y);
            break;
        case GAME_OVER:
            break;
        case RANKING:
            ranking_motion(x, y);
            break;
    }
}

// マウスの動きがあった時に呼び出される関数
void passiveMotion(int x, int y) {
    switch (state) {
        case MENU:
            menu_passiveMotion(x, y);
            break;
        case SELECTOR:
            selector_passiveMotion(x, y);
            break;
        case GAME:
            game_passiveMotion(x, y);
            break;
        case GAME_OVER:
            break;
        case RANKING:
            ranking_passiveMotion(x, y);
            break;
    }
}

// 初期化関数
void init(void) {
    glEnable(GL_DEPTH_TEST);            // 深度判定を行う

    glEnable(GL_CULL_FACE);             // 裏面除去を使う
    glCullFace(GL_BACK);                // 除去するのは裏面のみ

    glEnable(GL_LIGHTING);              // ライティングを使う
    glEnable(GL_LIGHT0);                // 光源を1個使う

    window_width = INITIAL_WIDTH;
    window_height = INITIAL_HHEIGHT;
    refresh_msec = INITIAL_MSEC;
    state = MENU;
}

// メイン関数
int main(int argc, char *argv[]) {

    printf("application start.\n");

    glutInitWindowSize(INITIAL_WIDTH, INITIAL_HHEIGHT);   // 最初に出るウィンドウのサイズは 1280x720 なんとなくこれ
    glutInitWindowPosition(0, 40);   // ウィンドウの出現位置は(0,40) なんとなくこれ
    glutInit(&argc, argv);          // glutの初期化
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);    // 色と奥行きとダブルバッファリング
    glutCreateWindow(GAME_NAME);                      // ウィンドウの名前はmyapp なんとなく

    glutTimerFunc(refresh_msec, timer, 0);    // 固定フレームレートで実行
    glutDisplayFunc(display);               // 描画関数
    glutReshapeFunc(reshape);               // ウィンドウサイズが変わった時の関数
    glutKeyboardFunc(keyboard);             // キーボードの受け取り
    glutSpecialFunc(special);               // 特殊キーが押された時
    glutMouseFunc(mouse);                   // マウスの受け取り
    glutMotionFunc(motion);                 // マウスドラッグの受け取り
    glutPassiveMotionFunc(passiveMotion);   // マウスの動きの受け取り

    init();                                 // 適切な初期化をする
    glutMainLoop();                         // 開始

    return 0;
}
