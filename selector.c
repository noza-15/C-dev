
#include <GL/glut.h>
#include "global.h"
#include "selector.h"
#include <math.h>
#include "player.h"
#include "game.h"

#include <stdio.h> // debug

static int SELECTOR_INIT_FLAG = UNFINISHED;
static double rad = 0;
// ゲーム開始時に実行する
static void selector_init(void){
    printf("now selector initialize.\n");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (double)viewport_width/ (double)viewport_height, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

static void printString_over(char *str, int x0, int y0) {
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

// ゲームを終わる時に実行する
static void selector_exit(void){
    printf("now selector exit.\n");
}

// 選択画面の描画関数
void selector_disp(void){
    if(SELECTOR_INIT_FLAG == UNFINISHED){
        selector_init();
        SELECTOR_INIT_FLAG = FINISHED;
    }
	glDisable(GL_LIGHTING);
	glColor3d(0.9, 0.3, 0.3);
	glEnable(GL_LIGHTING);
    glViewport(viewport_start_x,viewport_start_y,viewport_width,viewport_height);
    glClearColor(0.8,0.3,0.3,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	rad += 0.5;
	glPushMatrix();
	glRotatef(180 * sin(1.0*rad / 15), 1.0, 0.0, 0.0);  // x軸回転行列をかける
	glRotatef(180 * sin(1.0*rad / 45), 0.0, 1.0, 0.0);  // y軸回転行列をかける
	glRotatef(180 * sin(1.0*rad / 30), 0.0, 0.0, 1.0);  // z軸回転行列をかける
	switch (getModelNum())
	{
	case 0:
		glutSolidCube(0.8);               // モデルの描画
		break;
	case 1:
		glScalef(0.5, 0.5, 0.5);
		glutWireIcosahedron();
		break;
	case 2:
		glScalef(0.3, 0.3, 0.3);
		glutWireDodecahedron();
		break;
	default:
		break;
	}
	glPopMatrix();
	glColor3d(0, 0, 0);
	printString_over("space  :  change unit", viewport_width / 2, viewport_height / 2 + 60);
	printString_over("  <-   :  back menu", viewport_width / 2, viewport_height / 2 + 80);
	printString_over("  ->   :  start!!", viewport_width / 2, viewport_height / 2 + 100);
    glutSwapBuffers();
}

// 選択画面でのキーボード入力
void selector_keyboard(unsigned char key, int x, int y){
	switch (key) {
	case ' ':
		changeModel();
	}
}

// 選択画面での特殊キー入力
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

// 選択画面でのマウス入力
void selector_mouse(int button, int mouse_state, int x, int y){

}

// 選択画面でのマウスドラッグ入力
void selector_motion(int x, int y){

}

// 選択画面でのマウスの動き入力
void selector_passiveMotion(int x, int y){

}

// 画面に文字列を表示する関数
