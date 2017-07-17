
#include <stdlib.h>
#include "global.h"
#include "obstacle.h"
#include "player.h"

#include "hoge.h"
#include "huga.h"
#include "missile.h"
#include "speed.h"

#include <stdio.h> // debug

#define OBSTACLE_NUMBER 4   // 全ての障害物の種類の数

// 障害物定義リスト
static Obstacle *ObstacleDefenition;

// 実際に存在するものの定義
typedef struct {
    int key;
    double *param;
    int birth;
} resident;

static resident *residentList;     // 存在する障害物のリスト
static int residentList_start;
static int residentList_end;

#define MAX_RESIDENT 100    // 存在して良い障害物の上限

static int wave=1;
static int WAVE_CHANGED_FLAG = 0;

// 確率的に障害物を発生して返す関数
static resident residentBirth(void) {
	int i = rand() % ((int)wave /( OBSTACLE_NUMBER-1) + 1);
	if (i > OBSTACLE_NUMBER-1)i = rand()%(OBSTACLE_NUMBER);
    //int i = 0;
    resident x;
    double z = getPlayerPosition();
    x.key = i;
    x.param = ObstacleDefenition[i].init(z);
    x.birth = frame_count;
    return x;
}

// 各フレームごとに実行する
// 確率や条件的に障害物を発生させるかさせないかを決めて障害物リストを更新する関数
// 消したり生成したり
void refreshResidents(void) {
    if (frame_count % (100-(int)(wave/2)*12) == 0) {
        residentList[residentList_end++] = residentBirth();
    }
    int j;
    for (j = residentList_start; j < residentList_end; j++) {
        if (residentList[j].birth < frame_count - 700) {
            residentList_start = j + 1;
        };
    }
}

// 全ての障害物と衝突していないか判定する関数
int judgeCollision(double z) {
    int i;
    for (i = residentList_start; i < residentList_end; i++) {
        resident obj = residentList[i];
        if (ObstacleDefenition[obj.key].judge(obj.birth, obj.param, z)) {
            return 1;
        }
    }
    return 0;
}

// 障害物の初期化
void initObstacles(void) {
    ObstacleDefenition = malloc(sizeof(Obstacle) * OBSTACLE_NUMBER);
    ObstacleDefenition[0] = getSpeedDefinition();
    ObstacleDefenition[1] = getHugaDefinition();
    ObstacleDefenition[2] = getHogeDefinition();
    ObstacleDefenition[3] = getMissileDefinition();
    residentList = malloc(sizeof(resident) * 100);
    residentList_start = 0;
    residentList_end = 0;
}

// 全ての障害物を描画する
void renderObstacles(void) {
    //printf("call render obstacles\n");
    double z = getPlayerPosition();
    int i;
    for (i = residentList_start; i < residentList_end; i++) {
        resident obj = residentList[i];
        ObstacleDefenition[obj.key].render(obj.birth, obj.param, z);
    }
}

// 障害物の終了
void endObstacles(void) {
    free(residentList);
}

void changeWave(void) {
	wave+=1;
	WAVE_CHANGED_FLAG = 30;
	if (wave > 15)wave = 15;
}

int getWave(void) {
	return wave;
}

void resetWave(void) {
	wave = 1;
}

int getWaveChangedFlag(void) {
	if (WAVE_CHANGED_FLAG>0) {
		WAVE_CHANGED_FLAG --;
		return 1;
	}
	else {
		return 0;
	}
}