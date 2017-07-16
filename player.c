
#include <GL/glut.h>
#include "global.h"
#include "player.h"

static int jumped_frame = -1;          // player object
static double jump_v = 0;        // player object
static double jump_z = 0;        // player object

#define GRAVITY    10.8             // player object
#define JUMP_POWER 10.0             // player object

// 前回のジャンプからの経過時間 player object
static double getJumpTime(void) {
    if (frame_count > jumped_frame)
        return (frame_count - jumped_frame) * refresh_msec / 1000.0;
    else
        return (frame_count - jumped_frame + MAX_FRAME) * refresh_msec / 1000.0;
}

// ジャンプした時 player object interface
void jump(void) {
//void jump(double v) {
    double second = getJumpTime();
//    jump_z += v;
//    jump_v = v;
//    if (jump_z > 0) {
//        jump_v -= 0.01;
//        jump_z += jump_v;
//        if (jump_z <= 0) {
//            jump_z = 0;
//            jump_v = 0;
//        }
//    }


    if (jumped_frame < 0) { // 初めてのジャンプなら
        jump_z = 0.0;
        jump_v = JUMP_POWER;
    } else {              // n度めのジャンプなら
        jump_z = jump_z + jump_v * second - GRAVITY / 2.0 * second * second;
        jump_v = jump_v - GRAVITY * second + JUMP_POWER;
    }

    jumped_frame = frame_count;
}

// プレイヤーの位置を決める関数 player object
double getPlayerPosition(void) {
    // 前回のジャンプからの経過時間を計算
    double second = getJumpTime();

    double z;
    if (jumped_frame < 0) z = 0.0;                             // まだジャンプしたことないなら0
    else z = jump_z + jump_v * second - GRAVITY / 2.0 * second * second;   // ジャンプしたことあるなら

    if (z > 20.0)z = 20.0;
    if (z < -20.0)z = -20.0;

    return z;
}

// プレイヤーの初期化
void initPlayer(void) {
    jumped_frame = -1;          // player object
    jump_v = 0;              // player object
    jump_z = 0;              // player object
}

// プレイヤーを描画する関数
void renderPlayer(void) {

    double rad = frame_count / 5.0;
    double z = getPlayerPosition();         // プレイヤーのz座標を取得

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();                 // 現在の変換行列を記憶
    glTranslatef(0.0, z, 0.0);      // 平行移動の行列をかける
    glRotated(0.0, 1.0, 0.0, 0.0);  // x軸回転行列をかける
    glRotated(rad, 0.0, 1.0, 0.0);  // y軸回転行列をかける
    glRotated(0.0, 0.0, 0.0, 1.0);  // z軸回転行列をかける
    glutSolidCube(1);               // モデルの描画
    glPopMatrix();                  // 変換行列を記憶した行列に戻す

}

void endPlayer(void) {

}
