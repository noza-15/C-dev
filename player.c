
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "global.h"
#include "player.h"

static int jumped_frame = -1;          // player object
static double jump_v = 0;        // player object
static double jump_z = 0;        // player object
static double jump_z0 = 0;        // player object
static int modelNum = 0;

#define GRAVITY    20              // player object
#define JUMP_POWER 7.0             // player object

// 前回のジャンプからの経過時間 player object
static double getJumpTime(void) {
    if (frame_count > jumped_frame) {
        return (frame_count - jumped_frame) * refresh_msec / 1000.0;
    } else {
        return (frame_count - jumped_frame + MAX_FRAME) * refresh_msec / 1000.0;
    }
}

// ジャンプした時 player object interface
void jump(void) {

    if (jumped_frame < 0) { // 初めてのジャンプなら
        jump_z0 = 0.0;
    } else {              // n度めのジャンプなら
        jump_z0 = jump_z;
    }
    jump_v = JUMP_POWER;
    jumped_frame = frame_count;
}

// プレイヤーの位置を決める関数 player object
double getPlayerPosition(void) {
    // 前回のジャンプからの経過時間を計算
    double second = getJumpTime();

    if (jumped_frame < 0) {
        jump_z = 0.0;                             // まだジャンプしたことないなら0
    } else {
        jump_z = jump_z0 + jump_v * second - GRAVITY / 2.0 * second * second;
    }

    return jump_z;
}

// プレイヤーの初期化
void initPlayer(void) {
    jumped_frame = -1;          // player object
    jump_v = 0;              // player object
    jump_z = 0;              // player object
}

// プレイヤーを描画する関数
void renderPlayer(void) {

	double rad = frame_count;// / 5.0;
    double z = getPlayerPosition();         // プレイヤーのz座標を取得

    if (z > UPPER_FRAME_OUT || z < LOWER_FRAME_OUT) {
        state = GAME_OVER;
    }

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();                 // 現在の変換行列を記憶
    glTranslatef(0.0, z, 0.0);      // 平行移動の行列をかける
	glRotatef(180 * sin(1.0*rad / 15), 1.0, 0.0, 0.0);  // x軸回転行列をかける
    glRotatef(180 * sin(1.0*rad / 45), 0.0, 1.0, 0.0);  // y軸回転行列をかける
    glRotatef(180 * sin(1.0*rad / 30), 0.0, 0.0, 1.0);  // z軸回転行列をかける
	switch (modelNum)
	{
	case 0:
		glutSolidCube(0.8);               // モデルの描画
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}
    glPopMatrix();                  // 変換行列を記憶した行列に戻す

}

void endPlayer(void) {

}
