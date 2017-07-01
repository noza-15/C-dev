#include "global.h"
#include "player.h"

int frame_count;
unsigned int refresh_msec;

static int jumped_frame = -1;          // player object
static double jump_v    = 0;        // player object
static double jump_z    = 0;        // player object

#define GRAVITY    17.0             // player object
#define JUMP_POWER 13.0             // player object

// 前回のジャンプからの経過時間 player object
static double getJumpTime(void){
    if(frame_count > jumped_frame)
        return (frame_count-jumped_frame) * refresh_msec / 1000.0;
    else
        return (frame_count-jumped_frame + MAX_FRAME) * refresh_msec / 1000.0;
}

void playerInit(void){
    jumped_frame = -1;          // player object
    jump_v    = 0;              // player object
    jump_z    = 0;              // player object
}

// ジャンプした時 player object interface
void jump(void){
    double second = getJumpTime();
    if(jumped_frame<0){ // 初めてのジャンプなら
        jump_z = 0.0;
        jump_v = JUMP_POWER;
    }else{              // n度めのジャンプなら
        jump_z = jump_z + jump_v*second - GRAVITY/2.0*second*second;
        jump_v = jump_v - GRAVITY*second + JUMP_POWER;
    }
    jumped_frame = frame_count;
}

// プレイヤーの位置を決める関数 player object
double getPlayerPosition(void){
    // 前回のジャンプからの経過時間を計算
    double second = getJumpTime();

    double z;
    if(jumped_frame < 0)   z = 0.0;                             // まだジャンプしたことないなら0
    else                    z = jump_z + jump_v*second - GRAVITY/2.0*second*second;   // ジャンプしたことあるなら

    if(z>20.0)z = 20.0;
    if(z< -20.0)z = -20.0;

    return z;
}
